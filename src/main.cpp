#include <cassert>
#include <fstream>
#include <functional>
#include <iostream>
#include <print>
#include <ranges>
#include <regex>
#include <sstream>
#include <stacktrace>
#include <string>

#include "wcpp/args_parser.hpp"

void set_up_parser(ArgsParser& parser) {
    parser.add_arg_spec(ArgsParser::ArgSpec("--output", "The path of the generated file")
                            .add_name("-o")
                            .set_flag(false)
                            .add_default_value("webgpu.cppm"));
    parser.add_arg_spec(ArgsParser::ArgSpec("--header", "The path of the input header file")
                            .add_name("-i")
                            .add_name("--input")
                            .add_default_value("webgpu/webgpu.h")
                            .set_flag(false));
    parser.add_arg_spec(ArgsParser::ArgSpec("--namespace", "The namespace to use in the generated file")
                            .add_name("-n")
                            .set_flag(false)
                            .add_default_value("wgpu"));
    parser.add_arg_spec(ArgsParser::ArgSpec("--template", "The template file to use for generation")
                            .add_name("-t")
                            .set_flag(false)
                            .add_default_value("webgpu.template.cppm"));
    parser.add_arg_spec(
        ArgsParser::ArgSpec("--use-raii",
                            "Generate RAII wrappers for handles, and make non raii handles use raw namespace")
            .set_flag(true));
    parser.print_help();
}

std::string strip(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}

std::ofstream log_file("parser.log");

ArgsParser parser("Webgpu C++ Wrapper Program");

struct InjectedData {
    std::unordered_map<std::string, std::vector<std::string>> members;
};

struct TemplateMeta {
    std::string text;
    InjectedData injections;
} template_meta;

struct HandleApi {
    std::string name;  // name of the handle, without the WGPU prefix
    std::vector<std::string> methods_decl;
    std::vector<std::string> methods_impl;
};
struct FieldApi {
    std::string name;
    std::string type;                    // type of the field, WGPU prefix included
    std::optional<std::string> counter;  // name of the field that counts the number of elements if this is an array
    bool is_counter = false;
    bool nullable   = false;
    bool is_pointer = false;
    bool is_const   = false;
};
struct StructApi {
    std::string name;  // name of the struct, without the WGPU prefix
    std::vector<FieldApi> fields;
    bool is_descriptor = false;
    bool writable_arg  = false;  // this is a struct used as mutable argument in functions, like
                                // Surface::getCurrentTexture, with this field true, the struct should store raii handle
                                // instead of raw handle
};
struct FuncParamApi {
    std::string name;
    std::string type;  // type of the parameter, WGPU prefix included
    bool nullable   = false;
    bool is_pointer = false;
    bool is_const   = false;
};
struct FuncApi {
    std::string name;  // name of the function, without the wgpu prefix
    std::string return_type;
    bool nullable = false;
    std::optional<std::string> parent;  // might be bind to a handle.
    std::vector<FuncParamApi> params;
};
struct EnumentryApi {
    std::string name;   // name of the enum entry, without the WGPU and enum name prefixes
    std::string value;  // the value of this entry, can be integer or WGPUxxx_yyy
};
struct EnumApi {
    std::string name;  // name of the enum, without the WGPU prefix
    std::vector<EnumentryApi> entries;
};
struct CallbackApi {
    std::string name;  // name of the callback, without the WGPU prefix
    std::vector<FuncParamApi> params;
};
struct InitMacro {
    std::string macro;
    std::string type;
};

struct WebGpuApi {
    std::vector<HandleApi> handles;
    std::vector<StructApi> structs;
    std::vector<FuncApi> functions;
    std::vector<EnumApi> enums;
    std::vector<CallbackApi> callbacks;
    std::vector<InitMacro> init_macros;
} api;

StructApi parse_struct(const std::string& name, std::span<std::string>& lines);
EnumApi parse_enum(const std::string& name, std::span<std::string>& lines);
FuncApi parse_func(const std::string& name, const std::string& return_type, const std::string& params_str);
std::vector<FuncParamApi> parse_params(const std::string& params_str);
void parse_header(WebGpuApi& api, const std::string& header_content);
TemplateMeta loadTemplate(const std::string& template_path);

struct EnumToString {
    std::string enum_name;
    std::string func_decl;
    std::string func_impl;
};
struct StructFieldCpp {
    std::string type;
    std::string name;
    std::vector<std::string> convert_to_native;
    std::string convert_from_native;
};
struct StructApiCpp {
    std::string name;
    std::vector<StructFieldCpp> fields;
    std::vector<std::string> methods_decl;
    std::vector<std::string> methods_template_impl;
    std::vector<std::string> methods_impl;
};

struct WebGpuApiCpp {
    std::vector<EnumApi> enums;
    std::vector<EnumToString> enum_to_string;
    std::vector<StructApiCpp> structs;
};

WebGpuApiCpp produce_webgpu_cpp(const WebGpuApi& api, const TemplateMeta& template_meta);
WebGpuApiCpp produce_webgpu_cpp(const WebGpuApi& api, const TemplateMeta& template_meta) {
    WebGpuApiCpp result;
    // enums and enum to string functions
    for (auto enum_api : api.enums) {
        for (auto& entry : enum_api.entries) {
            entry.name = std::format("e{}", entry.name);  // prefix e to avoid staring with digit
        }
        result.enums.push_back(enum_api);
        // Generate enum to string function
        EnumToString ets;
        ets.enum_name = enum_api.name;
        ets.func_decl = std::format("std::string_view to_string({} value);", enum_api.name);
        std::ostringstream impl_ss;
        impl_ss << std::format("std::string_view to_string({} value) {{\n", enum_api.name);
        impl_ss << "    switch (value) {\n";
        for (const auto& entry : enum_api.entries) {
            impl_ss << std::format("        case {}::{}: return \"{}\";\n", enum_api.name, entry.name,
                                   entry.name.substr(1));
        }
        impl_ss << "        default: return \"Unknown\";\n";
        impl_ss << "    }\n";
        impl_ss << "}\n";
        ets.func_impl = impl_ss.str();
        result.enum_to_string.push_back(ets);
    }

    auto get_cpp_field_type = [&](std::string type) -> std::string {
        if (type.starts_with("WGPU")) {
            type = type.substr(4);
            if (parser.contains("--use-raii") &&
                std::ranges::contains(api.handles, type, [](const HandleApi& h) { return h.name; })) {
                return "wgpu::raw::" + type;
            } else {
                return "wgpu::" + type;
            }
        } else {
            return std::move(type);
        }
    };

    // structs
    for (const auto& struct_api : api.structs) {
        StructApiCpp struct_cpp;
        struct_cpp.name = struct_api.name;
        for (const auto& field : struct_api.fields) {
            if (field.is_counter) continue;   // we store vector for array, so no need to store count field
            if (field.counter.has_value()) {  // has a counter, this is an array field
                auto counter_type =
                    (struct_api.fields |
                     std::views::filter([&](const FieldApi& f) { return f.name == field.counter.value(); }) |
                     std::views::transform([](const FieldApi& f) { return f.type; }) |
                     std::ranges::to<std::vector<std::string>>())[0];
                StructFieldCpp field_cpp;
                field_cpp.name                = field.name;
                std::string cpp_type          = get_cpp_field_type(field.type);
                field_cpp.type                = std::format("std::vector<{}>", cpp_type);
                field_cpp.convert_from_native =  // we use span | transform | to to convert array
                    std::format(
                        "std::span(native.{}, native.{}) | std::views::transform([](auto&& e) {{ return "
                        "static_cast<{}>(e); }}) | "
                        "std::ranges::to<std::vector<{}>>();",
                        field.name, field.counter.value(), cpp_type, cpp_type);
                field_cpp.convert_to_native.push_back(
                    std::format("native.{} = static_cast<uint32_t>({}.size());", field.counter.value(), field.name));
            }
        }
        result.structs.push_back(struct_cpp);
    }

    return result;
}

void generate_webgpu_cpp(const WebGpuApiCpp& api_cpp, const TemplateMeta& template_meta) {
    std::ofstream output_file(std::string(parser.get_single("--output").value()));
    if (!output_file.is_open()) {
        throw std::runtime_error("Failed to open output file: " + std::string(parser.get_single("--output").value()));
    }
    std::string output         = template_meta.text;
    std::string namespace_name = std::string(parser.get_single("--namespace").value());

    // {{webgpu_includes}}
    std::string includes_text;
    for (const auto& header : *parser.get_all("--header")) {
        std::string file_name = header | std::views::reverse |
                                std::views::take_while([](char c) { return c != '/' && c != '\\'; }) |
                                std::views::reverse | std::ranges::to<std::string>();
        includes_text += std::format("#include <webgpu/{}>\n", file_name);
    }
    output = std::regex_replace(output, std::regex(R"(\{\{webgpu_includes\}\})"), includes_text);

    // {{enums}}
    std::string enums_text;
    for (const auto& enum_api : api_cpp.enums) {
        enums_text += std::format("enum class {} {{\n", enum_api.name);
        for (const auto& entry : enum_api.entries) {
            enums_text += std::format("    {} = {},\n", entry.name, entry.value);
        }
        enums_text += "};\n\n";
    }
    output = std::regex_replace(output, std::regex(R"(\{\{enums\}\})"),
                                std::format("namespace {} {{\n{}\n}}", namespace_name, enums_text));

    // {{enums_to_string_decl}}
    std::string enums_to_string_decl_text;
    for (const auto& ets : api_cpp.enum_to_string) {
        enums_to_string_decl_text += ets.func_decl + "\n";
    }
    output = std::regex_replace(output, std::regex(R"(\{\{enums_to_string_decl\}\})"),
                                std::format("namespace {} {{\n{}\n}}", namespace_name, enums_to_string_decl_text));

    // {{enums_to_string_impl}}
    std::string enums_to_string_impl_text;
    for (const auto& ets : api_cpp.enum_to_string) {
        enums_to_string_impl_text += ets.func_impl + "\n";
    }
    output = std::regex_replace(output, std::regex(R"(\{\{enums_to_string_impl\}\})"),
                                std::format("namespace {} {{\n{}\n}}", namespace_name, enums_to_string_impl_text));

    // {{structs_decl}}
    std::string structs_decl_text;
    for (const auto& struct_cpp : api_cpp.structs) {
        structs_decl_text += std::format("struct {};\n", struct_cpp.name);
    }
    output = std::regex_replace(output, std::regex(R"(\{\{structs_decl\}\})"),
                                std::format("namespace {} {{\n{}\n}}", namespace_name, structs_decl_text));

    // {{structs}}
    std::string structs_def_text;

    output = std::regex_replace(output, std::regex(R"(\{\{structs\}\})"),
                                std::format("namespace {} {{\n{}\n}}", namespace_name, structs_def_text));

    // remove all non used binding
    output = std::regex_replace(output, std::regex(R"(\{\{\w+\}\})"), "");
    // remain atmost one consecutive empty lines
    output = std::regex_replace(output, std::regex(R"((\n\s*){3,})"), "\n\n");
    output_file << output;
}

void handle_terminate();

int main(int argc, char** argv) {
    std::set_terminate(handle_terminate);
    set_up_parser(parser);
    parser.parse(argc, argv);

    template_meta = loadTemplate(std::string(parser.get_single("--template").value()));
    auto headers  = parser.get_all("--header");
    for (const auto& header : *headers) {
        // Load header content
        std::ifstream file(header);
        if (!file.is_open()) {
            std::cerr << "Failed to open header file: " << header << std::endl;
            continue;
        }
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        parse_header(api, content);
    }
    auto webgpu_cpp = produce_webgpu_cpp(api, template_meta);
    generate_webgpu_cpp(webgpu_cpp, template_meta);

    return 0;
}

void handle_terminate() {
    std::stacktrace st            = std::stacktrace::current();
    const std::exception_ptr eptr = std::current_exception();
    if (eptr) {
        try {
            std::rethrow_exception(eptr);
        } catch (const std::exception& ex) {
            std::println(std::cerr, "Unhandled exception: {}", ex.what());
        } catch (...) {
            std::println(std::cerr, "Unhandled non-standard exception.");
        }
    } else {
        std::println(std::cerr, "Terminated due to unknown reason.");
    }
    std::println(std::cerr, "Unhandled exception occurred. Stacktrace:\n{}", st);
    std::exit(1);
}

void parse_header(WebGpuApi& api, const std::string& header_content) {
    std::vector<std::string> lines_vec =
        header_content | std::views::split('\n') |
        std::views::transform([](auto&& rng) { return std::string(rng.begin(), rng.end()); }) |
        std::ranges::to<std::vector<std::string>>();
    std::span<std::string> lines = lines_vec;

    std::regex struct_re(R"(typedef +struct +WGPU(\w+) *\{)");
    std::regex handle_re(R"(typedef +struct .*\* *WGPU(\w+)\s+WGPU_OBJECT_ATTRIBUTE;)");
    std::regex func_re(R"(WGPU_EXPORT +([\w *]+) +wgpu(\w+)\s*\(([^)]*)\)\s+WGPU_FUNCTION_ATTRIBUTE;)");
    std::regex enum_re(R"(typedef +enum +WGPU(\w+) *\{)");
    std::regex flag_enum_re(R"(typedef +WGPUFlags +WGPU(\w+)\s*;)");
    std::regex flat_value_re(R"(static +const +WGPU(\w+) +WGPU(\w+)_(\w+) *= *(\w+)( /\*(.*)\*/)?;)");
    std::regex callback_re(R"(typedef +void +\(\*WGPU(\w+)Callback\)\((.*)\)\s*WGPU_FUNCTION_ATTRIBUTE;)");
    std::regex init_macro_re(R"(#define +(WGPU_[A-Z0-9_]+_INIT))");

    for (; !lines.empty(); lines = lines.subspan(1)) {
        std::smatch match;
        if (std::regex_search(lines[0], match, struct_re)) {
            // Parse struct
            std::string struct_name = match[1];
            std::println(log_file, "Parsing struct: {}", struct_name);
            api.structs.emplace_back(parse_struct(struct_name, lines = lines.subspan(1)));
        } else if (std::regex_search(lines[0], match, handle_re)) {
            // Parse handle
            std::string handle_name = match[1];
            std::println(log_file, "Parsing handle: {}", handle_name);
            api.handles.push_back(HandleApi{std::move(handle_name)});
        } else if (std::regex_search(lines[0], match, func_re)) {
            // Parse function
            std::string return_type = match[1];
            std::string func_name   = match[2];
            std::string params_str  = match[3];
            std::println(log_file, "Parsing function: {}", func_name);
            api.functions.push_back(parse_func(func_name, return_type, params_str));
        } else if (std::regex_search(lines[0], match, enum_re)) {
            // Parse enum
            std::string enum_name = match[1];
            std::println(log_file, "Parsing enum: {}", enum_name);
            api.enums.emplace_back(parse_enum(enum_name, lines = lines.subspan(1)));
        } else if (std::regex_search(lines[0], match, flag_enum_re)) {
            // Parse flag enum alias
            std::string enum_name = match[1];
            std::println(log_file, "Parsing flag enum alias: {}", enum_name);
            api.enums.emplace_back(EnumApi{enum_name, {}});
        } else if (std::regex_search(lines[0], match, flat_value_re)) {
            // Parse flat value
            std::string enum_name  = match[1];
            std::string enum_name2 = match[2];
            assert(enum_name == enum_name2);
            EnumentryApi entry;
            entry.name  = match[3];
            entry.value = std::format("WGPU{}_{}", enum_name, entry.name);
            std::println(log_file, "Parsing flag value: enum: {}, entry: {}, value: {}", enum_name, entry.name,
                         entry.value);
            bool found = false;
            for (auto& enum_api : api.enums) {
                if (enum_api.name == enum_name) {
                    enum_api.entries.push_back(std::move(entry));
                    found = true;
                    break;
                }
            }
            if (!found) {
                api.enums.emplace_back(EnumApi{enum_name, {std::move(entry)}});
            }
        } else if (std::regex_search(lines[0], match, callback_re)) {
            std::println(log_file, "Parsing callback: {}", (std::string)match[1]);
            api.callbacks.push_back(CallbackApi{match[1], parse_params(match[2])});
        } else if (std::regex_search(lines[0], match, init_macro_re)) {
            std::string macro = match[1];
            // Infer type from macro name
            std::size_t first = macro.find_first_of('_');
            std::size_t last  = macro.find_last_of('_');
            std::string type  = macro.substr(first, last - first).substr(1);
            // map type to correct type name, e.g. _RENDER_PASS_COLOR_ATTACHMENT -> RenderPassColorAttachment
            type = type | std::views::split('_') | std::views::transform([](auto&& rng) {
                       std::string part =
                           rng | std::views::transform([](char c) { return static_cast<char>(std::tolower(c)); }) |
                           std::ranges::to<std::string>();
                       part[0] = std::toupper(part[0]);
                       return part;
                   }) |
                   std::views::join | std::ranges::to<std::string>();
            std::println(log_file, "Found init macro: macro: {}, type: {}", macro, type);
            api.init_macros.push_back(InitMacro{macro, type});
        }
    }
}

std::vector<FuncParamApi> parse_params(const std::string& params_str) {
    std::vector<std::string> params = params_str | std::views::split(',') | std::views::transform([](auto&& rng) {
                                          auto res     = std::string(rng.begin(), rng.end());
                                          size_t first = res.find_first_not_of(' ');
                                          size_t last  = res.find_last_not_of(' ');
                                          return res.substr(first, last - first + 1);
                                      }) |
                                      std::ranges::to<std::vector<std::string>>();
    std::vector<FuncParamApi> result;
    for (auto& param : params) {
        FuncParamApi func_param;
        if (param.starts_with("WGPU_NULLABLE")) {
            func_param.nullable = true;
            param               = param.substr(sizeof("WGPU_NULLABLE"));
            param               = param.substr(param.find_first_not_of(' '));
        }
        std::vector<std::string> tokens =
            param | std::views::split(' ') | std::views::filter([](auto&& rng) { return !rng.empty(); }) |
            std::views::transform([](auto&& rng) { return std::string(rng.begin(), rng.end()); }) |
            std::ranges::to<std::vector<std::string>>();
        func_param.name = tokens.back();
        func_param.type =
            tokens | std::views::take(tokens.size() - 1) | std::views::join_with(' ') | std::ranges::to<std::string>();
        if (func_param.type.contains('*')) {
            func_param.is_pointer = true;
            func_param.type       = func_param.type.replace(func_param.type.find('*'), 1, "");
        }
        if (func_param.type.contains("const ") || func_param.type.contains(" const")) {
            func_param.is_const = true;
            func_param.type     = func_param.type.replace(func_param.type.find("const"), 5, "");
        }
        func_param.type = strip(func_param.type);
        std::println(log_file, "  Param: name: {}, type: {}, nullable: {}, is_pointer: {}, is_const: {}",
                     func_param.name, func_param.type, func_param.nullable, func_param.is_pointer, func_param.is_const);
        result.push_back(std::move(func_param));
    }
    return result;
}

FuncApi parse_func(const std::string& name, const std::string& return_type, const std::string& params_str) {
    FuncApi api;
    api.name        = name;
    api.return_type = return_type;
    if (api.return_type.starts_with("WGPU_NULLABLE")) {
        api.return_type = api.return_type.substr(sizeof("WGPU_NULLABLE"));
        api.return_type = api.return_type.substr(api.return_type.find_first_not_of(' '));
        api.nullable    = true;
    }
    std::println(log_file, "  Return type: {}, nullable: {}", api.return_type, api.nullable);
    api.params = parse_params(params_str);
    return api;
}

EnumApi parse_enum(const std::string& name, std::span<std::string>& lines) {
    static std::regex end_of_enum_re(R"(.*\})");
    static std::regex entry_re(R"(^\s+WGPU([^_]+)_([\w_]+) *= *([^,]+),?)");
    EnumApi api;
    api.name = name;

    std::smatch match;
    for (; !std::regex_search(lines[0], match, end_of_enum_re); lines = lines.subspan(1)) {
        if (std::regex_search(lines[0], match, entry_re)) {
            EnumentryApi entry;
            std::string prefix = match[1];
            entry.name         = match[2];
            entry.value        = std::format("WGPU{}_{}", prefix, entry.name);
            api.entries.push_back(std::move(entry));
        }
    }
    for (const auto& entry : api.entries) {
        std::println(log_file, "  Entry: name: {}, value: {}", entry.name, entry.value);
    }

    return api;
}

StructApi parse_struct(const std::string& name, std::span<std::string>& lines) {
    static std::regex end_of_struct_re(R"(.*\})");
    static std::regex field_re(R"(^\s*(.+) (\w+);$)");
    StructApi api;
    api.name = name;

    std::vector<std::size_t> count_fields;  // index of fields that are count of array elements for array fields
    std::smatch match;
    for (; !std::regex_search(lines[0], match, end_of_struct_re); lines = lines.subspan(1)) {
        if (std::regex_search(lines[0], match, field_re)) {
            FieldApi field;
            field.type = match[1];
            if (field.type.starts_with("WGPU_NULLABLE")) {
                field.nullable = true;
                field.type     = field.type.substr(sizeof("WGPU_NULLABLE"));
            }
            if (field.type.contains('*')) {
                field.is_pointer = true;
                field.type       = field.type.replace(field.type.find('*'), 1, "");
            }
            if (field.type.contains("const ") || field.type.contains(" const")) {
                field.is_const = true;
                field.type     = field.type.replace(field.type.find("const"), 5, "");
            }
            field.type = strip(field.type);
            field.name = match[2];
            if (field.name == "nextInChain") {
                api.is_descriptor = true;
            }
            if (field.name.ends_with("Count")) {
                count_fields.push_back(api.fields.size());
            }
            api.fields.push_back(field);
        }
    }
    for (auto& count_field : count_fields) {
        // entr|yCount -> entr|ies
        std::string array_field_prefix =
            api.fields[count_field].name.substr(0, api.fields[count_field].name.size() - 6);
        bool found = false;
        for (auto& field : api.fields) {
            if (field.name.starts_with(array_field_prefix) && field.name != api.fields[count_field].name &&
                field.is_pointer) {
                field.counter                      = api.fields[count_field].name;
                api.fields[count_field].is_counter = true;
                found                              = true;
                break;
            }
        }
    }
    for (auto& field : api.fields) {
        std::println(
            log_file,
            "  Field: type: {}, name: {}, nullable: {}, counter: {}, is_counter: {}, is_pointer: {}, is_const: {}",
            field.type, field.name, field.nullable, field.counter.has_value() ? field.counter.value() : "none",
            field.is_counter, field.is_pointer, field.is_const);
    }

    return api;
}

TemplateMeta loadTemplate(const std::string& template_path) {
    TemplateMeta meta;
    std::ifstream file(template_path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open template file: " + template_path);
    }
    std::string line;
    std::vector<std::string> current_injection;
    while (std::getline(file, line)) {
        std::smatch match;
        static std::regex begin_inject_re(R"(\{\{begin_inject\}\})");
        static std::regex end_inject_re(R"(\{\{end_inject\}\})");
        static std::regex inject_typename_re(R"(\s*typename +(\w+):)");
        if (std::regex_search(line, match, begin_inject_re)) {
            current_injection.clear();
        } else if (std::regex_search(line, match, end_inject_re)) {
            if (!current_injection.empty()) {
                std::string type_name = current_injection[0];
                current_injection.erase(current_injection.begin());
                meta.injections.members[type_name] = current_injection;
            }
            current_injection.clear();
        } else if (std::regex_search(line, match, inject_typename_re)) {
            if (!current_injection.empty()) {
                std::string type_name = current_injection[0];
                current_injection.erase(current_injection.begin());
                meta.injections.members[type_name] = current_injection;
            }
            current_injection.clear();
            current_injection.push_back(match[1]);
        } else {
            if (!current_injection.empty()) {
                current_injection.push_back(line);
            } else {
                meta.text += line + "\n";
            }
        }
    }
    std::println(log_file, "Loaded template:\ntext:\n{}\n-----------\ninjections:\n{}\n\n", meta.text,
                 meta.injections.members);
    return meta;
}