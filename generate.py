import argparse
import re
from dataclasses import dataclass, field
from pathlib import Path
from typing import Dict, List, Optional, Tuple


def strip_spaces(value: str) -> str:
	first = value.find(" ")
	if first == -1:
		return value.strip(" ")
	return value.strip(" ")


class Logger:
	def __init__(self, path: Path) -> None:
		self._file = path.open("w", encoding="utf-8")

	def _fmt_value(self, value):
		if isinstance(value, bool):
			return "true" if value else "false"
		return value

	def println(self, fmt: str, *args) -> None:
		if args:
			formatted_args = tuple(self._fmt_value(a) for a in args)
			line = fmt.format(*formatted_args)
		else:
			line = fmt
		self._file.write(line + "\n")
		self._file.flush()


log = Logger(Path("parser.log"))


@dataclass
class InjectedData:
	members: Dict[str, List[str]] = field(default_factory=dict)


@dataclass
class TemplateMeta:
	text: str = ""
	injections: InjectedData = field(default_factory=InjectedData)


@dataclass
class HandleApi:
	name: str


@dataclass
class FieldApi:
	name: str
	type: str
	counter: Optional[str] = None
	is_counter: bool = False
	nullable: bool = False
	is_pointer: bool = False
	is_const: bool = False

	def full_type(self) -> str:
		full = ""
		if self.is_const:
			full += "const "
		full += self.type
		if self.is_pointer:
			full += "*"
		return full


@dataclass
class StructApi:
	name: str
	fields: List[FieldApi] = field(default_factory=list)
	is_descriptor: bool = False
	owning: bool = False


@dataclass
class FuncParamApi:
	name: str
	type: str
	nullable: bool = False
	is_pointer: bool = False
	is_const: bool = False

	def full_type(self) -> str:
		full = self.type
		if self.is_const:
			full += " const"
		if self.is_pointer:
			full += "*"
		return full


@dataclass
class FuncApi:
	name: str
	return_type: str
	nullable: bool = False
	parent: Optional[str] = None
	params: List[FuncParamApi] = field(default_factory=list)


@dataclass
class EnumEntryApi:
	name: str
	value: str


@dataclass
class EnumApi:
	name: str
	flags: bool = False
	entries: List[EnumEntryApi] = field(default_factory=list)


@dataclass
class CallbackApi:
	name: str
	params: List[FuncParamApi] = field(default_factory=list)


@dataclass
class InitMacro:
	macro: str
	type: str


@dataclass
class TypeAliasApi:
	alias: str
	original: str


@dataclass
class WebGpuApi:
	handles: List[HandleApi] = field(default_factory=list)
	structs: List[StructApi] = field(default_factory=list)
	functions: List[FuncApi] = field(default_factory=list)
	enums: List[EnumApi] = field(default_factory=list)
	callbacks: List[CallbackApi] = field(default_factory=list)
	init_macros: List[InitMacro] = field(default_factory=list)
	type_aliases: List[TypeAliasApi] = field(default_factory=list)


@dataclass
class EnumToString:
	enum_name: str
	func_decl: str
	func_impl: str


@dataclass
class FuncParamApiCpp:
	type: str
	name: str
	is_pointer: bool = False
	is_const: bool = False
	is_struct: bool = False
	is_handle: bool = False
	binary_compatible: bool = False
	nullable: bool = False

	def full_type(self, namespace: str) -> str:
		full = self.type
		if self.is_handle and not self.is_pointer:
			full += " const&"
		elif self.is_pointer:
			if self.is_const:
				full += " const"
			full += "*" if self.nullable or not self.type.startswith(namespace + "::") else "&"
		return full

	def get_assign_from_native(self, namespace: str) -> Tuple[str, str]:
		assign = ""
		temp_data = ""
		if self.is_handle:
			assign = (
				f"{'*' if not self.nullable else ''}reinterpret_cast<{self.type}{'' if (self.is_pointer and not self.is_const) else ' const'}*>({'' if self.is_pointer else '&'}{self.name})"
			)
		elif self.is_pointer and self.is_struct:
			if self.nullable:
				if self.binary_compatible:
					assign = f"reinterpret_cast<{self.type}{' const' if self.is_const else ''}*>({self.name})"
				else:
					temp_data = (
						f"\n    {self.type} {self.name}_temp;\n    if ({self.name}) {self.name}_temp = static_cast<{self.type}>(*{self.name});"
					)
					assign = f"{self.name}? &{self.name}_temp : nullptr"
			else:
				if self.binary_compatible:
					assign = f"*reinterpret_cast<{self.type} const*>({self.name})"
				else:
					temp_data = f"\t{self.type} {self.name}_temp = static_cast<{self.type}>(*{self.name});\n"
					assign = f"{self.name}_temp"
		else:
			assign = f"static_cast<{self.type}>({self.name})"
		return assign, temp_data


@dataclass
class CallbackApiCpp:
	name: str
	params: List[FuncParamApiCpp] = field(default_factory=list)
	userdatas: List[str] = field(default_factory=list)

	def gen_param_types(self, namespace: str) -> str:
		return ", ".join(p.full_type(namespace) for p in self.params)

	def gen_param_names(self) -> str:
		return ", ".join(p.name for p in self.params)

	def gen_param_sig(self, namespace: str) -> str:
		return ", ".join(f"{p.full_type(namespace)} {p.name}" for p in self.params)

	def gen_cparam_sig(self, namespace: str) -> str:
		ns = namespace + "::"
		items = []
		for p in self.params:
			type_name = p.type
			if type_name.startswith(ns):
				type_name = type_name[len(ns):]
				if type_name.startswith("raw::"):
					type_name = type_name[5:]
				type_name = "WGPU" + type_name
			if p.is_pointer:
				type_name += " const*"
			items.append(f"{type_name} {p.name}")
		return ", ".join(items)

	def gen_definition(self, namespace: str) -> str:
		userdatas = ", ".join(f"void* {s}" for s in self.userdatas)
		return (
			f"\nstruct {self.name} {{\n"
			f"    struct Control {{\n"
			f"        std::atomic<std::size_t> count{{1}};\n"
			f"        virtual ~Control() = default;\n"
			f"        virtual void invoke({self.gen_param_sig(namespace)}) const = 0;\n"
			f"        virtual void invoke_c({self.gen_cparam_sig(namespace)}) const;\n"
			f"    }};\n"
			f"private:\n"
			f"    template <typename F>\n"
			f"    struct ControlImpl : Control {{\n"
			f"        F func;\n"
			f"        ControlImpl(const F& f) : func(f) {{}}\n"
			f"        void invoke({self.gen_param_sig(namespace)}) const override;\n"
			f"    }};\n"
			f"    Control* data;\n"
			f"public:\n"
			f"    {self.name}() : data(nullptr) {{}}\n"
			f"    {self.name}(WGPU{self.name} native, {userdatas});\n"
			f"    {self.name}(const {self.name}& other) : data(other.data) {{ if (data) ++data->count; }}\n"
			f"    {self.name}({self.name}&& other) noexcept : data(other.data) {{ other.data = nullptr; }}\n"
			f"    {self.name}& operator=(const {self.name}& other);\n"
			f"    {self.name}& operator=({self.name}&& other);\n"
			f"    {self.name}& operator=(std::nullptr_t) {{ reset(); return *this; }}\n"
			f"    ~{self.name}() {{ if (data && --data->count == 0) {{ delete data; }} }}\n"
			f"    template <std::invocable<{self.gen_param_types(namespace)}> F>\n"
			f"    {self.name}(const F& f);\n"
			f"    void operator()({self.gen_param_sig(namespace)}) const;\n"
			f"    void operator()({self.gen_cparam_sig(namespace)}) const;\n"
			f"    void reset() {{ if (data && --data->count == 0) {{ delete data; }} data = nullptr; }}\n"
			f"    operator bool() const {{ return data != nullptr; }}\n"
			f"}};"
		)

	def gen_template_impl(self, namespace: str) -> str:
		return (
			f"template <std::invocable<{self.gen_param_types(namespace)}> F>\n"
			f"{self.name}::{self.name}(const F& f) {{\n"
			f"    data = new ControlImpl<F>(f);\n"
			f"}}\n"
			f"template <typename F>\n"
			f"void {self.name}::ControlImpl<F>::invoke({self.gen_param_sig(namespace)}) const {{\n"
			f"    func({self.gen_param_names()});\n"
			f"}}"
		)

	def gen_impl(self, namespace: str) -> str:
		temp_data = "\n".join(p.get_assign_from_native(namespace)[1] for p in self.params)
		assign = ", ".join(p.get_assign_from_native(namespace)[0] for p in self.params)
		userdata_sig = ", ".join(f"void* {s}" for s in self.userdatas)
		userdata_pass = ", ".join(self.userdatas)
		userdata_members = "\n".join(f"    void* {s};" for s in self.userdatas)
		userdata_init = ", ".join(f"{s}({s})" for s in self.userdatas)
		return (
			f"\nstruct {self.name}ControlNative : {self.name}::Control {{\n"
			f"    WGPU{self.name} native;\n"
			f"{userdata_members}\n"
			f"    {self.name}ControlNative(WGPU{self.name} n, {userdata_sig}) : native(n), {userdata_init} {{}}\n"
			f"    void invoke({self.gen_param_sig(namespace)}) const override {{}}\n"
			f"    void invoke_c({self.gen_cparam_sig(namespace)}) const override;\n"
			f"}};\n"
			f"void {self.name}ControlNative::invoke_c({self.gen_cparam_sig(namespace)}) const {{\n"
			f"    native({self.gen_param_names()}, {userdata_pass});\n"
			f"}}\n"
			f"{self.name}::{self.name}(WGPU{self.name} native, {userdata_sig}) {{\n"
			f"    if (native) {{\n"
			f"        data = new {self.name}ControlNative(native, {userdata_pass});\n"
			f"    }} else {{\n"
			f"        data = nullptr;\n"
			f"    }}\n"
			f"}}\n"
			f"void {self.name}::Control::invoke_c({self.gen_cparam_sig(namespace)}) const {{\n"
			f"{temp_data}\n"
			f"    invoke({assign});\n"
			f"}}\n"
			f"{self.name}& {self.name}::operator=(const {self.name}& other) {{\n"
			f"    if (this != &other) {{\n"
			f"        if (data && --data->count == 0) {{ delete data; }}\n"
			f"        data = other.data;\n"
			f"        if (data) ++data->count;\n"
			f"    }}\n"
			f"    return *this;\n"
			f"}}\n"
			f"{self.name}& {self.name}::operator=({self.name}&& other) {{\n"
			f"    if (this != &other) {{\n"
			f"        if (data && --data->count == 0) {{ delete data; }}\n"
			f"        data = other.data;\n"
			f"        other.data = nullptr;\n"
			f"    }}\n"
			f"    return *this;\n"
			f"}}\n"
			f"void {self.name}::operator()({self.gen_param_sig(namespace)}) const {{ if (data) data->invoke({self.gen_param_names()}); }}\n"
			f"void {self.name}::operator()({self.gen_cparam_sig(namespace)}) const {{ if (data) data->invoke_c({self.gen_param_names()}); }}\n"
		)


NEXT_IN_CHAIN_HELPER = """
struct NextInChainBase {
    std::atomic<std::size_t> ref_count{1};
    virtual WGPUChainedStruct* getNextInChain() const = 0;
    virtual ~NextInChainBase() = default;
};
template <typename Struct>
struct NextInChainImpl : Struct, NextInChainBase {
    mutable typename Struct::CStruct cstruct;
    NextInChainImpl(const Struct& s) : Struct(s) {
        cstruct = this->to_cstruct();
    }
    NextInChainImpl(Struct&& s) : Struct(std::move(s)) {
        cstruct = this->to_cstruct();
    }
    WGPUChainedStruct* getNextInChain() const override {
        cstruct = this->to_cstruct();
        return reinterpret_cast<WGPUChainedStruct*>(&cstruct);
    }
    void updateFromCStruct() {
        (Struct&)(*this) = static_cast<Struct>(cstruct);
    }
};
struct NextInChainNative : NextInChainBase {
    WGPUChainedStruct* next;
    NextInChainNative(WGPUChainedStruct* n) : next(n) {}
    WGPUChainedStruct* getNextInChain() const override { return next; }
};
struct NextInChain {
public:
    NextInChain() : data(nullptr) {}
    NextInChain(const NextInChain& other) : data(other.data) { if (data) ++data->ref_count; }
    NextInChain(NextInChain&& other) noexcept : data(other.data) { other.data = nullptr; }
    NextInChain& operator=(const NextInChain& other) {
        if (this != &other) {
            if (data && --data->ref_count == 0) {
                delete data;
            }
            data = other.data;
            if (data) ++data->ref_count;
        }
        return *this;
    }
    NextInChain& operator=(NextInChain&& other) noexcept {
        if (this != &other) {
            if (data && --data->ref_count == 0) {
                delete data;
            }
            data = other.data;
            other.data = nullptr;
        }
        return *this;
    }
    ~NextInChain() { reset(); }
    template <typename Struct>
    void setNext(Struct&& s) {
        reset();
        data = new NextInChainImpl<std::decay_t<Struct>>(std::forward<Struct>(s));
    }
    void setNext(WGPUChainedStruct* next) {
        reset();
        data = new NextInChainNative(next);
    }
    WGPUChainedStruct* getNext() const {
        if (data) {
            return data->getNextInChain();
        }
        return nullptr;
    }
    template <typename Struct>
    Struct* getAs() const {
        if (data) {
            if (auto impl = dynamic_cast<NextInChainImpl<Struct>*>(data)) {
                impl->updateFromCStruct();
                return static_cast<Struct*>(impl);
            }
        }
        return nullptr;
    }
    void reset() {
        if (data && --data->ref_count == 0) {
            delete data;
        }
        data = nullptr;
    }
private:
    NextInChainBase* data;
};"""


@dataclass
class StructFieldCpp:
	type: str
	name: str
	assign_from_native: str
	assign_to_cstruct: str


@dataclass
class StructApiCpp:
	name: str
	binary_compatible: bool = True
	init_macro: str = ""
	fields: List[StructFieldCpp] = field(default_factory=list)
	methods_decl: List[str] = field(default_factory=list)
	methods_template_impl: List[str] = field(default_factory=list)
	methods_impl: List[str] = field(default_factory=list)
	extra_cstruct_members: List[str] = field(default_factory=list)

	def gen_definition(self) -> str:
		extra_members = "\n        ".join(self.extra_cstruct_members)
		methods_decl = "\n    ".join(self.methods_decl)
		fields_decl = "\n    ".join(f"{f.type} {f.name};" for f in self.fields)
		init = "" if not self.init_macro else f" : {self.name}({self.init_macro})"
		return (
			f"\nstruct {self.name} {{\n"
			f"    struct CStruct : public WGPU{self.name} {{\n"
			f"        {extra_members}\n"
			f"    }};\n"
			f"    {self.name}(const WGPU{self.name}& native);\n"
			f"    {self.name}(){init} {{}};\n"
			f"    CStruct to_cstruct() const;\n"
			f"    {methods_decl}\n"
			f"    {fields_decl}\n"
			f"}};"
		)

	def gen_template_impl(self) -> str:
		return "\n".join(self.methods_template_impl)

	def gen_impl(self) -> str:
		from_native = "\n".join(f.assign_from_native for f in self.fields)
		to_cstruct = "\n".join(f.assign_to_cstruct for f in self.fields)
		methods = "\n".join(self.methods_impl)
		return (
			f"\n{self.name}::{self.name}(const WGPU{self.name}& native) {{\n"
			f"{from_native}\n"
			f"}}\n"
			f"\n{self.name}::CStruct {self.name}::to_cstruct() const {{\n"
			f"    CStruct cstruct;\n"
			f"{to_cstruct}\n"
			f"    return cstruct;\n"
			f"}}\n"
			f"\n{methods}"
		)


@dataclass
class HandleApiCpp:
	name: str
	methods_decl: List[str] = field(default_factory=list)
	methods_template_impl: List[str] = field(default_factory=list)
	methods_impl: List[str] = field(default_factory=list)

	def gen_raii_definition(self) -> str:
		return (
			f"\nclass {self.name} : public raw::{self.name} {{\n"
			f"public:\n"
			f"    using base_type = raw::{self.name};\n"
			f"    using wgpu_type = WGPU{self.name};\n"
			f"    {self.name}() : base_type() {{}}\n"
			f"    WEBGPU_RAII_FRIENDS\n"
			f"private:\n"
			f"    {self.name}(wgpu_type raw) : base_type(raw) {{}}\n"
			f"    {self.name}(base_type raw) : base_type(raw) {{}}\n"
			f"    {self.name}& operator=(const base_type& raw) {{ if (*this) this->release(); base_type::operator=(raw); return *this; }}\n"
			f"public:\n"
			f"    {self.name}& operator=(std::nullptr_t) {{ if (*this) this->release(); base_type::operator=(nullptr); return *this; }}\n"
			f"    {self.name}(const {self.name}& other) : base_type(other) {{ if (*this) this->addRef(); }}\n"
			f"    {self.name}({self.name}&& other) : base_type(other) {{ (base_type&)(other) = nullptr; }}\n"
			f"    {self.name}& operator=(const {self.name}& other) {{ if (this != &other) {{ if (*this) this->release(); base_type::operator=(other); if (*this) this->addRef(); }} return *this; }}\n"
			f"    {self.name}& operator=({self.name}&& other) {{ if (this != &other) {{ if (*this) this->release(); base_type::operator=(other); (base_type&)(other) = nullptr; }} return *this; }}\n"
			f"    ~{self.name}() {{ if (*this) this->release(); }}\n"
			f"    operator bool() const {{ return base_type::operator bool(); }}\n"
			f"    bool operator==(const {self.name}& other) const {{ return base_type::operator==(other); }}\n"
			f"    bool operator!=(const {self.name}& other) const {{ return base_type::operator!=(other); }}\n"
			f"    {self.name} clone() const {{ this->addRef(); return {self.name}((const base_type&)*this); }}\n"
			f"}};"
		)

	def gen_definition(self) -> str:
		methods_decl = "\n".join(self.methods_decl)
		return (
			f"\nclass {self.name} {{\n"
			f"public:\n"
			f"    using wgpu_type = WGPU{self.name};\n"
			f"    {self.name}() : m_raw(nullptr) {{}}\n"
			f"    {self.name}(WGPU{self.name} raw) : m_raw(raw) {{}}\n"
			f"    operator WGPU{self.name}() const {{ return m_raw; }}\n"
			f"    operator bool() const {{ return m_raw != nullptr; }}\n"
			f"    bool operator==(const {self.name}& other) const {{ return m_raw == other.m_raw; }}\n"
			f"    bool operator!=(const {self.name}& other) const {{ return m_raw != other.m_raw; }}\n"
			f"    {self.name}& operator=(std::nullptr_t) {{ m_raw = nullptr; return *this; }}\n"
			f"{methods_decl}\n"
			f"private:\n"
			f"    WGPU{self.name} m_raw;\n"
			f"}};"
		)

	def gen_template_impl(self) -> str:
		return "\n".join(self.methods_template_impl)

	def gen_impl(self) -> str:
		return "\n".join(self.methods_impl)


@dataclass
class FuncApiCpp:
	name: str
	func_decl: str
	func_template_impl: str
	func_impl: str


@dataclass
class WebGpuApiCpp:
	type_aliases: List[TypeAliasApi] = field(default_factory=list)
	enums: List[EnumApi] = field(default_factory=list)
	enum_to_string: List[EnumToString] = field(default_factory=list)
	structs: List[StructApiCpp] = field(default_factory=list)
	callbacks: List[CallbackApiCpp] = field(default_factory=list)
	handles: List[HandleApiCpp] = field(default_factory=list)
	functions: List[FuncApiCpp] = field(default_factory=list)


def parse_params(params_str: str) -> List[FuncParamApi]:
	if not params_str or params_str.strip() == "void":
		return []
	params = [p.strip() for p in params_str.split(",")]
	result: List[FuncParamApi] = []
	for param in params:
		if not param:
			continue
		nullable = False
		if param.startswith("WGPU_NULLABLE"):
			nullable = True
			param = param[len("WGPU_NULLABLE"):].lstrip(" ")
		tokens = [t for t in param.split(" ") if t]
		name = tokens[-1]
		type_tokens = tokens[:-1]
		typ = " ".join(type_tokens)
		is_pointer = False
		is_const = False
		if "*" in typ:
			is_pointer = True
			typ = typ.replace("*", "", 1)
		if "const " in typ or " const" in typ:
			is_const = True
			typ = typ.replace("const", "", 1)
		if "struct " in typ or " struct" in typ:
			typ = typ.replace("struct", "", 1)
		typ = typ.strip()
		log.println(
			"  Param: name: {}, type: {}, nullable: {}, is_pointer: {}, is_const: {}",
			name,
			typ,
			nullable,
			is_pointer,
			is_const,
		)
		result.append(FuncParamApi(name=name, type=typ, nullable=nullable, is_pointer=is_pointer, is_const=is_const))
	return result


def parse_func(name: str, return_type: str, params_str: str) -> FuncApi:
	nullable = False
	if return_type.startswith("WGPU_NULLABLE"):
		return_type = return_type[len("WGPU_NULLABLE"):].strip()
		nullable = True
	log.println("  Return type: {}, nullable: {}", return_type, nullable)
	params = parse_params(params_str)
	return FuncApi(name=name, return_type=return_type, nullable=nullable, params=params)


def parse_enum(name: str, lines: List[str], start_index: int) -> Tuple[EnumApi, int]:
	end_re = re.compile(r".*\}")
	entry_re = re.compile(r"^\s+WGPU([^_]+)_([\w_]+) *= *([^,]+),?")
	api = EnumApi(name=name)
	i = start_index
	while i < len(lines) and not end_re.search(lines[i]):
		match = entry_re.search(lines[i])
		if match:
			prefix = match.group(1)
			entry_name = match.group(2)
			value = f"WGPU{prefix}_{entry_name}"
			api.entries.append(EnumEntryApi(entry_name, value))
		i += 1
	for entry in api.entries:
		log.println("  Entry: name: {}, value: {}", entry.name, entry.value)
	return api, i


def parse_struct(name: str, lines: List[str], start_index: int) -> Tuple[StructApi, int]:
	end_re = re.compile(r".*\}")
	field_re = re.compile(r"^\s*(.+) (\w+);$")
	api = StructApi(name=name)
	count_fields: List[int] = []
	i = start_index
	while i < len(lines) and not end_re.search(lines[i]):
		match = field_re.search(lines[i])
		if match:
			field_type = match.group(1)
			field_name = match.group(2)
			nullable = False
			is_pointer = False
			is_const = False
			if field_type.startswith("WGPU_NULLABLE"):
				nullable = True
				field_type = field_type[len("WGPU_NULLABLE"):]
			if "*" in field_type:
				is_pointer = True
				field_type = field_type.replace("*", "", 1)
			if "const " in field_type or " const" in field_type:
				is_const = True
				field_type = field_type.replace("const", "", 1)
			field_type = field_type.strip()
			if field_name == "nextInChain":
				api.is_descriptor = True
			if field_name.endswith("Count"):
				count_fields.append(len(api.fields))
			api.fields.append(
				FieldApi(
					name=field_name,
					type=field_type,
					nullable=nullable,
					is_pointer=is_pointer,
					is_const=is_const,
				)
			)
		i += 1
	for count_index in count_fields:
		array_field_prefix = api.fields[count_index].name[:-6]
		for field in api.fields:
			if field.name.startswith(array_field_prefix) and field.name != api.fields[count_index].name and field.is_pointer:
				field.counter = api.fields[count_index].name
				api.fields[count_index].is_counter = True
				break
	for field in api.fields:
		log.println(
			"  Field: type: {}, name: {}, nullable: {}, counter: {}, is_counter: {}, is_pointer: {}, is_const: {}",
			field.type,
			field.name,
			field.nullable,
			field.counter if field.counter else "none",
			field.is_counter,
			field.is_pointer,
			field.is_const,
		)
	return api, i


def parse_header(api: WebGpuApi, header_content: str) -> None:
	lines = header_content.split("\n")
	struct_re = re.compile(r"typedef +struct +WGPU(\w+) *\{")
	handle_re = re.compile(r"typedef +struct .*\* *WGPU(\w+)\s+WGPU_OBJECT_ATTRIBUTE;")
	func_re = re.compile(r"WGPU_EXPORT +([\w *]+) +wgpu(\w+)\s*\(([^)]*)\)\s+WGPU_FUNCTION_ATTRIBUTE;")
	enum_re = re.compile(r"typedef +enum +WGPU(\w+) *\{")
	flag_enum_re = re.compile(r"typedef +WGPUFlags +WGPU(\w+)\s*;")
	flat_value_re = re.compile(r"static +const +WGPU(\w+) +WGPU(\w+)_(\w+) *= *(\w+)( /\*(.*)\*/)?;")
	callback_re = re.compile(r"typedef +void +\(\*WGPU(\w+)Callback\)\((.*)\)\s*WGPU_FUNCTION_ATTRIBUTE;")
	init_macro_re = re.compile(r"#define +(WGPU_[A-Z0-9_]+_INIT)")
	typedef_re = re.compile(r"typedef +(\w+) +WGPU(\w+)\s*;")

	i = 0
	while i < len(lines):
		line = lines[i]
		match = typedef_re.search(line)
		if match:
			original_type = match.group(1)
			alias_name = match.group(2)
			log.println("Parsing typedef: {} -> {}", alias_name, original_type)
			api.type_aliases.append(TypeAliasApi(alias=alias_name, original=original_type))
			i += 1
			continue
		match = struct_re.search(line)
		if match:
			struct_name = match.group(1)
			log.println("Parsing struct: {}", struct_name)
			struct_api, end_index = parse_struct(struct_name, lines, i + 1)
			api.structs.append(struct_api)
			i = end_index + 1
			continue
		match = handle_re.search(line)
		if match:
			handle_name = match.group(1)
			log.println("Parsing handle: {}", handle_name)
			api.handles.append(HandleApi(handle_name))
			i += 1
			continue
		match = func_re.search(line)
		if match:
			return_type = match.group(1)
			func_name = match.group(2)
			params_str = match.group(3)
			log.println("Parsing function: {}", func_name)
			api.functions.append(parse_func(func_name, return_type, params_str))
			i += 1
			continue
		match = enum_re.search(line)
		if match:
			enum_name = match.group(1)
			log.println("Parsing enum: {}", enum_name)
			enum_api, end_index = parse_enum(enum_name, lines, i + 1)
			api.enums.append(enum_api)
			i = end_index + 1
			continue
		match = flag_enum_re.search(line)
		if match:
			enum_name = match.group(1)
			log.println("Parsing flag enum alias: {}", enum_name)
			api.enums.append(EnumApi(name=enum_name, flags=True, entries=[]))
			i += 1
			continue
		match = flat_value_re.search(line)
		if match:
			enum_name = match.group(1)
			enum_name2 = match.group(2)
			if enum_name != enum_name2:
				raise AssertionError("enum name mismatch")
			entry_name = match.group(3)
			entry_value = f"WGPU{enum_name}_{entry_name}"
			log.println(
				"Parsing flag value: enum: {}, entry: {}, value: {}",
				enum_name,
				entry_name,
				entry_value,
			)
			found = False
			for enum_api in api.enums:
				if enum_api.name == enum_name:
					enum_api.entries.append(EnumEntryApi(entry_name, entry_value))
					found = True
					break
			if not found:
				api.enums.append(EnumApi(name=enum_name, flags=True, entries=[EnumEntryApi(entry_name, entry_value)]))
			i += 1
			continue
		match = callback_re.search(line)
		if match:
			log.println("Parsing callback: {}", match.group(1))
			params = parse_params(match.group(2))
			api.callbacks.append(CallbackApi(name=match.group(1), params=params))
			i += 1
			continue
		match = init_macro_re.search(line)
		if match:
			macro = match.group(1)
			first = macro.find("_")
			last = macro.rfind("_")
			type_name = macro[first + 1:last]
			parts = type_name.split("_")
			type_name = "".join(p.lower().capitalize() for p in parts)
			log.println("Found init macro: macro: {}, type: {}", macro, type_name)
			api.init_macros.append(InitMacro(macro=macro, type=type_name))
			i += 1
			continue
		i += 1

	for func_api in api.functions:
		max_len = 0
		for handle_api in api.handles:
			if func_api.name.startswith(handle_api.name) and len(handle_api.name) > max_len:
				max_len = len(handle_api.name)
				func_api.parent = handle_api.name
		for struct_api in api.structs:
			if func_api.name.startswith(struct_api.name) and len(struct_api.name) > max_len:
				max_len = len(struct_api.name)
				func_api.parent = None
		if func_api.parent:
			log.println("Function {} is a method of {}", func_api.name, func_api.parent)
		else:
			log.println("Function {} is a non-member function", func_api.name)

	for func_api in api.functions:
		if func_api.return_type.startswith("WGPU"):
			possible_struct_name = func_api.return_type[4:]
			for struct_api in api.structs:
				if struct_api.name == possible_struct_name:
					struct_api.owning = True
		for param in func_api.params:
			if param.type.startswith("WGPU") and param.is_pointer and not param.is_const:
				possible_struct_name = param.type[4:]
				for struct_api in api.structs:
					if struct_api.name == possible_struct_name:
						struct_api.owning = True


def load_template(template_path: Path) -> TemplateMeta:
	if not template_path.exists():
		raise RuntimeError(f"Failed to open template file: {template_path}")
	meta = TemplateMeta()
	begin_inject_re = re.compile(r"\{\{begin_inject\}\}")
	end_inject_re = re.compile(r"\{\{end_inject\}\}")
	inject_typename_re = re.compile(r"\s*typename +(\w+):")
	current_type = ""
	for line in template_path.read_text(encoding="utf-8").splitlines():
		if begin_inject_re.search(line):
			current_type = ""
		elif end_inject_re.search(line):
			current_type = ""
		else:
			match = inject_typename_re.search(line)
			if match:
				current_type = match.group(1)
				if current_type not in meta.injections.members:
					meta.injections.members[current_type] = []
			else:
				if current_type:
					meta.injections.members.setdefault(current_type, []).append(line)
				else:
					meta.text += line + "\n"
	log.println("Loaded template:\ntext:\n{}\n-----------\ninjections:\n{}\n\n", meta.text, meta.injections.members)
	return meta


def produce_webgpu_cpp(api: WebGpuApi, template_meta: TemplateMeta, args: argparse.Namespace) -> WebGpuApiCpp:
	result = WebGpuApiCpp()
	namespace = args.namespace
	use_raii = args.use_raii
	force_raii = args.force_raii

	for type_alias in api.type_aliases:
		result.type_aliases.append(type_alias)

	for enum_api in api.enums:
		entries = [EnumEntryApi(name=f"e{e.name}", value=e.value) for e in enum_api.entries]
		new_enum = EnumApi(name=enum_api.name, flags=enum_api.flags, entries=entries)
		result.enums.append(new_enum)
		func_decl = f"std::string_view to_string({enum_api.name} value);"
		impl_lines = [f"std::string_view to_string({enum_api.name} value) {{\n", "    switch (value) {\n"]
		for entry in entries:
			impl_lines.append(f"        case {enum_api.name}::{entry.name}: return \"{entry.name[1:]}\";\n")
		impl_lines.append("        default: return \"Unknown\";\n")
		impl_lines.append("    }\n")
		impl_lines.append("}\n")
		result.enum_to_string.append(EnumToString(enum_name=enum_api.name, func_decl=func_decl, func_impl="".join(impl_lines)))

	for handle_api in api.handles:
		result.handles.append(HandleApiCpp(name=handle_api.name))

	def get_func_param_cpp_type(param: FuncParamApi) -> FuncParamApiCpp:
		type_without_wgpu = param.type
		if type_without_wgpu.startswith("WGPU"):
			type_without_wgpu = type_without_wgpu[4:]
		if param.type.startswith("WGPU"):
			cpp_type = namespace + "::" + param.type[4:]
		else:
			cpp_type = param.type
		param_cpp = FuncParamApiCpp(
			type=cpp_type,
			name=param.name,
			is_pointer=param.is_pointer,
			nullable=param.nullable,
			is_const=param.is_const,
		)
		if param.type.startswith("WGPU") and any(h.name == type_without_wgpu for h in api.handles):
			param_cpp.is_handle = True
		if param.type.startswith("WGPU") and any(s.name == type_without_wgpu for s in api.structs):
			param_cpp.is_struct = True
		return param_cpp

	for callback_api in api.callbacks:
		callback_cpp = CallbackApiCpp(name=callback_api.name + "Callback")
		for param in [p for p in callback_api.params if not p.name.startswith("userdata")]:
			callback_cpp.params.append(get_func_param_cpp_type(param))
		for param in [p for p in callback_api.params if p.name.startswith("userdata")]:
			callback_cpp.userdatas.append(param.name)
		result.callbacks.append(callback_cpp)

	def get_cpp_field_type(typ: str, owning: bool) -> str:
		if typ.startswith("WGPU"):
			typ = typ[4:]
			if use_raii and not force_raii and not owning and any(h.name == typ for h in api.handles):
				return namespace + "::raw::" + typ
			return namespace + "::" + typ
		return typ

	struct_cpp_map: Dict[str, StructApiCpp] = {}

	def get_struct_api_cpp(name: str, pstruct_api: Optional[StructApi] = None) -> StructApiCpp:
		if name in struct_cpp_map:
			return struct_cpp_map[name]
		if pstruct_api is None:
			pstruct_api = next(s for s in api.structs if s.name == name)
		struct_api = pstruct_api
		struct_cpp = StructApiCpp(name=struct_api.name)

		for init_macro in api.init_macros:
			if struct_api.name == init_macro.type:
				struct_cpp.init_macro = init_macro.macro
				break

		for field in struct_api.fields:
			if field.is_counter:
				continue
			if field.counter:
				struct_cpp.binary_compatible = False
				counter_type = next(f.type for f in struct_api.fields if f.name == field.counter)
				cpp_type = get_cpp_field_type(field.type, struct_api.owning)
				field_cpp = StructFieldCpp(
					type=f"std::vector<{cpp_type}>",
					name=field.name,
					assign_from_native=(
						f"\n    this->{field.name} = std::span(native.{field.name}, native.{field.counter}) | std::views::transform([](auto&& e) {{ return static_cast<{cpp_type}>(e); }}) | std::ranges::to<std::vector<{cpp_type}>>();"
					),
					assign_to_cstruct="",
				)
				type_without_wgpu = field.type
				if type_without_wgpu.startswith("WGPU"):
					type_without_wgpu = type_without_wgpu[4:]
				if (
					field_cpp.type == field.type
					or any(e.name == type_without_wgpu for e in api.enums)
					or any(h.name == type_without_wgpu for h in api.handles)
					or get_struct_api_cpp(field.type[4:]).binary_compatible
				):
					field_cpp.assign_to_cstruct = (
						f"\n    cstruct.{field.name} = reinterpret_cast<{field.full_type()}>(this->{field.name}.data());\n"
						f"    cstruct.{field.counter} = static_cast<{counter_type}>(this->{field.name}.size());"
					)
				elif not get_struct_api_cpp(field.type[4:]).extra_cstruct_members:
					struct_cpp.extra_cstruct_members.append(f"std::vector<{field.type}> {field.name}_vec;")
					field_cpp.assign_to_cstruct = (
						f"\n    cstruct.{field.name}_vec = this->{field.name} | std::views::transform([](auto&& e) {{ return static_cast<{field.type}>(e.to_cstruct()); }}) | std::ranges::to<std::vector<{field.type}>>();\n"
						f"    cstruct.{field.name} = cstruct.{field.name}_vec.data();\n"
						f"    cstruct.{field.counter} = static_cast<{counter_type}>(cstruct.{field.name}_vec.size());"
					)
				else:
					struct_cpp.extra_cstruct_members.append(f"std::vector<{cpp_type}::CStruct> {field.name}_cstruct_vec;")
					struct_cpp.extra_cstruct_members.append(f"std::vector<{field.type}> {field.name}_vec;")
					field_cpp.assign_to_cstruct = (
						f"\n    cstruct.{field.name}_cstruct_vec = this->{field.name} | std::views::transform([](auto&& e) {{ return e.to_cstruct(); }}) | std::ranges::to<std::vector<{cpp_type}::CStruct>>();\n"
						f"    cstruct.{field.name}_vec = cstruct.{field.name}_cstruct_vec | std::views::transform([](auto&& e) {{ return static_cast<{field.type}>(e); }}) | std::ranges::to<std::vector<{field.type}>>();\n"
						f"    cstruct.{field.name} = cstruct.{field.name}_vec.data();\n"
						f"    cstruct.{field.counter} = static_cast<{counter_type}>(cstruct.{field.name}_vec.size());"
					)

				setter_name = "set" + field.name[:1].upper() + field.name[1:]
				struct_cpp.methods_decl.append(
					f"\n    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, {cpp_type}>\n    {struct_cpp.name}& {setter_name}(T&& values) &;"
				)
				struct_cpp.methods_template_impl.append(
					f"\ntemplate <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, {cpp_type}>\n{struct_cpp.name}& {struct_cpp.name}::{setter_name}(T&& values) & {{\n    this->{field.name} = values | std::views::transform([](auto&& e) {{ return static_cast<{cpp_type}>(e); }}) | std::ranges::to<std::vector<{cpp_type}>>();\n    return *this;\n}}"
				)
				struct_cpp.methods_decl.append(
					f"\n    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, {cpp_type}>\n    {struct_cpp.name}&& {setter_name}(T&& values) &&;"
				)
				struct_cpp.methods_template_impl.append(
					f"\ntemplate <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, {cpp_type}>\n{struct_cpp.name}&& {struct_cpp.name}::{setter_name}(T&& values) && {{\n    this->{field.name} = values | std::views::transform([](auto&& e) {{ return static_cast<{cpp_type}>(e); }}) | std::ranges::to<std::vector<{cpp_type}>>();\n    return std::move(*this);\n}}"
				)
				struct_cpp.fields.append(field_cpp)
				continue

			if field.is_pointer and field.type == "void" and field.name.startswith("userdata") and struct_cpp.name.endswith("CallbackInfo"):
				struct_cpp.binary_compatible = False
				continue

			if struct_cpp.name.endswith("CallbackInfo") and field.name == "callback":
				struct_cpp.binary_compatible = False
				cpp_type = get_cpp_field_type(field.type, struct_api.owning)
				callback_api = next(c for c in api.callbacks if c.name + "Callback" == field.type[4:])
				field_cpp = StructFieldCpp(
					type=cpp_type,
					name=field.name,
					assign_from_native=(
						f"\n    if (native.{field.name} != nullptr) {{\n        this->{field.name} = {cpp_type}(native.{field.name}, {', '.join('native.' + f.name for f in struct_api.fields if f.name.startswith('userdata'))});\n    }}"
					),
					assign_to_cstruct=(
						f"\n    if (this->{field.name}) {{\n        cstruct.{field.name} = []({', '.join(p.full_type() + ' ' + p.name for p in callback_api.params)}) {{\n            auto callback = std::move(*reinterpret_cast<{cpp_type}*>({next(p.name for p in callback_api.params if p.name.startswith('userdata'))}));\n            callback({', '.join(p.name for p in callback_api.params if not p.name.startswith('userdata'))});\n        }};\n        new (cstruct.{next(p.name for p in callback_api.params if p.name.startswith('userdata'))}) {cpp_type}(this->{field.name});\n    }} else {{\n        cstruct.{field.name} = nullptr;\n    }}"
					),
				)
				setter_name = "set" + field.name[:1].upper() + field.name[1:]
				struct_cpp.methods_decl.append(f"\n    {struct_cpp.name}& {setter_name}({cpp_type} value) &;")
				struct_cpp.methods_impl.append(
					f"\n{struct_cpp.name}& {struct_cpp.name}::{setter_name}({cpp_type} value) & {{\n    this->{field.name} = std::move(value);\n    return *this;\n}}"
				)
				struct_cpp.methods_decl.append(f"\n    {struct_cpp.name}&& {setter_name}({cpp_type} value) &&;")
				struct_cpp.methods_impl.append(
					f"\n{struct_cpp.name}&& {struct_cpp.name}::{setter_name}({cpp_type} value) && {{\n    this->{field.name} = std::move(value);\n    return std::move(*this);\n}}"
				)
				struct_cpp.fields.append(field_cpp)
				continue

			if field.is_pointer and field.nullable and field.type.startswith("WGPU") and any(s.name == field.type[4:] for s in api.structs):
				struct_cpp.binary_compatible = False
				cpp_type = get_cpp_field_type(field.type, struct_api.owning)
				field_cpp = StructFieldCpp(
					type=f"std::optional<{cpp_type}>",
					name=field.name,
					assign_from_native=(
						f"\n    if (native.{field.name} != nullptr) {{\n        this->{field.name} = static_cast<{cpp_type}>(*(native.{field.name}));\n    }} else {{\n        this->{field.name}.reset();\n    }}"
					),
					assign_to_cstruct="",
				)
				if (
					field_cpp.type == field.type
					or any(e.name == field.type[4:] for e in api.enums)
					or any(h.name == field.type[4:] for h in api.handles)
					or get_struct_api_cpp(field.type[4:]).binary_compatible
				):
					field_cpp.assign_to_cstruct = (
						f"\n    if (this->{field.name}.has_value()) {{\n        cstruct.{field.name} = reinterpret_cast<{field.full_type()}>(&(*(this->{field.name})));\n    }} else {{\n        cstruct.{field.name} = nullptr;\n    }}"
					)
				else:
					struct_cpp.extra_cstruct_members.append(f"{cpp_type}::CStruct {field.name}_cstruct;")
					field_cpp.assign_to_cstruct = (
						f"\n    if (this->{field.name}.has_value()) {{\n        cstruct.{field.name}_cstruct = this->{field.name}->to_cstruct();\n        cstruct.{field.name} = &(cstruct.{field.name}_cstruct);\n    }} else {{\n        cstruct.{field.name} = nullptr;\n    }}"
					)
				setter_name = "set" + field.name[:1].upper() + field.name[1:]
				struct_cpp.methods_decl.append(f"\n    {struct_cpp.name}& {setter_name}(const {cpp_type}& value) &;")
				struct_cpp.methods_impl.append(
					f"\n{struct_cpp.name}& {struct_cpp.name}::{setter_name}(const {cpp_type}& value) & {{\n    this->{field.name}.emplace(value);\n    return *this;\n}}"
				)
				struct_cpp.methods_decl.append(f"\n    {struct_cpp.name}&& {setter_name}(const {cpp_type}& value) &&;")
				struct_cpp.methods_impl.append(
					f"\n{struct_cpp.name}&& {struct_cpp.name}::{setter_name}(const {cpp_type}& value) && {{\n    this->{field.name}.emplace(value);\n    return std::move(*this);\n}}"
				)
				struct_cpp.methods_decl.append(f"\n    {struct_cpp.name}& {setter_name}({cpp_type}&& value) &;")
				struct_cpp.methods_impl.append(
					f"\n{struct_cpp.name}& {struct_cpp.name}::{setter_name}({cpp_type}&& value) & {{\n    this->{field.name}.emplace(std::move(value));\n    return *this;\n}}"
				)
				struct_cpp.methods_decl.append(f"\n    {struct_cpp.name}&& {setter_name}({cpp_type}&& value) &&;")
				struct_cpp.methods_impl.append(
					f"\n{struct_cpp.name}&& {struct_cpp.name}::{setter_name}({cpp_type}&& value) && {{\n    this->{field.name}.emplace(std::move(value));\n    return std::move(*this);\n}}"
				)
				struct_cpp.fields.append(field_cpp)
				continue

			if field.is_pointer and (field.name == "nextInChain" or field.name == "next"):
				struct_cpp.binary_compatible = False
				field_cpp = StructFieldCpp(
					type="NextInChain",
					name=field.name,
					assign_from_native=f"\n    this->{field.name}.setNext(native.{field.name});",
					assign_to_cstruct=f"\n    cstruct.{field.name} = this->{field.name}.getNext();",
				)
				setter_name = "set" + field.name[:1].upper() + field.name[1:]
				struct_cpp.methods_decl.append(f"\n    template <typename T>\n    {struct_cpp.name}& {setter_name}(T&& value) &;")
				struct_cpp.methods_template_impl.append(
					f"\ntemplate <typename T>\n{struct_cpp.name}& {struct_cpp.name}::{setter_name}(T&& value) & {{\n    this->{field.name}.setNext(std::forward<T>(value));\n    return *this;\n}}"
				)
				struct_cpp.methods_decl.append(f"\n    template <typename T>\n    {struct_cpp.name}&& {setter_name}(T&& value) &&;")
				struct_cpp.methods_template_impl.append(
					f"\ntemplate <typename T>\n{struct_cpp.name}&& {struct_cpp.name}::{setter_name}(T&& value) && {{\n    this->{field.name}.setNext(std::forward<T>(value));\n    return std::move(*this);\n}}"
				)
				struct_cpp.fields.append(field_cpp)
				continue

			cpp_type = get_cpp_field_type(field.type, struct_api.owning)
			if field.is_const:
				cpp_type += " const"
			if field.is_pointer:
				cpp_type += "*"
			field_cpp = StructFieldCpp(
				type=cpp_type,
				name=field.name,
				assign_from_native=f"\n    this->{field.name} = static_cast<{cpp_type}>(native.{field.name});",
				assign_to_cstruct="",
			)
			if field.type.startswith("WGPU") and any(h.name == field.type[4:] for h in api.handles) and args.force_raii and not struct_api.owning:
				field_cpp.assign_from_native += f"\n    if (this->{field.name}) this->{field.name}.addRef();"

			if field.type.startswith("WGPU") and any(s.name == field.type[4:] for s in api.structs):
				if field.is_pointer:
					raise RuntimeError(
						f"Unhandled pointer field {field.name} of wgpu struct in struct {struct_cpp.name}"
					)
				struct_cpp.binary_compatible = struct_cpp.binary_compatible and get_struct_api_cpp(field.type[4:]).binary_compatible
				if get_struct_api_cpp(field.type[4:]).extra_cstruct_members:
					struct_cpp.extra_cstruct_members.append(f"{get_cpp_field_type(field.type, struct_api.owning)}::CStruct {field.name}_cstruct;")
					field_cpp.assign_to_cstruct = (
						f"\n    cstruct.{field.name}_cstruct = this->{field.name}.to_cstruct();\n    cstruct.{field.name} = static_cast<{field.type}>(cstruct.{field.name}_cstruct);"
					)
				else:
					field_cpp.assign_to_cstruct = (
						f"\n    cstruct.{field.name} = static_cast<{field.type}>(this->{field.name}.to_cstruct());"
					)
				if field.name == "chain" and field.type == "WGPUChainedStruct":
					setter_name = "setNext"
					struct_cpp.methods_decl.append(f"\n    template <typename T>\n    {struct_cpp.name}& {setter_name}(T&& value) &;")
					struct_cpp.methods_template_impl.append(
						f"\ntemplate <typename T>\n{struct_cpp.name}& {struct_cpp.name}::{setter_name}(T&& value) & {{\n    this->{field.name}.setNext(std::forward<T>(value));\n    return *this;\n}}"
					)
					struct_cpp.methods_decl.append(f"\n    template <typename T>\n    {struct_cpp.name}&& {setter_name}(T&& value) &&;")
					struct_cpp.methods_template_impl.append(
						f"\ntemplate <typename T>\n{struct_cpp.name}&& {struct_cpp.name}::{setter_name}(T&& value) && {{\n    this->{field.name}.setNext(std::forward<T>(value));\n    return std::move(*this);\n}}"
					)
				else:
					setter_name = "set" + field.name[:1].upper() + field.name[1:]
					struct_cpp.methods_decl.append(f"\n    {struct_cpp.name}& {setter_name}(const {cpp_type}& value) &;")
					struct_cpp.methods_impl.append(
						f"\n{struct_cpp.name}& {struct_cpp.name}::{setter_name}(const {cpp_type}& value) & {{\n    this->{field.name} = value;\n    return *this;\n}}"
					)
					struct_cpp.methods_decl.append(f"\n    {struct_cpp.name}&& {setter_name}(const {cpp_type}& value) &&;")
					struct_cpp.methods_impl.append(
						f"\n{struct_cpp.name}&& {struct_cpp.name}::{setter_name}(const {cpp_type}& value) && {{\n    this->{field.name} = value;\n    return std::move(*this);\n}}"
					)
					struct_cpp.methods_decl.append(f"\n    {struct_cpp.name}& {setter_name}({cpp_type}&& value) &;")
					struct_cpp.methods_impl.append(
						f"\n{struct_cpp.name}& {struct_cpp.name}::{setter_name}({cpp_type}&& value) & {{\n    this->{field.name} = std::move(value);\n    return *this;\n}}"
					)
					struct_cpp.methods_decl.append(f"\n    {struct_cpp.name}&& {setter_name}({cpp_type}&& value) &&;")
					struct_cpp.methods_impl.append(
						f"\n{struct_cpp.name}&& {struct_cpp.name}::{setter_name}({cpp_type}&& value) && {{\n    this->{field.name} = std::move(value);\n    return std::move(*this);\n}}"
					)
				struct_cpp.fields.append(field_cpp)
				continue

			field_cpp.assign_to_cstruct = (
				f"\n    cstruct.{field.name} = static_cast<{field.full_type()}>(this->{field.name});"
			)
			setter_name = "set" + field.name[:1].upper() + field.name[1:]
			struct_cpp.methods_decl.append(f"\n    {struct_cpp.name}& {setter_name}({cpp_type} value) &;")
			struct_cpp.methods_impl.append(
				f"\n{struct_cpp.name}& {struct_cpp.name}::{setter_name}({cpp_type} value) & {{\n    this->{field.name} = std::move(value);\n    return *this;\n}}"
			)
			struct_cpp.methods_decl.append(f"\n    {struct_cpp.name}&& {setter_name}({cpp_type} value) &&;")
			struct_cpp.methods_impl.append(
				f"\n{struct_cpp.name}&& {struct_cpp.name}::{setter_name}({cpp_type} value) && {{\n    this->{field.name} = std::move(value);\n    return std::move(*this);\n}}"
			)
			struct_cpp.fields.append(field_cpp)

		struct_cpp_map[name] = struct_cpp
		return struct_cpp

	for struct_api in api.structs:
		result.structs.append(get_struct_api_cpp(struct_api.name, struct_api))

	for callback_cpp in result.callbacks:
		for param in callback_cpp.params:
			if param.is_struct:
				type_without_wgpu = param.type
				if type_without_wgpu.startswith(namespace + "::"):
					type_without_wgpu = type_without_wgpu[len(namespace) + 2:]
				param.binary_compatible = get_struct_api_cpp(type_without_wgpu).binary_compatible
			log.println(
				"Callback param: {} {}, is_handle: {}, is_struct: {}, binary_compatible: {}, is_pointer: {}, nullable: {}",
				param.type,
				param.name,
				param.is_handle,
				param.is_struct,
				param.binary_compatible,
				param.is_pointer,
				param.nullable,
			)

	def get_assign_to_native(param: FuncParamApiCpp) -> Tuple[str, str, str]:
		assign = ""
		temp_data = ""
		write_back = ""
		type_without_wgpu = param.type
		is_wgpu_type = type_without_wgpu.startswith(namespace + "::")
		if not is_wgpu_type:
			return param.name, "", ""
		if is_wgpu_type:
			type_without_wgpu = type_without_wgpu[len(namespace) + 2:]
			if type_without_wgpu.startswith("raw::"):
				type_without_wgpu = type_without_wgpu[5:]
		has_free_members = any(f.name == type_without_wgpu + "FreeMembers" for f in api.functions)
		free_members = f"wgpu{type_without_wgpu}FreeMembers" if has_free_members else ""
		if param.is_handle:
			prefix = "" if param.is_pointer else "*"
			const_suffix = " const" if (param.is_const or not param.is_pointer) else ""
			addr_prefix = "" if (param.nullable and param.is_pointer) else "&"
			assign = f"{prefix}reinterpret_cast<WGPU{type_without_wgpu}{const_suffix}*>({addr_prefix}{param.name})"
		elif param.is_pointer and param.is_struct:
			if param.nullable:
				if param.binary_compatible:
					assign = f"reinterpret_cast<WGPU{type_without_wgpu}{' const' if param.is_const else ''}*>({param.name})"
				elif param.is_const:
					temp_data = (
						f"\n    {param.type}::CStruct {param.name}_cstruct;\n    if ({param.name}) {param.name}_cstruct = {param.name}->to_cstruct();"
					)
					assign = f"{param.name}? &{param.name}_cstruct : nullptr"
				else:
					temp_data = f"\n    WGPU{type_without_wgpu} {param.name}_native;"
					assign = f"{param.name}? &{param.name}_native : nullptr"
					write_back = f"\n    if ({param.name}) *{param.name} = static_cast<{param.type}>({param.name}_native);"
					if has_free_members:
						write_back += f"\n    if ({param.name}) {free_members}({param.name}_native);"
			else:
				if param.binary_compatible:
					assign = f"reinterpret_cast<WGPU{type_without_wgpu}{' const' if param.is_const else ''}*>(&{param.name})"
				elif param.is_const:
					temp_data = f"\n    {param.type}::CStruct {param.name}_cstruct = {param.name}.to_cstruct();"
					assign = f"&{param.name}_cstruct"
				else:
					temp_data = f"\n    WGPU{type_without_wgpu} {param.name}_native;"
					assign = f"&{param.name}_native"
					write_back = f"\n    {param.name} = static_cast<{param.type}>({param.name}_native);"
					if has_free_members:
						write_back += f"\n    {free_members}({param.name}_native);"
		elif param.is_struct:
			temp_data = f"\n    {param.type}::CStruct {param.name}_cstruct = {param.name}.to_cstruct();"
			assign = f"{param.name}_cstruct"
		else:
			assign = f"static_cast<WGPU{type_without_wgpu}>({param.name})"
		return assign, temp_data, write_back

	for func_api in api.functions:
		if func_api.name.endswith("FreeMembers") or func_api.name.endswith("ProcAddress"):
			continue
		if not func_api.parent:
			func_name = func_api.name
			func_name = func_name[:1].lower() + func_name[1:]
			params_cpp = [get_func_param_cpp_type(p) for p in func_api.params]
			return_type = func_api.return_type
			if return_type.startswith("WGPU"):
				return_type = namespace + "::" + return_type[4:]
			nullable_overload = False
			if params_cpp and params_cpp[-1].nullable and params_cpp[-1].is_pointer:
				params_cpp[-1].nullable = False
				nullable_overload = True
			func_decl = f"{return_type} {func_name}({', '.join(p.full_type(namespace) + ' ' + p.name for p in params_cpp)});"
			arg3 = ", ".join(p.full_type(namespace) + " " + p.name for p in params_cpp)
			arg4 = "\n".join(get_assign_to_native(p)[1] for p in params_cpp)
			arg5 = ", ".join(get_assign_to_native(p)[0] for p in params_cpp)
			arg6 = "\n".join(get_assign_to_native(p)[2] for p in params_cpp)
			if return_type == "void":
				func_impl = (
					f"\n{return_type} {func_name}({arg3}) {{\n{arg4}\n    wgpu{func_api.name}({arg5});\n{arg6}\n}}"
				)
			else:
				func_impl = (
					f"\n{return_type} {func_name}({arg3}) {{\n{arg4}\n    {return_type} res = static_cast<{return_type}>(wgpu{func_api.name}({arg5}));\n{arg6}\n    return res;\n}}"
				)
			result.functions.append(FuncApiCpp(name=func_name, func_decl=func_decl, func_template_impl="", func_impl=func_impl))

			if nullable_overload:
				params_cpp.pop()
				func_decl = f"{return_type} {func_name}({', '.join(p.full_type(namespace) + ' ' + p.name for p in params_cpp)});"
				arg3 = ", ".join(p.full_type(namespace) + " " + p.name for p in params_cpp)
				arg4 = "\n".join(get_assign_to_native(p)[1] for p in params_cpp)
				arg5 = ", ".join(get_assign_to_native(p)[0] for p in params_cpp)
				arg6 = "\n".join(get_assign_to_native(p)[2] for p in params_cpp)
				arg7 = "nullptr" if not params_cpp else ", nullptr"
				if return_type == "void":
					func_impl = (
						f"\n{return_type} {func_name}({arg3}) {{\n{arg4}\n    wgpu{func_api.name}({arg5}{arg7});\n{arg6}\n}}"
					)
				else:
					func_impl = (
						f"\n{return_type} {func_name}({arg3}) {{\n{arg4}\n    {return_type} res = static_cast<{return_type}>(wgpu{func_api.name}({arg5}{arg7}));\n{arg6}\n    return res;\n}}"
					)
				result.functions.append(FuncApiCpp(name=func_name, func_decl=func_decl, func_template_impl="", func_impl=func_impl))
		else:
			handle_cpp = next(h for h in result.handles if h.name == func_api.parent)
			func_name = func_api.name[len(handle_cpp.name):]
			func_name = func_name[:1].lower() + func_name[1:]
			params_cpp = [get_func_param_cpp_type(p) for p in func_api.params[1:]]
			return_type = func_api.return_type
			if return_type.startswith("WGPU"):
				return_type = namespace + "::" + return_type[4:]
			nullable_overload = False
			if params_cpp and params_cpp[-1].nullable and params_cpp[-1].is_pointer:
				params_cpp[-1].nullable = False
				nullable_overload = True
			handle_cpp.methods_decl.append(
				f"\n    {return_type} {func_name}({', '.join(p.full_type(namespace) + ' ' + p.name for p in params_cpp)}) const;"
			)
			arg3 = ", ".join(p.full_type(namespace) + " " + p.name for p in params_cpp)
			arg4 = "\n".join(get_assign_to_native(p)[1] for p in params_cpp)
			arg5 = func_api.name
			arg6 = "m_raw" if not params_cpp else "m_raw, "
			arg7 = ", ".join(get_assign_to_native(p)[0] for p in params_cpp)
			arg8 = "\n".join(get_assign_to_native(p)[2] for p in params_cpp)
			if return_type == "void":
				handle_cpp.methods_impl.append(
					f"\n{return_type} {handle_cpp.name}::{func_name}({arg3}) const {{\n{arg4}\n    wgpu{arg5}({arg6}{arg7});\n{arg8}\n}}"
				)
			else:
				handle_cpp.methods_impl.append(
					f"\n{return_type} {handle_cpp.name}::{func_name}({arg3}) const {{\n{arg4}\n    {return_type} res = static_cast<{return_type}>(wgpu{arg5}({arg6}{arg7}));\n{arg8}\n    return res;\n}}"
				)
			if nullable_overload:
				params_cpp.pop()
				handle_cpp.methods_decl.append(
					f"\n    {return_type} {func_name}({', '.join(p.full_type(namespace) + ' ' + p.name for p in params_cpp)}) const;"
				)
				arg3 = ", ".join(p.full_type(namespace) + " " + p.name for p in params_cpp)
				arg4 = "\n".join(get_assign_to_native(p)[1] for p in params_cpp)
				arg6 = "m_raw" if not params_cpp else "m_raw, "
				arg7 = ", ".join(get_assign_to_native(p)[0] for p in params_cpp)
				arg8 = "\n".join(get_assign_to_native(p)[2] for p in params_cpp)
				if return_type == "void":
					handle_cpp.methods_impl.append(
						f"\n{return_type} {handle_cpp.name}::{func_name}({arg3}) const {{\n{arg4}\n    wgpu{arg5}({arg6}{arg7}, nullptr);\n{arg8}\n}}"
					)
				else:
					handle_cpp.methods_impl.append(
						f"\n{return_type} {handle_cpp.name}::{func_name}({arg3}) const {{\n{arg4}\n    {return_type} res = static_cast<{return_type}>(wgpu{arg5}({arg6}{arg7}, nullptr));\n{arg8}\n    return res;\n}}"
					)

	return result


def generate_webgpu_cpp(api: WebGpuApi, api_cpp: WebGpuApiCpp, template_meta: TemplateMeta, args: argparse.Namespace) -> None:
	namespace = args.namespace
	use_raii = args.use_raii
	headers = args.headers
	output_path = Path(args.output)
	output = template_meta.text

	type_aliases_text = ""
	for type_alias in api_cpp.type_aliases:
		if type_alias.original.startswith("WGPU"):
			continue
		cpp_type = type_alias.alias
		if cpp_type.startswith("WGPU"):
			cpp_type = cpp_type[4:]
		type_aliases_text += f"using {cpp_type} = {type_alias.original};\n"
	output = re.sub(r"\{\{type_aliases\}\}", f"namespace {namespace} {{\n{type_aliases_text}\n}}", output)

	includes_text = ""
	for header in headers:
		file_name = Path(header).name
		includes_text += f"#include <webgpu/{file_name}>\n"
	includes_text += f"\n#define WEBGPU_CPP_NAMESPACE {namespace}\n"
	output = re.sub(r"\{\{webgpu_includes\}\}", includes_text, output)

	enums_text = ""
	for enum_api in api_cpp.enums:
		name = enum_api.name + " : Flags" if enum_api.flags else enum_api.name
		enums_text += f"enum class {name} {{\n"
		for entry in enum_api.entries:
			enums_text += f"    {entry.name} = {entry.value},\n"
		enums_text += "};\n\n"
	for ets in api_cpp.enum_to_string:
		enums_text += ets.func_decl + "\n"
	for enum_api in api_cpp.enums:
		if enum_api.flags:
			enums_text += f"{enum_api.name} operator|({enum_api.name} lhs, {enum_api.name} rhs);\n"
	output = re.sub(r"\{\{enums\}\}", f"namespace {namespace} {{\n{enums_text}\n}}", output)

	enums_impl_text = ""
	for ets in api_cpp.enum_to_string:
		enums_impl_text += ets.func_impl + "\n"
	for enum_api in api_cpp.enums:
		if enum_api.flags:
			enums_impl_text += (
				f"\n{enum_api.name} operator|({enum_api.name} lhs, {enum_api.name} rhs) {{\n"
				f"    using T = std::underlying_type_t<{enum_api.name}>;\n"
				f"    return static_cast<{enum_api.name}>(static_cast<T>(lhs) | static_cast<T>(rhs));\n"
				f"}}"
			)
	output = re.sub(r"\{\{enums_impl\}\}", f"namespace {namespace} {{\n{enums_impl_text}\n}}", output)

	callbacks_decl_text = "".join(f"struct {c.name};\n" for c in api_cpp.callbacks)
	output = re.sub(r"\{\{callbacks_decl\}\}", f"namespace {namespace} {{\n{callbacks_decl_text}\n}}", output)

	callbacks_def_text = "\n\n".join(c.gen_definition(namespace) for c in api_cpp.callbacks) + "\n\n"
	output = re.sub(r"\{\{callbacks\}\}", f"namespace {namespace} {{\n{callbacks_def_text}\n}}", output)

	callbacks_template_impl_text = "\n\n".join(c.gen_template_impl(namespace) for c in api_cpp.callbacks) + "\n\n"
	output = re.sub(r"\{\{callbacks_template_impl\}\}", f"namespace {namespace} {{\n{callbacks_template_impl_text}\n}}", output)

	callbacks_impl_text = "\n\n".join(c.gen_impl(namespace) for c in api_cpp.callbacks) + "\n\n"
	output = re.sub(r"\{\{callbacks_impl\}\}", f"namespace {namespace} {{\n{callbacks_impl_text}\n}}", output)

	structs_decl_text = "".join(f"struct {s.name};\n" for s in api_cpp.structs)
	output = re.sub(r"\{\{structs_decl\}\}", f"namespace {namespace} {{\n{structs_decl_text}\n}}", output)

	structs_def_text = NEXT_IN_CHAIN_HELPER
	for struct_cpp in api_cpp.structs:
		for name, injects in template_meta.injections.members.items():
			if name == struct_cpp.name:
				struct_cpp.methods_decl.append("\n" + "\n".join(injects))
		structs_def_text += struct_cpp.gen_definition() + "\n\n"
	output = re.sub(r"\{\{structs\}\}", f"namespace {namespace} {{\n{structs_def_text}\n}}", output)

	structs_template_impl_text = "\n\n".join(s.gen_template_impl() for s in api_cpp.structs) + "\n\n"
	output = re.sub(r"\{\{structs_template_impl\}\}", f"namespace {namespace} {{\n{structs_template_impl_text}\n}}", output)

	structs_impl_text = "\n\n".join(s.gen_impl() for s in api_cpp.structs) + "\n\n"
	output = re.sub(r"\{\{structs_impl\}\}", f"namespace {namespace} {{\n{structs_impl_text}\n}}", output)

	handles_decl_text = "".join(f"class {h.name};\n" for h in api_cpp.handles)
	if use_raii:
		handles_decl_text = f"namespace {namespace}::raw {{\n{handles_decl_text}\n}}\nnamespace {namespace} {{\n{handles_decl_text}\n}}"
	else:
		handles_decl_text = f"namespace {namespace} {{\n{handles_decl_text}\n}}"
	output = re.sub(r"\{\{handles_decl\}\}", handles_decl_text, output)

	handles_def_text = ""
	for handle_cpp in api_cpp.handles:
		for name, injects in template_meta.injections.members.items():
			if name == handle_cpp.name:
				handle_cpp.methods_decl.append("\n" + "\n".join(injects))
		handles_def_text += handle_cpp.gen_definition() + "\n\n"
	if use_raii:
		handles_def_text = f"namespace {namespace}::raw {{\n{handles_def_text}\n}}\n"
		raii_def_text = ""
		raii_friends_text = "#define WEBGPU_RAII_FRIENDS"
		for handle_cpp in api_cpp.handles:
			raii_friends_text += f" \\\n    friend class raw::{handle_cpp.name};"
		for struct_cpp in api.structs:
			if struct_cpp.owning or args.force_raii:
				raii_friends_text += f" \\\n    friend struct {struct_cpp.name};"
		for func_cpp in api_cpp.functions:
			decl = re.sub(r"\n", " ", func_cpp.func_decl)
			if "wgpu" in func_cpp.func_impl:
				raii_friends_text += f" \\\n    friend {decl}"
		for handle_cpp in api_cpp.handles:
			raii_def_text += handle_cpp.gen_raii_definition() + "\n\n"
		handles_def_text += (
			raii_friends_text + "\n" + f"namespace {namespace} {{\n{raii_def_text}\n}}" + "\n#undef WEBGPU_RAII_FRIENDS\n"
		)
	else:
		handles_def_text = f"namespace {namespace} {{\n{handles_def_text}\n}}"
	output = re.sub(r"\{\{handles\}\}", handles_def_text, output)

	handles_template_impl_text = "\n\n".join(h.gen_template_impl() for h in api_cpp.handles) + "\n\n"
	if use_raii:
		handles_template_impl_text = f"namespace {namespace}::raw {{\n{handles_template_impl_text}\n}}\n"
	else:
		handles_template_impl_text = f"namespace {namespace} {{\n{handles_template_impl_text}\n}}"
	output = re.sub(r"\{\{handles_template_impl\}\}", handles_template_impl_text, output)

	handles_impl_text = "\n\n".join(h.gen_impl() for h in api_cpp.handles) + "\n\n"
	if use_raii:
		handles_impl_text = f"namespace {namespace}::raw {{\n{handles_impl_text}\n}}\n"
	else:
		handles_impl_text = f"namespace {namespace} {{\n{handles_impl_text}\n}}"
	output = re.sub(r"\{\{handles_impl\}\}", handles_impl_text, output)

	functions_decl_text = "".join(f.func_decl + "\n" for f in api_cpp.functions)
	output = re.sub(r"\{\{functions_decl\}\}", f"namespace {namespace} {{\n{functions_decl_text}\n}}", output)

	functions_template_impl_text = "\n\n".join(f.func_template_impl for f in api_cpp.functions) + "\n\n"
	output = re.sub(r"\{\{functions_template_impl\}\}", f"namespace {namespace} {{\n{functions_template_impl_text}\n}}", output)

	functions_impl_text = "\n\n".join(f.func_impl for f in api_cpp.functions) + "\n\n"
	output = re.sub(r"\{\{functions_impl\}\}", f"namespace {namespace} {{\n{functions_impl_text}\n}}", output)

	output += "#undef WEBGPU_CPP_NAMESPACE"
	output = re.sub(r"\{\{\w+\}\}", "", output)
	output = re.sub(r"(\s*\n\s*){1,}\n", "\n", output)

	output_path.write_text(output, encoding="utf-8")


def main() -> None:
	parser = argparse.ArgumentParser("Webgpu C++ Wrapper Program")
	parser.add_argument("-o", "--output", default="webgpu.cppm", help="The path of the generated file")
	parser.add_argument("-i", "--input", "--header", dest="headers", action="append", help="The path of the input header file")
	parser.add_argument("-n", "--namespace", dest="namespace", default="wgpu", help="The namespace to use in the generated file")
	parser.add_argument("-t", "--template", dest="template", default="webgpu.template.cppm", help="The template file to use for generation")
	parser.add_argument("--use-raii", action="store_true", help="Generate RAII wrappers for handles, and make non raii handles use raw namespace")
	parser.add_argument("--force-raii", action="store_true", help="while using RAII, also force structs to store raii handles")
	args = parser.parse_args()

	template_meta = load_template(Path(args.template))
	api = WebGpuApi()
	for header in args.headers:
		header_path = Path(header)
		if not header_path.exists():
			print(f"Failed to open header file: {header}")
			continue
		parse_header(api, header_path.read_text(encoding="utf-8"))
	api_cpp = produce_webgpu_cpp(api, template_meta, args)
	generate_webgpu_cpp(api, api_cpp, template_meta, args)


if __name__ == "__main__":
	main()