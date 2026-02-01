import re
from dataclasses import dataclass, field
from collections import defaultdict
import os
from os.path import dirname, isfile, join
from typing import Dict, List
import logging

DEFAULT_HEADER_URL = "https://raw.githubusercontent.com/webgpu-native/webgpu-headers/main/webgpu.h"

def makeArgParser():
    import argparse

    parser = argparse.ArgumentParser(description="""
    Generate the webgpu-cpp binding from official webgpu-native headers.
    You should not have to change any of the default arguments for a regular use.

    This generates a webgpu.hpp file that you can include in your project.
    Exactly one of your source files must #define WEBGPU_CPP_IMPLEMENTATION
    before including this header.
    """)

    parser.add_argument("-o", "--output", type=str,
                        default="webgpu.hpp",
                        help="Path where to output the generated webgpu.hpp")

    parser.add_argument("-u", "--header-url", action='append',
                        default=[],
                        help=f"""
                        URL of the official webgpu.h from the webgpu-native project. If the URL
                        does not start with http(s)://, it is considered as a local file. You can
                        specify this option multiple times to agregate multiple headers (e.g.,
                        the standard webgpu.h plus backend-specific extensions wgpu.h).
                        If no URL is specified, the official header from '{DEFAULT_HEADER_URL}'
                        is used.
                        """)

    parser.add_argument("--ext-suffix",
                        default="",
                        help="""
                        Extension number needed for Dawn, which uses them to maintain backward
                        compatibility of their API (might get removed once version 1 is out).
                        Set to "2" when using dawn and leave to the empty default with wgpu-native.
                        """)

    parser.add_argument("--use-inline", action='store_true', dest="use_inline",
                        help="Make all methods inlined (seems to have an effect with clang, but MSVC fails at linking in that case).")

    parser.add_argument("--use-raw-namespace", action='store_true', dest="use_raw_namespace",
                        help="Put non-RAII handle types into the nested namespace 'raw' (wgpu::raw) and make structs/descriptors use them.")
    
    parser.add_argument("--namespace", type=str,
                        default="wgpu",
                        help="Namespace to use for the generated binding.")

    return parser

logging.basicConfig(level=logging.INFO, format="%(levelname)s %(message)s")

def main(args):
    applyDefaultArgs(args)

    if args.version:
        print("WebGPU-C++ generator v2.0.0")
        return

    template, meta = loadTemplate(args.template)
    api = WebGpuApi()
    for url in args.header_url:
        header = downloadHeader(url)
        parseHeader(api, header)

    binding = produceBinding(args, api, meta, template)
    
    generateOutput(args, template, binding)

def applyDefaultArgs(args):
    if hasattr(args, "virtual_fs"):
        VfsFile.virtual_fs = args.virtual_fs

# -----------------------------------------------------------------------------
# Virtual File System, to enable using this script as a lib (in an environement
# where there is no file system).

class VfsFile():
    virtual_fs = None
    def __init__(self, filename):
        self.filename = filename
    def __enter__(self):
        stream = VfsFile.virtual_fs[self.filename]
        stream.seek(0)
        return stream
    def __exit__(self, exc_type, exc_val, exc_tb):
        pass

def openVfs(filename, mode='r', **kwargs):
    """
    This is a wrapper around the standard 'open' that enables paths starting
    with "vfs://" to refer to files in a virtual file system.
    """
    if filename.startswith("vfs://"):
        return VfsFile(filename[6:])
    else:
        return open(filename, mode, **kwargs)

def isfileVfs(filename):
    if filename.startswith("vfs://"):
        return filename[6:] in VfsFile.virtual_fs
    else:
        return isfile(filename)

# -----------------------------------------------------------------------------
# Parser, for analyzing webgpu.h
# The output of parsing is a WebGpuApi object

@dataclass
class PropertyApi:
    name: str
    type: str
    counter: str|None = None  # list properties have an associated counter property

@dataclass
class HandleApi:
    """WebGPU objects manipulated through blind handles"""
    name: str
    properties: list[PropertyApi] = field(default_factory=list)

@dataclass
class ClassApi:
    """WebGPU structs whose fields are directly manipulated"""
    name: str
    parent: str|None = None
    properties: list[PropertyApi] = field(default_factory=list)
    is_descriptor: bool = False
    default_overrides: list[(str,str)] = field(default_factory=list)

@dataclass
class ProcedureArgumentApi:
    name: str
    type: str
    nullable: bool = False

@dataclass
class ProcedureApi:
    name: str
    return_type: str
    parent: str|None = None
    arguments: list[ProcedureArgumentApi] = field(default_factory=list)

@dataclass
class EnumerationEntryApi:
    key: str
    value: str

@dataclass
class EnumerationApi:
    name: str
    entries: list[EnumerationEntryApi] = field(default_factory=list)
    is_real_enum: bool = False

@dataclass
class CallbackApi:
    name: str
    arguments: list[ProcedureArgumentApi] = field(default_factory=list)
    raw_arguments: str = ""

@dataclass
class TypeAliasApi:
    aliased_type: str
    wgpu_type: str

@dataclass
class WebGpuApi:
    handles: list[HandleApi] = field(default_factory=list)
    classes: list[ClassApi] = field(default_factory=list)
    procedures: list[ProcedureApi] = field(default_factory=list)
    enumerations: list[EnumerationApi] = field(default_factory=list)
    callbacks: list[CallbackApi] = field(default_factory=list)
    type_aliases: list[TypeAliasApi] = field(default_factory=list)
    stypes: dict[str,str] = field(default_factory=dict) # Name => SType::Name
    init_macros: list[str] = field(default_factory=list)

def parseHeader(api: WebGpuApi, header: str):
    """
    Add fields to api while reading a header file
    """
    it = iter([
        line
            .replace("WGPU_OBJECT_ATTRIBUTE", "")
            .replace("WGPU_ENUM_ATTRIBUTE", "")
            .replace("WGPU_STRUCTURE_ATTRIBUTE", "")
            .replace("WGPU_FUNCTION_ATTRIBUTE", "")
            #.replace("WGPU_NULLABLE", "")
        for line in header.split("\n")
    ])
    
    struct_re = re.compile(r"struct *WGPU(\w+) *{")
    handle_re = re.compile(r"typedef struct .*WGPU([^_]\w+)\s*;")
    procedure_re = re.compile(r"(?:WGPU_EXPORT\s+)?([\w *]+) wgpu(\w+)\((.*)\)\s*;")
    enum_re = re.compile(r"typedef enum WGPU(\w+) {")
    flag_enum_re = re.compile(r"typedef WGPUFlags WGPU(\w+)Flags\s*;")
    new_flag_enum_re = re.compile(r"typedef WGPUFlags WGPU(\w+)\s*;")
    flag_value_re = re.compile(r"static const WGPU(\w+) WGPU(\w+)_(\w+) = (\w+)( /\*(.*)\*/)?;")
    typedef_re = re.compile(r"typedef (\w+) WGPU(\w+)\s*;")
    callback_re = re.compile(r"typedef void \(\*WGPU(\w+)Callback\)\((.*)\)\s*;")
    init_macro_re = re.compile(r"#define (WGPU_[A-Z0-9_]+_INIT)")

    while (x := next(it, None)) is not None:
        if (match := struct_re.search(x)):
            struct_name = match.group(1)
            api.classes.append(parseClass(struct_name, it))
            continue

        if (match := handle_re.search(x)):
            struct_name = match.group(1)
            api.handles.append(HandleApi(name=struct_name))
            continue

        if (match := procedure_re.search(x)):
            return_type = match.group(1)
            if return_type.startswith("WGPU_EXPORT"):
                return_type = return_type[len("WGPU_EXPORT"):]
            return_type = return_type.strip()
            if return_type.startswith("WGPU_NULLABLE"):
                return_type = return_type[len("WGPU_NULLABLE"):]
            return_type = return_type.strip()
            api.procedures.append(ProcedureApi(
                name=match.group(2),
                return_type=return_type,
                arguments=parseProcArgs(match.group(3)),
            ))
            continue

        if (match := enum_re.search(x)):
            name = match.group(1)
            api.enumerations.append(parseEnum(name, it, api.stypes))
            continue

        if (match := flag_enum_re.search(x)):
            api.type_aliases.append(TypeAliasApi(
                aliased_type="WGPUFlags",
                wgpu_type=match.group(1) + "Flags",
            ))
            continue

        if (match := new_flag_enum_re.search(x)):
            api.enumerations.append(EnumerationApi(
                name=match.group(1),
            ))
            continue

        if (match := flag_value_re.search(x)):
            enum_name = match.group(1)
            enum_name2 = match.group(2)
            entry = EnumerationEntryApi(
                key=match.group(3),
                value=f"WGPU{match.group(2)}_{match.group(3)}",
            )
            assert(enum_name == enum_name2)
            found = False
            for enum in api.enumerations:
                if enum.name == enum_name:
                    enum.entries.append(entry)
                    found = True
                    break
            if not found:
                api.enumerations.append(EnumerationApi(
                    name=enum_name,
                    entries=[ entry ]
                ))
            continue

        if (match := typedef_re.search(x)):
            api.type_aliases.append(TypeAliasApi(
                aliased_type=match.group(1),
                wgpu_type=match.group(2),
            ))
            continue

        if (match := callback_re.search(x)):
            api.callbacks.append(CallbackApi(
                name=match.group(1),
                arguments=parseProcArgs(match.group(2)),
                raw_arguments=match.group(2),
            ))
            continue

        if (match := init_macro_re.search(x)):
            api.init_macros.append(match.group(1))
            continue

    # Post process: find parent of each method
    for proc in api.procedures:
        maxi = 0
        parent_names = (
            [ handle.name for handle in api.handles ] +
            [ desc.name for desc in api.classes ]
        )
        for parent in parent_names:
            if len(parent) > maxi and proc.name.startswith(parent) and len(parent) < len(proc.name):
                proc.parent = parent
                maxi = len(parent)
        if proc.parent is not None:
            proc.name = proc.name[maxi:]

    return api

def parseEnum(name, it, stypes):
    entry_re = re.compile(r"^\s+WGPU([^_]+)_([\w_]+) = ([^,]+),?")
    end_re = re.compile(".*}")

    api = EnumerationApi(name=name, is_real_enum=True)

    while (x := next(it, None)) is not None:
        if (match := entry_re.search(x)):
            prefix = match.group(1)
            key = match.group(2)
            #value = match.group(3)
            value = f"WGPU{prefix}_{key}"
            api.entries.append(EnumerationEntryApi(key, value))

            if "WGPUSType_" in x:
                cast = "(WGPUSType)" if name != "SType" else ""
                stypes[key] = cast + name + "::" + key

        elif (match := end_re.search(x)):
            break

    return api

def parseClass(name: str, it: iter[str]) -> ClassApi:
    api = ClassApi(name=name)
    end_of_struct_re = re.compile(r".*}")
    property_re = re.compile(r"^\s*(.+) (\w+);$")

    count_properties = []
    x = next(it)
    while not end_of_struct_re.search(x):
        if (match := property_re.search(x)):
            prop = PropertyApi(name=match.group(2), type=match.group(1))
            if prop.name == "nextInChain":
                api.is_descriptor = True
            if prop.name == "chain" and api.parent is not None:
                pass
            if prop.name[-5:] == "Count":
                count_properties.append(prop)
            api.properties.append(prop)
        x = next(it)

    for counter in count_properties:
        # entri|ies -> entr|yCount
        # colorFormat|s -> colorFormat|sCount
        prefix = counter.name[:-6]
        found = False
        for r in api.properties:
            if r.name == counter.name:
                continue
            if "*" not in r.type:
                continue
            if r.name.startswith(prefix):
                r.counter = counter.name
                found = True
                break
        if not found:
            pass # Already appended

    return api

def parseProcArgs(line):
    args = []
    for entry in line.split(","):
        entry = entry.strip()
        nullable = False
        if entry.endswith("/* nullable */"):
            nullable = True
            entry = entry[:-14].strip()
        if entry.startswith("WGPU_NULLABLE"):
            nullable = True
            entry = entry[13:].strip()
        tokens = entry.split()
        args.append(ProcedureArgumentApi(
            name=tokens[-1],
            type=" ".join(tokens[:-1]),
            nullable=nullable
        ))
    return args

# -----------------------------------------------------------------------------

def produceBinding(args, api, meta, template):
    """Produce C++ binding"""
    binding = {
        "webgpu_includes": [],
        "descriptors": [],
        "structs": [],
        "defaults_impl": [],
        "class_impl": [],
        "class_oneliner": [],
        "handles_decl": [],
        "handles": [],
        "handles_impl": [],
        "handles_oneliner": [],
        "enums": [],
        "structs_decl": [],
        "descriptors_decl": [],
        "callbacks": [],
        "procedures": [],
        "type_aliases": [],
        "c_exports": [],
        "to_string_decl": [],
        "to_string_impl": [],
        "handle_template_impl": [],
        "class_template_impl": [],
        "handle_raii": [],
        "ext_suffix": args.ext_suffix,
    }

    for url in args.header_url:
        filename = os.path.split(url)[1]
        binding["webgpu_includes"].append(f"#include <webgpu/{filename}>")

    # Prepare macro injection for fake enums (static const) to allow re-exporting them
    fake_enum_values = []
    for e in api.enumerations:
        if not e.is_real_enum:
            for entry in e.entries:
                fake_enum_values.append(entry.value)
    
    defines_block = []
    undefs_block = []
    for val in fake_enum_values:
        defines_block.append(f"#define {val} {val}_Internal")
        undefs_block.append(f"#undef {val}")

    if "{c_exports}" in template:
        binding["webgpu_includes"] = (
            defines_block + 
            binding["webgpu_includes"] + 
            undefs_block
        )

    # Cached variables for format_arg
    handle_names = [ h.name for h in api.handles ]
    handle_cptr_names = [ f"{h.name} const *" for h in api.handles ]
    handle_ptr_names = [ f"{h.name} *" for h in api.handles ]
    class_cptr_names = [ f"{d.name} const *" for d in api.classes ]
    enum_names = [ e.name for e in api.enumerations ]
    enum_ptr_names = [ f"{e.name} *" for e in api.enumerations ]
    simple_class_names = { d.name for d in api.classes }
    
    # Identify structs that are returned by value or passed as mutable pointer
    owned_structs = set()
    for proc in api.procedures:
        # Check return type
        ret = proc.return_type
        if ret.startswith("WGPU_EXPORT"): ret = ret[11:].strip()
        if ret.startswith("WGPU"): ret = ret[4:]
        if ret in simple_class_names:
            owned_structs.add(ret)
        
        # Check arguments
        for arg in proc.arguments:
            t = arg.type
            if t.endswith("const *"): continue
            if "*" in t:
                # Mutable pointer
                clean = t.replace("*", "").strip()
                if clean.startswith("struct WGPU"): clean = clean[11:] # Handle "struct WGPU..."
                elif clean.startswith("WGPU"): clean = clean[4:]
                if clean in simple_class_names:
                    owned_structs.add(clean)

    callbacks = {
        f"{cb.name}Callback": cb
        for cb in api.callbacks
    }

    def get_cpp_name(base_name, force_raii=False):
        ns = args.namespace + "::" if args.namespace else ""
        if base_name in handle_names:
            if args.use_raw_namespace and not force_raii:
                 return f"{ns}raw::{base_name}"
            return f"{ns}{base_name}"
        if base_name in simple_class_names or base_name in enum_names:
             return f"{ns}{base_name}"
        return base_name

    def format_arg(arg, force_raii_return=False):
        """
        Given a function argument, return it (i) as an argument *received* from
        the C++ API and (ii) as an argument *passed* to the C API and (iii) as
        an argument passed to the C++ API.
        Also tells (iv) whether the next should be skipped (used only for the
        userdata pointer passed to callbacks).
        """
        arg_type = arg.type
        arg_c = arg.name
        arg_cpp = arg.name
        skip_next = False
        
        # Cleanup type to identify base type
        clean_type = arg_type.replace("struct ", "").strip()
        
        is_ptr = "*" in clean_type
        is_const = "const" in clean_type
        
        base_candidates = clean_type.replace("const", "").replace("*", "").strip()
        if base_candidates.startswith("WGPU"):
            base_candidates = base_candidates[4:]
            
        if base_candidates in simple_class_names:
            # It is a wrapped class (Descriptor or Struct)
            cpp_base = get_cpp_name(base_candidates)
            c_base = "WGPU" + base_candidates
            
            # Determine C++ signature type: default to pointers to avoid nullability issues and simplify casting
            cpp_sig = cpp_base
            if is_const: cpp_sig = "const " + cpp_sig
            if is_ptr: cpp_sig += "&"
            
            c_cast = c_base
            if is_const: c_cast = "const " + c_cast
            
            if is_ptr:
                 cast_type_ptr = c_cast + " *"
                 arg_c = f"reinterpret_cast<{cast_type_ptr}>(&{arg.name})"
                 
                 # arg_cpp callback conversion
                 cpp_sig_ptr = cpp_sig[:-1] + "*"
                 arg_cpp = f"*reinterpret_cast<{cpp_sig_ptr}>({arg.name})"
            else:
                 # Value argument (e.g. StringView): Cast address and dereference
                 cast_type_ptr = c_cast + " *"
                 arg_c = f"*reinterpret_cast<{cast_type_ptr}>(&{arg.name})"

                 cpp_sig_ptr = cpp_sig + " *"
                 arg_cpp = f"*reinterpret_cast<{cpp_sig_ptr}>(&{arg.name})"
            
            return f"{cpp_sig} {arg.name}", arg_c, arg_cpp, skip_next

        # Fallback to old logic for Handles/Enums/Callbacks
        if arg_type.startswith("struct "):
            arg_type = arg_type[len("struct "):]
        if arg_type.startswith("WGPU"):
            arg_type = arg_type[len("WGPU"):]

        # keep the base name for matching (used to decide raw:: prefix)
        base_arg_type = arg_type

        if arg_type in callbacks:
            arg_type = f"{arg_type}&&"
            arg_c = "cCallback"
            skip_next = True
        elif arg_type in handle_cptr_names:
            arg_c = f"reinterpret_cast<WGPU{arg_type}>({arg_c})"
        elif arg_type in handle_ptr_names:
            arg_c = f"reinterpret_cast<WGPU{arg_type}>({arg_c})"

        if args.use_scoped_enums:
            if arg_type in enum_names:
                arg_c = f"static_cast<WGPU{arg_type}>({arg_c})"
                arg_cpp = f"static_cast<{get_cpp_name(arg_type)}>({arg_cpp})"
            elif arg_type in enum_ptr_names:
                arg_c = f"reinterpret_cast<WGPU{arg_type}>({arg_c})"

        # If user wants non-RAII handles under wgpu::raw, present signature types accordingly
        final_arg_type = base_arg_type
        if base_arg_type in handle_names:
             is_out_param = "*" in arg.type and "const" not in arg.type and base_arg_type in handle_names
             is_raii = is_out_param or force_raii_return
             final_arg_type = get_cpp_name(base_arg_type, force_raii=is_raii)
        elif base_arg_type in enum_names:
             final_arg_type = get_cpp_name(base_arg_type)

        sig_cpp = f"{final_arg_type} {arg.name}"

        return sig_cpp, arg_c, arg_cpp, skip_next

    maybe_inline = "inline " if args.use_inline else ""
    class_names = [f"WGPU{c.name}" for c in api.classes]
    classes_and_handles = (
        [ ('CLASS', cls_api) for cls_api in api.classes ] +
        [ ('HANDLE', handle_api) for handle_api in api.handles ]
    )
    for entry_type, handle_or_class in classes_and_handles:
        entry_name = handle_or_class.name
        if entry_type == 'CLASS':
            macro = "DESCRIPTOR" if handle_or_class.is_descriptor else "STRUCT"
            namespace = "descriptors" if handle_or_class.is_descriptor else "structs"
            binding[namespace + "_decl"].append(f"struct {entry_name};")
            namespace_impl = "class_impl"
            namespace_oneliner = "class_oneliner"
            argument_self = "*this"
            use_const = False
        elif entry_type == 'HANDLE':
            binding["handles_decl"].append(f"class {entry_name};")
            macro = "HANDLE"
            namespace = "handles"
            namespace_impl = "handles_impl"
            namespace_oneliner = "handles_oneliner"
            argument_self = "m_raw"
            use_const = args.use_const
            # Also produce a RAII alias entry using HANDLE_RAII; second parameter points to underlying handle type
            # Produce a RAII alias in the wgpu namespace that wraps the underlying (possibly raw) handle.
            if args.use_raw_namespace:
                binding["handle_raii"].append(f"HANDLE_RAII({entry_name}, raw::{entry_name})\nEND")
            else:
                binding["handle_raii"].append(f"HANDLE_RAII({entry_name}, {entry_name})\nEND")

        if entry_name.startswith("INTERNAL__"):
            continue
        
        decls = []
        implems = []
        defaults_implems = []
        template_implems = []

        # Auto-generate setDefault
        if entry_type == 'CLASS':
            # Generate fields
            fields_declaration = []
            cls_api = handle_or_class
            prop_names = [f"{p.name}" for p in cls_api.properties]
            
            for prop in cls_api.properties:
                c_type = prop.type
                is_ptr = "*" in c_type
                is_const = "const" in c_type
                base_type = c_type.replace("const", "").replace("*", "").replace("WGPU_NULLABLE", "").strip()
                
                force_raii_field = (entry_name in owned_structs)

                cpp_base_type = base_type
                if base_type.startswith("WGPU"):
                    candidate = base_type[4:]
                    if candidate in enum_names or ("WGPU" + candidate) in class_names:
                        cpp_base_type = get_cpp_name(candidate)
                    elif candidate in handle_names:
                        # Use raw namespace handle for fields if requested, unless forced RAII
                        cpp_base_type = get_cpp_name(candidate, force_raii=force_raii_field)
                elif base_type == "WGPUBool":
                     cpp_base_type = "Bool"

                cpp_type = cpp_base_type
                if is_const: cpp_type = "const " + cpp_type
                if is_ptr: cpp_type = cpp_type + " *"
                
                fields_declaration.append(f"\t{cpp_type} {prop.name};\n")
            
            decls.extend(fields_declaration)
            decls.append(f"\t{maybe_inline}{entry_name}& setDefault();\n")

            init_macro = f"WGPU_{toConstantCase(entry_name)}_INIT"
            if init_macro not in api.init_macros:
                logging.warning(f"Initialization macro '{init_macro}' was not found, falling back to empty initializer '{{}}'.")
                init_macro = "{}"
            prop_defaults = [
                f"\treinterpret_cast<WGPU{entry_name}&>(*this) = WGPU{entry_name} {init_macro};\n",
            ]
            if "chain" in prop_names:
                if entry_name in api.stypes:
                    default_val = api.stypes[entry_name]
                    if args.use_scoped_enums and default_val.startswith("(WGPUSType)"):
                        default_val = f"static_cast<{get_cpp_name('SType')}>({default_val[11:]})"

                    prop_defaults.extend([
                        f"\tchain.sType = {default_val};\n",
                        f"\tchain.next = nullptr;\n",
                    ])
                else:
                    logging.warning(f"Type {entry_name} starts with a 'chain' field but has no apparent associated SType.")
            defaults_implems.append(
                f"{maybe_inline}{entry_name}& {entry_name}::setDefault() " + "{\n"
                + "".join(prop_defaults)
                + "\treturn *this;\n"
                + "}\n"
            )

            # Analyze properties to identify counters
            prop_types = { p.name: p.type for p in cls_api.properties }
            hidden_counters = set()
            for p in cls_api.properties:
                if p.counter:
                    hidden_counters.add(p.counter)

            for prop in cls_api.properties:
                if prop.name == "chain": continue
                # Skip generating standalone setters for properties that are managed by an array setter
                if prop.name in hidden_counters: continue

                c_type = prop.type
                is_ptr = "*" in c_type
                is_const = "const" in c_type
                
                base_type = c_type.replace("const", "").replace("*", "").replace("WGPU_NULLABLE", "").strip()
                
                cpp_base_type = base_type
                if base_type.startswith("WGPU"):
                    candidate = base_type[4:]
                    if candidate in enum_names or candidate in handle_names or ("WGPU" + candidate) in class_names:
                        if candidate in handle_names:
                             force_raii_field = (entry_name in owned_structs)
                             cpp_base_type = get_cpp_name(candidate, force_raii=force_raii_field)
                        else:
                             cpp_base_type = get_cpp_name(candidate)
                elif base_type == "WGPUBool":
                    cpp_base_type = "bool"

                cpp_type = cpp_base_type
                if is_const: cpp_type = "const " + cpp_type
                if is_ptr: cpp_type = cpp_type + " *"

                setter_name = "set" + prop.name[0].upper() + prop.name[1:]
                
                arg_name = prop.name
                
                trans_expr = arg_name
                if cpp_base_type != base_type:
                    # Case 1: bool arg -> WGPUBool/Bool field
                    if cpp_base_type == "bool" and base_type == "WGPUBool":
                         trans_expr = f"static_cast<WGPUBool>({arg_name} ? 1 : 0)"
                    # Case 2: Enum arg -> Enum/int field (via cast)
                    elif cpp_base_type in enum_names:
                        # No cast needed if field is C++ type.
                        trans_expr = arg_name
                        pass

                # If field is C++ type, we might not need reinterpret_cast for assignment unless mismatched
                # Re-eval cpp_base_type for FIELD (might differ from argument if argument used 'bool')
                should_move = False
                field_base_type = base_type
                if base_type.startswith("WGPU"):
                    candidate = base_type[4:]
                    if candidate in enum_names or candidate in handle_names or ("WGPU" + candidate) in class_names:
                        if candidate in handle_names:
                             force_raii_field = (entry_name in owned_structs)
                             field_base_type = get_cpp_name(candidate, force_raii=force_raii_field)
                             if force_raii_field: should_move = True
                        else:
                             field_base_type = get_cpp_name(candidate)
                elif base_type == "WGPUBool":
                     field_base_type = "Bool" # Field is uint32_t/Bool
                
                field_type = field_base_type
                if is_const: field_type = "const " + field_type
                if is_ptr: field_type = field_type + " *"

                # Internal assignment logic: this->prop = trans_expr;
                if should_move:
                    trans_expr = f"std::move({trans_expr})"
                assignment = f"\tthis->{prop.name} = {trans_expr};\n"
                
                if prop.counter:
                    # Array Setter
                    counter_type = prop_types.get(prop.counter, "uint32_t").strip()
                    
                    # 1. Pointer + Count
                    decls.append(f"\t{maybe_inline}{entry_name}& {setter_name}({counter_type} {prop.counter}, {cpp_type} {prop.name});\n")
                    implems.append(
                        f"{maybe_inline}{entry_name}& {entry_name}::{setter_name}({counter_type} {prop.counter}, {cpp_type} {prop.name}) {{\n"
                        f"\tthis->{prop.counter} = {prop.counter};\n"
                        + assignment
                        + f"\treturn *this;\n"
                        f"}}\n"
                    )
                    
                    # 2\3. Vector & Span
                    types = [cpp_base_type]
                    # If this is a handle and we act as raw, also allow RAII input (which casts to raw)
                    raw_ns_sub = "raw::"
                    if args.use_raw_namespace and raw_ns_sub in cpp_base_type:
                        # Extract base handle name
                        # wgpu::raw::Buffer -> Buffer
                        parts = cpp_base_type.split("raw::")
                        if len(parts) > 1:
                            types.append(parts[0] + parts[1]) # wgpu:: + Buffer -> wgpu::Buffer

                    for loop_type in types:

                        data_access = f"reinterpret_cast<{field_type}>({prop.name}.data())"
                        
                        # 2. Vector
                        vec_type = f"std::vector<{loop_type}>"
                        vec_arg = f"const {vec_type}&" if is_const else f"{vec_type}&"
                        decls.append(f"\t{maybe_inline}{entry_name}& {setter_name}({vec_arg} {prop.name});\n")
                        implems.append(
                            f"{maybe_inline}{entry_name}& {entry_name}::{setter_name}({vec_arg} {prop.name}) {{\n"
                            f"\tthis->{prop.counter} = static_cast<{counter_type}>({prop.name}.size());\n"
                            f"\tthis->{prop.name} = {data_access};\n"
                            f"\treturn *this;\n"
                            f"}}\n"
                        )

                        # 3. Span
                        span_t = f"const {loop_type}" if is_const else loop_type
                        span_type = f"std::span<{span_t}>"
                        decls.append(f"\t{maybe_inline}{entry_name}& {setter_name}(const {span_type}& {prop.name});\n")
                        implems.append(
                            f"{maybe_inline}{entry_name}& {entry_name}::{setter_name}(const {span_type}& {prop.name}) {{\n"
                            f"\tthis->{prop.counter} = static_cast<{counter_type}>({prop.name}.size());\n"
                            f"\tthis->{prop.name} = {data_access};\n"
                            f"\treturn *this;\n"
                            f"}}\n"
                        )
                
                else:
                    # Generic Setter
                    decls.append(f"\t{maybe_inline}{entry_name}& {setter_name}({cpp_type} {prop.name});\n")
                    implems.append(
                        f"{maybe_inline}{entry_name}& {entry_name}::{setter_name}({cpp_type} {prop.name}) {{\n"
                        + assignment
                        + f"\treturn *this;\n"
                        f"}}\n"
                    )

        for proc in api.procedures:
            if proc.parent != entry_name:
                continue
            if "wgpu" + entry_name + proc.name + "\n" in meta["blacklist"]:
                logging.debug(f"Skipping wgpu{entry_name}{proc.name} (blacklisted)...")
                continue
            method_name = proc.name[0].lower() + proc.name[1:]

            arguments, argument_names = [], []
            skip_next = False
            for arg in proc.arguments[1:]:
                if skip_next:
                    skip_next = False
                    continue
                sig, name, _, skip_next = format_arg(arg)
                arguments.append(sig)
                argument_names.append(name)

            return_type = proc.return_type
            template_args = None

            # Wrap callback into std::function/Lambda
            if "userdata" == proc.arguments[-1].name: # OLD callback mechanism
                cb = callbacks[proc.arguments[-2].type[4:]]
                cb_name = proc.arguments[-2].name
                cb_arg_names = map(lambda a: format_arg(a)[2], cb.arguments[:-1])
                body = (
                      f"\tauto handle = std::make_unique<{get_cpp_name(cb.name)}Callback>({cb_name});\n"
                    + f"\tstatic auto cCallback = []({cb.raw_arguments}) -> void {{\n"
                    + f"\t\t{get_cpp_name(cb.name)}Callback& callback = *reinterpret_cast<{get_cpp_name(cb.name)}Callback*>(userdata);\n"
                    + f"\t\tcallback({', '.join(cb_arg_names)});\n"
                    + "\t};\n"
                    + "\t{wrapped_call};\n"
                    + "\treturn handle;\n"
                )
                argument_names.append(f"reinterpret_cast<void*>(handle.get())")
                return_type = f"std::unique_ptr<{get_cpp_name(cb.name)}Callback>"
                maybe_no_discard = "NO_DISCARD "
            elif proc.arguments[-1].type.endswith("CallbackInfo"): # NEW callback mechanism
                cb_type = proc.arguments[-1].type[4:-len("Info")]
                cb = callbacks[cb_type]
                cb_arg_names = [ format_arg(a)[2] for a in cb.arguments[:-2] ]
                cb_args = [ f"{a.type} {a.name}" for a in cb.arguments[:-2] ]
                
                cb_cpp_types = [ format_arg(a)[0].rsplit(' ', 1)[0] for a in cb.arguments[:-2] ]
                
                # Remove callbackInfo arg and add CallbackMode and Lambda
                arguments.pop()
                arguments.extend([ "CallbackMode callbackMode", "const Lambda& callback" ])

                invocable_args = ", ".join(cb_cpp_types)
                template_args = [ f"std::invocable<{invocable_args}> Lambda" ]

                body = "\n".join([
                    f"\tauto* lambda = new Lambda(callback);",
                    f"\tauto cCallback = []({', '.join(cb_args)}, void* userdata1, void*) -> void {{",
                    f"\t\tstd::unique_ptr<Lambda> lambda(reinterpret_cast<Lambda*>(userdata1));",
                    f"\t\t(*lambda)({', '.join(cb_arg_names)});",
                    "\t};",
                    f"\tWGPU{cb_type}Info callbackInfo = {{",
                    "\t\t/* nextInChain = */ nullptr,",
                    "\t\t/* mode = */ callbackMode,",
                    "\t\t/* callback = */ cCallback,",
                    "\t\t/* userdata1 = */ (void*)lambda,",
                    "\t\t/* userdata2 = */ nullptr,",
                    "\t};",
                    "\treturn {wrapped_call};",
                    ""
                ])
                maybe_no_discard = ""
            else:
                if proc.arguments[-1].type.endswith("CallbackInfo"):
                    print(f"- {entry_name}.{proc.name}: {proc.arguments[-1].name}")
                body = "\treturn {wrapped_call};\n"
                maybe_no_discard = ""

            argument_names_str = ', '.join([argument_self] + argument_names)

            begin_cast = ""
            end_cast = ""

            if return_type.startswith("WGPU"):
                return_type = return_type[4:]
            
            # Use utility, force RAII for handles (return type always RAII if it's a handle)
            return_type_cpp = get_cpp_name(return_type, force_raii=(return_type in handle_names))
            
            if args.use_scoped_enums:
                if return_type in enum_names:
                    begin_cast = f"static_cast<{return_type_cpp}>("
                    end_cast = ")"
            
            return_type = return_type_cpp
            
            wrapped_call = f"{begin_cast}wgpu{entry_name}{proc.name}({argument_names_str}){end_cast}"
            maybe_const = " const" if use_const else ""
            name_and_args = f"{method_name}({', '.join(arguments)}){maybe_const}"
            if template_args is None:
                decls.append(f"\t{maybe_inline}{maybe_no_discard}{return_type} {name_and_args};\n")
                implems.append(
                    f"{maybe_inline}{return_type} {entry_name}::{name_and_args} {{\n"
                    + body.replace("{wrapped_call}", wrapped_call)
                    + "}\n"
                )
            else:
                current_template_impl = (
                    "template<" + ", ".join(template_args) + ">\n"
                    + f"{maybe_inline}{return_type} {entry_name}::{name_and_args} {{\n"
                    + body.replace("{wrapped_call}", wrapped_call)
                    + "}\n"
                )
                template_implems.append(current_template_impl)
                
                decls.append(
                    "\ttemplate<" + ", ".join(template_args) + ">\n"
                    + f"\t{maybe_inline}{return_type} {name_and_args};\n"
                )

            # Add utility overload for arguments of the form 'uint32_t xxCount, Xx const * xx'
            for i in range(len(proc.arguments) - 1):
                a, b = proc.arguments[i], proc.arguments[i + 1]
                if a.type in {"uint32_t","size_t"} and a.name.endswith("Count"):
                    name = a.name[:-5]
                    if b.type.endswith("const *") and b.name.startswith(name):
                        vec_type = b.type[:-8]
                        vec_name = name if name.endswith("s") else name + "s"

                        cpp_type = vec_type
                        if cpp_type.startswith("struct "):
                            cpp_type = cpp_type[7:]
                        if cpp_type.startswith("WGPU"):
                            candidate = cpp_type[4:]
                            if candidate in enum_names or candidate in handle_names or ("WGPU" + candidate) in class_names:
                                cpp_type = candidate

                        alternatives = [
                            (
                                [f"const std::vector<{cpp_type}>& {vec_name}"],
                                [f"static_cast<{a.type}>({vec_name}.size())", f"reinterpret_cast<const {vec_type} *>({vec_name}.data())"]
                            ),
                            (
                                [f"const std::span<const {cpp_type}>& {vec_name}"],
                                [f"static_cast<{a.type}>({vec_name}.size())", f"reinterpret_cast<const {vec_type} *>({vec_name}.data())"]
                            ),
                            (
                                [f"const {cpp_type}& {vec_name}"],
                                [f"1", f"reinterpret_cast<const {vec_type} *>(&{vec_name})"]
                            ),
                        ]

                        # If raw namespace is used and this element is a handle, also provide overloads
                        # that accept RAII handle types in the wgpu:: namespace (e.g. std::vector<BindGroup>)
                        if args.use_raw_namespace and cpp_type in handle_names:
                            raii = f"{args.namespace}::" + cpp_type
                            alternatives += [
                                (
                                    [f"const std::vector<{raii}>& {vec_name}"],
                                    [f"static_cast<{a.type}>({vec_name}.size())", f"reinterpret_cast<const {vec_type} *>({vec_name}.data())"]
                                ),
                                (
                                    [f"const std::span<const {raii}>& {vec_name}"],
                                    [f"static_cast<{a.type}>({vec_name}.size())", f"reinterpret_cast<const {vec_type} *>({vec_name}.data())"]
                                ),
                                (
                                    [f"const {raii}& {vec_name}"],
                                    [f"1", f"reinterpret_cast<const {vec_type} *>(&{vec_name})"]
                                ),
                            ]

                        for new_args, new_arg_names in alternatives:
                            alt_arguments = arguments[:i-1] + new_args + arguments[i+2:]
                            alt_argument_names = argument_names[:i-1] + new_arg_names + argument_names[i+2:]
                            alt_argument_names_str = ', '.join([argument_self] + alt_argument_names)

                            wrapped_call = f"wgpu{entry_name}{proc.name}({alt_argument_names_str})"

                            maybe_const = " const" if use_const else ""

                            name_and_args = f"{method_name}({', '.join(alt_arguments)}){maybe_const}"
                            decls.append(f"\t{maybe_inline}{return_type} {name_and_args};\n")
                            implems.append(
                                f"{return_type} {entry_name}::{name_and_args} {{\n"
                                + body.replace("{wrapped_call}", wrapped_call)
                                + "}\n"
                            )

            # Add a variant when the last argument is a nullable descriptor
            if len(proc.arguments) > 1:
                arg = proc.arguments[-1]
                _, arg_c, __, ___ = format_arg(arg)
                if arg.nullable and arg_c.startswith("&"):
                    alt_arguments = arguments[:-1]
                    alt_argument_names = argument_names[:-1]
                    alt_argument_names_str = ', '.join([argument_self] + alt_argument_names + ["nullptr"])

                    wrapped_call = f"{begin_cast}wgpu{entry_name}{proc.name}({alt_argument_names_str}){end_cast}"

                    maybe_const = " const" if use_const else ""

                    name_and_args = f"{method_name}({', '.join(alt_arguments)}){maybe_const}"
                    decls.append(f"\t{maybe_inline}{return_type} {name_and_args};\n")
                    implems.append(
                        f"{maybe_inline}{return_type} {entry_name}::{name_and_args} {{\n"
                        + body.replace("{wrapped_call}", wrapped_call)
                        + "}\n"
                    )

        # Check if AddRef exists to generate clone()
        has_add_ref = False
        add_ref_name = "addRef"
        for proc in api.procedures:
            if proc.parent == entry_name and proc.name in ["AddRef", "Reference"]:
                has_add_ref = True
                add_ref_name = proc.name[0].lower() + proc.name[1:]
                break

        if has_add_ref:
            raii_type = get_cpp_name(entry_name, force_raii=args.use_raw_namespace)
            decls.append(f"\t{maybe_inline}{raii_type} clone() const;\n")
            implems.append(
                f"{maybe_inline}{raii_type} {entry_name}::clone() const {{\n"
                f"\tthis->{add_ref_name}();\n"
                f"\treturn {raii_type}(m_raw);\n"
                f"}}\n"
            )

        injected_decls = meta["injected-decls"].members.get(entry_name, [])
        macro = meta["injected-decls"].macro_override.get(entry_name, macro)

        binding[namespace].append(
            f"{macro}({entry_name})\n"
            + "".join(decls + injected_decls)
            + "END\n"
        )

        binding[namespace_oneliner].append(
            f"{macro}({entry_name});"
        )

        if len(defaults_implems) > 0:
            binding["defaults_impl"].append(
                f"// Defaults of {entry_name}\n"
                + "".join(defaults_implems)
                + "\n"
            )

        binding[namespace_impl].append(
            f"// Methods of {entry_name}\n"
            + "".join(implems)
            + "\n"
        )

        if len(template_implems) > 0:
            if entry_type == 'HANDLE':
                binding["handle_template_impl"].append(
                    f"// Template methods of {entry_name}\n"
                    + "".join(template_implems)
                    + "\n"
                )
            elif entry_type == 'CLASS':
                binding["class_template_impl"].append(
                    f"// Template methods of {entry_name}\n"
                    + "".join(template_implems)
                    + "\n"
                )

    if args.use_non_member_procedures:
        for proc in api.procedures:
            if proc.parent is not None:
                continue
            
            arguments = []
            argument_names = []
            for arg in proc.arguments:
                sig, c_expr, _, _ = format_arg(arg)
                arguments.append(sig)
                argument_names.append(c_expr)
                
            proc_name = proc.name[0].lower() + proc.name[1:]
            
            # Return type handling
            ret_type = proc.return_type
            if ret_type.startswith("WGPU_EXPORT "):
                 ret_type = ret_type[12:].strip()
            
            # Just strip WGPU prefix for signature
            ret_sig = ret_type
            if ret_sig.startswith("WGPU"): ret_sig = ret_sig[4:]
            ret_sig = get_cpp_name(ret_sig, force_raii=(ret_sig in handle_names))
            
            binding["procedures"].append(
                f"{ret_sig} {proc_name}({', '.join(arguments)}) {{\n"
                + f"\treturn wgpu{proc.name}({', '.join(argument_names)});\n"
                + "}"
            )
            
            # Add a variant when the last argument is a nullable descriptor
            if len(proc.arguments) > 0:
                arg = proc.arguments[-1]
                _, arg_c, __, ___ = format_arg(arg)
                if arg.nullable and arg_c.startswith("&"):
                    alt_arguments = arguments[:-1]
                    alt_argument_names = argument_names[:-1] + ["nullptr"]
                    
                    binding["procedures"].append(
                        f"{ret_sig} {proc_name}({', '.join(alt_arguments)}) {{\n"
                        + f"\treturn wgpu{proc.name}({', '.join(alt_argument_names)});\n"
                        + "}"
                    )

    for enum in api.enumerations:
        if args.use_scoped_enums:
            if args.use_fake_scoped_enums:
                enum_text = (
                    f"ENUM({enum.name})\n"
                    + "".join([ f"\tENUM_ENTRY({formatEnumValue(e.key)}, {e.value})\n" for e in enum.entries ])
                    + "END"
                )
            else:
                # True enum class
                enum_text = (
                    f"enum class {enum.name}: int {{\n"
                    + "".join([ f"\t{formatEnumValue(e.key)} = {e.value},\n" for e in enum.entries ])
                    + "};\n"
                )
                
                # Operators
                enum_text += f"""
inline constexpr bool operator==({enum.name} a, WGPU{enum.name} b) {{ return static_cast<int>(a) == static_cast<int>(b); }}
inline constexpr bool operator==(WGPU{enum.name} a, {enum.name} b) {{ return static_cast<int>(a) == static_cast<int>(b); }}
inline constexpr bool operator!=({enum.name} a, WGPU{enum.name} b) {{ return !(a == b); }}
inline constexpr bool operator!=(WGPU{enum.name} a, {enum.name} b) {{ return !(a == b); }}

inline constexpr {enum.name} operator|({enum.name} a, {enum.name} b) {{ return static_cast<{enum.name}>(static_cast<int>(a) | static_cast<int>(b)); }}
inline constexpr {enum.name} operator&({enum.name} a, {enum.name} b) {{ return static_cast<{enum.name}>(static_cast<int>(a) & static_cast<int>(b)); }}
"""
                
        else:
            enum_text = f"typedef WGPU{enum.name} {enum.name};"
        binding["enums"].append(enum_text)
        
        # to_string
        decl = f"std::string_view to_string({enum.name} v);"
        binding["to_string_decl"].append(decl)
        
        impl = f"""std::string_view to_string({enum.name} v) {{
    switch (v) {{
"""
        for e in enum.entries:
            key = formatEnumValue(e.key)
            val = f"{enum.name}::{key}" if args.use_scoped_enums else f"WGPU{enum.name}_{key}"
            if args.use_scoped_enums and args.use_fake_scoped_enums:
                 val = f"{enum.name}::{key}"
            impl += f"    case {val}: return \"wgpu::{enum.name}::{key}\";\n"
            
        impl += f"""    default: return "Unknown";
    }}
}}
"""
        binding["to_string_impl"].append(impl)

    # Use a dict to merge duplicates
    cb_names = { cb.name for cb in api.callbacks }
    for cb_name in cb_names:
        binding["callbacks"].append(f"using {cb_name}Callback = WGPU{cb_name}Callback;")

    for ta in api.type_aliases:
        binding["type_aliases"].append(f"using {ta.wgpu_type} = {ta.aliased_type};")

    # Export C entities
    for h in api.handles:
        binding["c_exports"].append(f"export using ::WGPU{h.name};")

    for c in api.classes:
        binding["c_exports"].append(f"export using ::WGPU{c.name};")

    for e in api.enumerations:
        binding["c_exports"].append(f"export using ::WGPU{e.name};")
        for entry in e.entries:
            if e.is_real_enum:
                binding["c_exports"].append(f"export using ::{entry.value};")
            else:
                binding["c_exports"].append(f"export constexpr auto {entry.value} = ::{entry.value}_Internal;")

    for p in api.procedures:
        name = p.name
        if p.parent is not None:
            name = p.parent + name
        binding["c_exports"].append(f"export using ::wgpu{name};")

    for cb in api.callbacks:
        binding["c_exports"].append(f"export using ::WGPU{cb.name}Callback;")

    for ta in api.type_aliases:
        binding["c_exports"].append(f"export using ::WGPU{ta.wgpu_type};")

    # Add FRIENDS macro for RAII handles
    raii_friends = []
    if args.use_raw_namespace:
        ns_prefix = f"{args.namespace}::" if args.namespace else ""
        for h in api.handles:
           raii_friends.append(f"friend class {ns_prefix}raw::{h.name};")
        
        for s in owned_structs:
             raii_friends.append(f"friend struct {ns_prefix}{s};")

        raii_friends.append(f"friend {ns_prefix}Instance createInstance(const {ns_prefix}InstanceDescriptor&);")
        raii_friends.append(f"friend {ns_prefix}Instance createInstance();")
        
        if args.use_non_member_procedures:
            for proc in binding["procedures"]:
                proc_decl = proc.split('{')[0].strip().rstrip(';')
                raii_friends.append(f"friend {proc_decl};")

        macro = "#define WEBGPU_RAII_FRIENDS \\\n\t" + " \\\n\t".join(raii_friends)
        binding["webgpu_includes"].append(macro)
    else:
        binding["webgpu_includes"].append("#define WEBGPU_RAII_FRIENDS")

    for k, v in binding.items():
        binding[k] = "\n".join(v)

    # If raw namespace requested, wrap the non-RAII handle definitions in namespace raw
    if args.use_raw_namespace:
        if binding.get("handles"):
            binding["handles"] = "namespace raw {\n" + binding["handles"] + "\n}\n"
        if binding.get("handles_decl"):
            binding["handles_decl"] += "\nnamespace raw {\n" + binding["handles_decl"] + "\n}\n"
        if binding.get("handles_impl"):
            binding["handles_impl"] = "namespace raw {\n" + binding["handles_impl"] + "\n}\n"
        if binding.get("handle_template_impl"):
            binding["handle_template_impl"] = "namespace raw {\n" + binding["handle_template_impl"] + "\n}\n"
    
    if args.namespace != "":
        for k, v in binding.items():
            if k == "c_exports" or k == "webgpu_includes" or k == "ext_suffix":
                continue
            binding[k] = f"namespace {args.namespace} {{\n" + v + f"\n}} // namespace {args.namespace}\n"

    return binding

# -----------------------------------------------------------------------------
# Utility functions

def loadTemplate(path):
    resolved = resolveFilepath(path)
    logging.info(f"Loading template from {resolved}...")
    with openVfs(resolved, encoding="utf-8") as f:
        in_inject = False
        in_blacklist = False
        injected = ""
        blacklist = ""
        template = ""
        for line in f:
            if line.strip() == "{{begin-inject}}":
                in_inject = True
                continue
            if line.strip() == "{{end-inject}}":
                in_inject = False
                continue
            if line.strip() == "{{begin-blacklist}}":
                in_blacklist = True
                continue
            if line.strip() == "{{end-blacklist}}":
                in_blacklist = False
                continue
            if in_inject:
                injected += line
            elif in_blacklist:
                blacklist += line
            else:
                template += line
        template = (
            template
            .replace('{', '{{') # escape brackets
            .replace('}', '}}')
            .replace('{{{{', '{') # transform double brackets into format string
            .replace('}}}}', '}')
        )
    
    return template, {
        "injected-decls": parseTemplateInjection(injected),
        "blacklist": blacklist,
    }

@dataclass
class InjectedData():
    # Extra members to insert, for each type
    members: Dict[str,List[str]]

    # Replacement for the HANDLE/STRUCT/etc macro used for this definition
    # (used to replace STRUCT by STRUCT_NO_OSTREAM)
    macro_override: Dict[str,str]

def parseTemplateInjection(text):
    it = iter(text.split("\n"))
    
    injected_data = InjectedData(
        members = defaultdict(list),
        macro_override = {},
    )

    begin_re = re.compile(r"^(HANDLE|DESCRIPTOR|STRUCT|STRUCT_NO_OSTREAM)\((\w+)\)")
    end_re = re.compile(r"^END")
    current_category = None

    while (line := next(it, None)) is not None:
        if (match := begin_re.search(line)):
            current_category = match.group(2)
            injected_data.macro_override[current_category] = match.group(1)

        elif (match := end_re.search(line)):
            current_category = None

        elif current_category is not None:
            injected_data.members[current_category].append(line + "\n")

    return injected_data

def downloadHeader(url):
    if url.startswith("https://") or url.startswith("http://"):
        logging.info(f"Downloading webgpu-native header from {url}...")
        import urllib.request
        response = urllib.request.urlopen(url)
        data = response.read()
        text = data.decode('utf-8')
        return text
    else:
        resolved = resolveFilepath(url)
        logging.info(f"Loading webgpu-native header from {resolved}...")
        with openVfs(resolved, encoding="utf-8") as f:
            return f.read()

def generateOutput(args, template, fields):
    logging.info(f"Writing generated binding to {args.output}...")
    fields["webgpu_includes"] += "\n"
    if args.use_raw_namespace:
        fields["webgpu_includes"] = fields["webgpu_includes"] + "\n#define WEBGPU_CPP_USE_RAW_NAMESPACE\n"
    if args.namespace != "":
        fields["webgpu_includes"] = fields["webgpu_includes"] + "\n#define WEBGPU_CPP_NAMESPACE " + args.namespace + "\n"
    out = template.format(**fields)
    if args.use_raw_namespace:
        out += "\n#undef WEBGPU_CPP_USE_RAW_NAMESPACE\n"
    if args.namespace != "":
        out += "\n#undef WEBGPU_CPP_NAMESPACE\n"
    with openVfs(args.output, 'w', encoding="utf-8") as f:
        f.write(out)

def resolveFilepath(path):
    for p in [ join(dirname(__file__), path), path ]:
        if isfileVfs(p):
            return p
    logging.error(f"Invalid template path: {path}")
    raise ValueError("Invalid template path")

def formatEnumValue(value):
    if value[0] in '0123456789':
        return '_' + value
    else:
        return value

def toConstantCase(caml_case):
    naive = ''.join(['_'+c if c.isupper() or c.isnumeric() else c for c in caml_case]).lstrip('_').upper()
    # We then regroup isolated characters together (because they correspond to acronyms):
    current_acronym = None
    tokens = []
    for tok in naive.split("_"):
        if len(tok) == 1:
            if current_acronym is None:
                current_acronym = ""
            current_acronym += tok
        else:
            if current_acronym is not None:
                tokens.append(current_acronym)
            tokens.append(tok)
            current_acronym = None
    if current_acronym is not None:
        tokens.append(current_acronym)
    return "_".join(tokens)

# -----------------------------------------------------------------------------

if __name__ == "__main__":
    args = makeArgParser().parse_args()
    main(args)
    
