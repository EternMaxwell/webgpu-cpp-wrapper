#include <cassert>
#include <format>
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
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos) {
        return "";
    }
    size_t last = str.find_last_not_of(' ');
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
};
struct FieldApi {
    std::string name;
    std::string type;                    // type of the field, WGPU prefix included
    std::optional<std::string> counter;  // name of the field that counts the number of elements if this is an array
    bool is_counter = false;
    bool nullable   = false;
    bool is_pointer = false;
    bool is_const   = false;

    std::string full_type() const {
        std::string full_type_str;
        if (is_const) {
            full_type_str += "const ";
        }
        full_type_str += type;
        if (is_pointer) {
            full_type_str += "*";
        }
        return full_type_str;
    }
};
struct StructApi {
    std::string name;  // name of the struct, without the WGPU prefix
    std::vector<FieldApi> fields;
    bool is_descriptor = false;
    bool owning = false;  // this struct is a return type of any function, or as a mutable arg, should use raii handles
};
struct FuncParamApi {
    std::string name;
    std::string type;  // type of the parameter, WGPU prefix included
    bool nullable   = false;
    bool is_pointer = false;
    bool is_const   = false;

    std::string full_type() const {
        std::string full_type_str = type;
        if (is_const) full_type_str += " const";
        if (is_pointer) full_type_str += "*";
        return full_type_str;
    }
};
struct FuncApi {
    std::string name;         // name of the function, without the wgpu prefix
    std::string return_type;  // return type of the function, WGPU prefix included
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
    bool flags = false;
    std::vector<EnumentryApi> entries;
};
struct CallbackApi {
    std::string name;  // name of the callback, without the WGPU prefix and Callback suffix
    std::vector<FuncParamApi> params;
};
struct InitMacro {
    std::string macro;  // the macro
    std::string type;   // the type of the struct this init macro is for, without WGPU prefix
};
struct TypeAliasApi {
    std::string alias;
    std::string original;
};

struct WebGpuApi {
    std::vector<HandleApi> handles;
    std::vector<StructApi> structs;
    std::vector<FuncApi> functions;
    std::vector<EnumApi> enums;
    std::vector<CallbackApi> callbacks;
    std::vector<InitMacro> init_macros;
    std::vector<TypeAliasApi> type_aliases;
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
// Callback Wrapper is a container of ref counted any invocable object
struct FuncParamApiCpp {
    std::string type;                // type of the param in cpp.
    std::string name;                // name of the param in cpp.
    bool is_pointer        = false;  // whether this param is passed by reference
    bool is_const          = false;
    bool is_struct         = false;
    bool is_handle         = false;
    bool binary_compatible = false;  // whether the struct param is binary compatible, only valid when is_struct is true
    bool nullable          = false;  // whether this param is nullable, if true, will change reference to pointer

    std::string full_type() const {
        std::string full_type_str = type;
        if (is_handle && !is_pointer) {
            full_type_str += " const";
            full_type_str += "&";
        } else if (is_pointer) {
            if (is_const) full_type_str += " const";
            std::string ns = std::string(parser.get_single("--namespace").value_or("wgpu"));
            full_type_str += nullable || !type.starts_with(ns + "::") ? "*" : "&";
        }
        return full_type_str;
    }
    // assign param from native args, return a pair of assign code and temp data needed, mainly for callbacks
    std::pair<std::string, std::string> get_assign_from_native() const {
        std::string assign;
        std::string temp_data;
        if (is_handle) {
            assign = std::format("{}reinterpret_cast<{}{}*>({}{})", nullable ? "" : "*", type,
                                 (is_pointer && !is_const) ? "" : " const", is_pointer ? "" : "&", name);
        } else if (is_pointer && is_struct) {  // pointer param, since not handle, likely be structs
            if (nullable) {                    // pass as const ptr
                if (binary_compatible) {       // binary compatible, can reinterpret cast
                    assign = std::format("reinterpret_cast<{}{}*>({})", type, is_const ? " const" : "", name);
                } else {  // not binary compatible, need a temp data.
                    temp_data = std::format(
                        R"(
    {1} {0}_temp;
    if ({0}) {0}_temp = static_cast<{1}>(*{0});)",
                        name, type);
                    assign = std::format("{}? &{}_temp : nullptr", name, name);
                }
            } else {                      // pass as const ref
                if (binary_compatible) {  // binary compatible, can reinterpret cast
                    assign = std::format("*reinterpret_cast<{} const*>({})", type, name);
                } else {  // not binary compatible, need a temp data.
                    temp_data = std::format("\t{1} {0}_temp = static_cast<{1}>(*{0});\n", name, type);
                    assign    = std::format("{}_temp", name);
                }
            }
        } else {
            // not handle, not pointer, maybe struct or enum passed by value, static_cast
            assign = std::format("static_cast<{}>({})", type, name);
        }
        return {assign, temp_data};
    }
};
struct CallbackApiCpp {
    std::string name;
    std::vector<FuncParamApiCpp> params;
    std::vector<std::string> userdatas;

    std::string gen_param_types() const {
        return params | std::views::transform([](const FuncParamApiCpp& p) { return p.full_type(); }) |
               std::views::join_with(std::string(", ")) | std::ranges::to<std::string>();
    }
    std::string gen_param_names() const {
        return params | std::views::transform([](const FuncParamApiCpp& p) { return p.name; }) |
               std::views::join_with(std::string(", ")) | std::ranges::to<std::string>();
    }
    std::string gen_param_sig() const {
        return params | std::views::transform([](const FuncParamApiCpp& p) { return p.full_type() + " " + p.name; }) |
               std::views::join_with(std::string(", ")) | std::ranges::to<std::string>();
    }
    std::string gen_cparam_sig() const {
        std::string ns = std::string(parser.get_single("--namespace").value_or("wgpu"));
        ns += "::";
        return params | std::views::transform([ns](const FuncParamApiCpp& p) {
                   std::string type = p.type;
                   if (type.starts_with(ns)) {
                       type = type.substr(ns.size());
                       if (type.starts_with("raw::")) type = type.substr(5);
                       type = "WGPU" + type;
                   }
                   if (p.is_pointer) type += " const*";
                   return type + " " + p.name;
               }) |
               std::views::join_with(std::string(", ")) | std::ranges::to<std::string>();
    }
    std::string gen_definition() const {
        return std::format(
            R"(
struct {0} {{
    struct Control {{
        std::atomic<std::size_t> count{{1}};
        virtual ~Control() = default;
        virtual void invoke({3}) const = 0;
        virtual void invoke_c({4}) const;
    }};
private:
    template <typename F>
    struct ControlImpl : Control {{
        F func;
        ControlImpl(const F& f) : func(f) {{}}
        void invoke({3}) const override;
    }};
    Control* data;
public:
    {0}() : data(nullptr) {{}}
    {0}(WGPU{0} native, {5});
    {0}(const {0}& other) : data(other.data) {{ if (data) ++data->count; }}
    {0}({0}&& other) noexcept : data(other.data) {{ other.data = nullptr; }}
    {0}& operator=(const {0}& other);
    {0}& operator=({0}&& other);
    {0}& operator=(std::nullptr_t) {{ reset(); return *this; }}
    ~{0}() {{ if (data && --data->count == 0) {{ delete data; }} }}
    template <std::invocable<{1}> F>
    {0}(const F& f);
    void operator()({3}) const;
    void operator()({4}) const;
    void reset() {{ if (data && --data->count == 0) {{ delete data; }} data = nullptr; }}
    operator bool() const {{ return data != nullptr; }}
}};)",
            name, gen_param_types(), gen_param_names(), gen_param_sig(), gen_cparam_sig(),
            userdatas | std::views::transform([](const std::string& s) { return "void* " + s; }) |
                std::views::join_with(std::string(", ")) | std::ranges::to<std::string>());
    }
    std::string gen_template_impl() const {
        return std::format(
            R"(template <std::invocable<{1}> F>
{0}::{0}(const F& f) {{
    data = new ControlImpl<F>(f);
}}
template <typename F>
void {0}::ControlImpl<F>::invoke({3}) const {{
    func({2});
}})",
            name, gen_param_types(), gen_param_names(), gen_param_sig());
    }
    std::string gen_impl() const {
        return std::format(
            R"(
struct {0}ControlNative : {0}::Control {{
    WGPU{0} native;
{8}
    {0}ControlNative(WGPU{0} n, {6}) : native(n), {9} {{}}
    void invoke({2}) const override {{}}
    void invoke_c({3}) const override;
}};
void {0}ControlNative::invoke_c({3}) const {{
    native({1}, {7});
}}
{0}::{0}(WGPU{0} native, {6}) {{
    if (native) {{
        data = new {0}ControlNative(native, {7});
    }} else {{
        data = nullptr;
    }}
}}
void {0}::Control::invoke_c({3}) const {{
{4}
    invoke({5});
}}
{0}& {0}::operator=(const {0}& other) {{
    if (this != &other) {{
        if (data && --data->count == 0) {{ delete data; }}
        data = other.data;
        if (data) ++data->count;
    }}
    return *this;
}}
{0}& {0}::operator=({0}&& other) {{
    if (this != &other) {{
        if (data && --data->count == 0) {{ delete data; }}
        data = other.data;
        other.data = nullptr;
    }}
    return *this;
}}
void {0}::operator()({2}) const {{ if (data) data->invoke({1}); }}
void {0}::operator()({3}) const {{ if (data) data->invoke_c({1}); }}
)",
            name, gen_param_names(), gen_param_sig(), gen_cparam_sig(),
            params | std::views::transform([](const FuncParamApiCpp& p) { return p.get_assign_from_native().second; }) |
                std::views::join_with(std::string("\n")) | std::ranges::to<std::string>(),
            params | std::views::transform([](const FuncParamApiCpp& p) { return p.get_assign_from_native().first; }) |
                std::views::join_with(std::string(", ")) | std::ranges::to<std::string>(),
            userdatas | std::views::transform([](const std::string& s) { return "void* " + s; }) |
                std::views::join_with(std::string(", ")) | std::ranges::to<std::string>(),
            userdatas | std::views::join_with(std::string(", ")) | std::ranges::to<std::string>(),
            userdatas | std::views::transform([](const std::string& s) { return "    void* " + s + ";"; }) |
                std::views::join_with(std::string("\n")) | std::ranges::to<std::string>(),
            userdatas | std::views::transform([](const std::string& s) { return std::format("{}({})", s, s); }) |
                std::views::join_with(std::string(", ")) | std::ranges::to<std::string>());
    }
};

constexpr auto nextInChainHelper = R"(
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
};)";

struct StructFieldCpp {
    std::string type;
    std::string name;
    std::string assign_from_native;  // code snippet to assign value to this field using native struct data
    std::string assign_to_cstruct;   // code snippet to assign value to CStruct fields using this field data
};
struct StructApiCpp {
    bool binary_compatible = true;
    std::string name;
    std::string init_macro;
    std::vector<StructFieldCpp> fields;
    std::vector<std::string> methods_decl;
    std::vector<std::string> methods_template_impl;
    std::vector<std::string> methods_impl;
    std::vector<std::string>
        extra_cstruct_members;  // a CStruct nested struct which inherits from the native struct and adds extra members

    std::string gen_definition() const {
        return std::format(
            R"(
struct {0} {{
    struct CStruct : public WGPU{0} {{
        {2}
    }};
    {0}(const WGPU{0}& native);
    {0}(){1} {{}};
    CStruct to_cstruct() const;
    {3}
    {4}
}};)",
            name, init_macro.empty() ? "" : std::format(" : {}({})", name, init_macro),
            extra_cstruct_members | std::views::join_with(std::string("\n        ")) | std::ranges::to<std::string>(),
            methods_decl | std::views::join_with(std::string("\n    ")) | std::ranges::to<std::string>(),
            fields |
                std::views::transform([](const StructFieldCpp& f) { return std::format("{} {};", f.type, f.name); }) |
                std::views::join_with(std::string("\n    ")) | std::ranges::to<std::string>(),
            binary_compatible ? "true" : "false");
    }
    std::string gen_template_impl() const {
        return methods_template_impl | std::views::join_with(std::string("\n")) | std::ranges::to<std::string>();
    }
    std::string gen_impl() const {
        std::string from_native_impl =
            std::format(R"(
{0}::{0}(const WGPU{0}& native) {{
{1}
}})",
                        name,
                        fields | std::views::transform([](const StructFieldCpp& f) { return f.assign_from_native; }) |
                            std::views::join_with(std::string("\n")) | std::ranges::to<std::string>());
        std::string to_cstruct_impl =
            std::format(R"(
{0}::CStruct {0}::to_cstruct() const {{
    CStruct cstruct;
{1}
    return cstruct;
}})",
                        name,
                        fields | std::views::transform([](const StructFieldCpp& f) { return f.assign_to_cstruct; }) |
                            std::views::join_with(std::string("\n")) | std::ranges::to<std::string>());
        std::string methods = methods_impl | std::views::join_with(std::string("\n")) | std::ranges::to<std::string>();
        return from_native_impl + "\n" + to_cstruct_impl + "\n" + methods;
    }
};

// handle info struct for webgpu cpp wrapper handle, will be raw handle if use raii
struct HandleApiCpp {
    std::string name;
    std::vector<std::string> methods_decl;
    std::vector<std::string> methods_template_impl;
    std::vector<std::string> methods_impl;

    std::string gen_raii_definition() const {
        return std::format(R"(
class {0} : public raw::{0} {{
public:
    using base_type = raw::{0};
    using wgpu_type = WGPU{0};
    {0}() : base_type() {{}}
    WEBGPU_RAII_FRIENDS
private:
    {0}(wgpu_type raw) : base_type(raw) {{}}
    {0}(base_type raw) : base_type(raw) {{}}
    {0}& operator=(const base_type& raw) {{ if (*this) this->release(); base_type::operator=(raw); return *this; }}
public:
    {0}& operator=(std::nullptr_t) {{ if (*this) this->release(); base_type::operator=(nullptr); return *this; }}
    {0}(const {0}& other) : base_type(other) {{ if (*this) this->addRef(); }}
    {0}({0}&& other) : base_type(other) {{ (base_type&)(other) = nullptr; }}
    {0}& operator=(const {0}& other) {{ if (this != &other) {{ if (*this) this->release(); base_type::operator=(other); if (*this) this->addRef(); }} return *this; }}
    {0}& operator=({0}&& other) {{ if (this != &other) {{ if (*this) this->release(); base_type::operator=(other); (base_type&)(other) = nullptr; }} return *this; }}
    ~{0}() {{ if (*this) this->release(); }}
    operator bool() const {{ return base_type::operator bool(); }}
    bool operator==(const {0}& other) const {{ return base_type::operator==(other); }}
    bool operator!=(const {0}& other) const {{ return base_type::operator!=(other); }}
    {0} clone() const {{ this->addRef(); return {0}((const base_type&)*this); }}
}};)",
                           name);
    }
    std::string gen_definition() const {
        return std::format(R"(
class {0} {{
public:
    using wgpu_type = WGPU{0};
    {0}() : m_raw(nullptr) {{}}
    {0}(WGPU{0} raw) : m_raw(raw) {{}}
    operator WGPU{0}() const {{ return m_raw; }}
    operator bool() const {{ return m_raw != nullptr; }}
    bool operator==(const {0}& other) const {{ return m_raw == other.m_raw; }}
    bool operator!=(const {0}& other) const {{ return m_raw != other.m_raw; }}
    {0}& operator=(std::nullptr_t) {{ m_raw = nullptr; return *this; }}
{1}
private:
    WGPU{0} m_raw;
}};)",
                           name,
                           methods_decl | std::views::join_with(std::string("\n")) | std::ranges::to<std::string>());
    }
    std::string gen_template_impl() const {
        return methods_template_impl | std::views::join_with(std::string("\n")) | std::ranges::to<std::string>();
    }
    std::string gen_impl() const {
        return methods_impl | std::views::join_with(std::string("\n")) | std::ranges::to<std::string>();
    }
};
struct FuncApiCpp {
    std::string name;
    std::string func_decl;
    std::string func_template_impl;  // will be empty if not templated
    std::string func_impl;           // will be empty if templated
};

struct WebGpuApiCpp {
    std::vector<TypeAliasApi> type_aliases;
    std::vector<EnumApi> enums;
    std::vector<EnumToString> enum_to_string;
    std::vector<StructApiCpp> structs;
    std::vector<CallbackApiCpp> callbacks;
    std::vector<HandleApiCpp> handles;
    std::vector<FuncApiCpp> functions;  // functions that are not bind to any handle
};

WebGpuApiCpp produce_webgpu_cpp(const WebGpuApi& api, const TemplateMeta& template_meta);
WebGpuApiCpp produce_webgpu_cpp(const WebGpuApi& api, const TemplateMeta& template_meta) {
    WebGpuApiCpp result;
    std::string ns = std::string(parser.get_single("--namespace").value_or("wgpu"));

    // type aliases
    for (const auto& type_alias_api : api.type_aliases) {
        result.type_aliases.push_back(type_alias_api);
    }

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

    // handles, function will be added later by api.functions
    for (const auto& handle_api : api.handles) {
        result.handles.push_back(HandleApiCpp{.name = handle_api.name});
    }

    // helpers
    auto get_func_param_cpp_type = [&](const FuncParamApi& param) -> FuncParamApiCpp {
        FuncParamApiCpp param_cpp;
        param_cpp.name                = param.name;
        std::string type_without_wgpu = param.type;
        if (type_without_wgpu.starts_with("WGPU")) {
            type_without_wgpu = type_without_wgpu.substr(4);
        }
        if (param.type.starts_with("WGPU")) {
            param_cpp.type = ns + "::" + param.type.substr(4);
        } else {
            param_cpp.type = param.type;
        }
        param_cpp.is_pointer = param.is_pointer;
        param_cpp.nullable   = param.nullable;
        param_cpp.is_const   = param.is_const;

        // param is handle
        if (param.type.starts_with("WGPU") &&
            std::ranges::contains(api.handles, type_without_wgpu, [](const HandleApi& h) { return h.name; })) {
            param_cpp.is_handle = true;
        }
        if (param.type.starts_with("WGPU") &&
            std::ranges::contains(api.structs, type_without_wgpu, [](const StructApi& s) { return s.name; })) {
            param_cpp.is_struct = true;
        }
        return param_cpp;
    };

    // callbacks
    for (auto& callback_api : api.callbacks) {
        CallbackApiCpp callback_cpp;
        callback_cpp.name = callback_api.name + "Callback";
        for (const auto& param : callback_api.params | std::views::filter([](const FuncParamApi& p) {
                                     return !p.name.starts_with("userdata");
                                 })) {
            callback_cpp.params.push_back(get_func_param_cpp_type(param));
        }
        for (const auto& param : callback_api.params | std::views::filter([](const FuncParamApi& p) {
                                     return p.name.starts_with("userdata");
                                 })) {
            callback_cpp.userdatas.push_back(param.name);
        }
        result.callbacks.push_back(std::move(callback_cpp));
    }

    // struct helpers
    auto get_cpp_field_type = [&](std::string type, bool owning) -> std::string {
        if (type.starts_with("WGPU")) {
            type = type.substr(4);
            if (parser.contains("--use-raii") && (!owning) &&
                std::ranges::contains(api.handles, type, [](const HandleApi& h) { return h.name; })) {
                return ns + "::raw::" + type;
            } else {
                return ns + "::" + type;
            }
        } else {
            return std::move(type);
        }
    };

    // structs
    std::unordered_map<std::string, StructApiCpp> struct_cpp_map;
    auto get_struct_api_cpp = [&](this auto&& get_struct_api_cpp, const std::string& name,
                                  const StructApi* pstruct_api = nullptr) -> StructApiCpp& {
        if (struct_cpp_map.find(name) != struct_cpp_map.end()) {
            return struct_cpp_map.at(name);
        }
        if (!pstruct_api) {
            auto it = std::ranges::find_if(api.structs, [&](const StructApi& s) { return s.name == name; });
            assert(it != api.structs.end());
            pstruct_api = &(*it);
        }
        auto& struct_api = *pstruct_api;
        StructApiCpp struct_cpp;
        struct_cpp.name = struct_api.name;
        // init macro
        for (const auto& init_macro : api.init_macros) {
            if (struct_api.name == init_macro.type) {
                struct_cpp.init_macro = init_macro.macro;
                break;
            }
        }
        // fields, converts, setters
        for (const auto& field : struct_api.fields) {
            if (field.is_counter) continue;   // we store vector for array, so no need to store count field
            if (field.counter.has_value()) {  // has a counter, this is an array field
                struct_cpp.binary_compatible = false;
                auto counter_type =
                    (struct_api.fields |
                     std::views::filter([&](const FieldApi& f) { return f.name == field.counter.value(); }) |
                     std::views::transform([](const FieldApi& f) { return f.type; }) |
                     std::ranges::to<std::vector<std::string>>())[0];
                StructFieldCpp field_cpp;
                field_cpp.name       = field.name;
                std::string cpp_type = get_cpp_field_type(field.type, struct_api.owning);
                field_cpp.type       = std::format("std::vector<{}>", cpp_type);

                // converter
                field_cpp.assign_from_native =  // we use span | transform | to to convert array
                    std::format(R"(
    this->{} = std::span(native.{}, native.{}) | std::views::transform([](auto&& e) {{ return static_cast<{}>(e); }}) | std::ranges::to<std::vector<{}>>();)",
                                field.name, field.name, field.counter.value(), cpp_type, cpp_type);
                auto type_without_wgpu = field.type;
                if (type_without_wgpu.starts_with("WGPU")) {
                    type_without_wgpu = type_without_wgpu.substr(4);
                }
                if (field_cpp.type == field.type ||
                    std::ranges::contains(api.enums, type_without_wgpu, [](const EnumApi& e) { return e.name; }) ||
                    std::ranges::contains(api.handles, type_without_wgpu, [](const HandleApi& h) { return h.name; }) ||
                    get_struct_api_cpp(field.type.substr(4), nullptr).binary_compatible) {
                    // binary compatible, reinterpret cast
                    field_cpp.assign_to_cstruct =
                        std::format(R"(
    cstruct.{0} = reinterpret_cast<{1}>(this->{0}.data());
    cstruct.{2} = static_cast<{3}>(this->{0}.size());)",
                                    field.name, field.full_type(), field.counter.value(), counter_type);
                } else if (get_struct_api_cpp(field.type.substr(4), nullptr).extra_cstruct_members.empty()) {
                    // no extra members, can direct cast to new vector
                    struct_cpp.extra_cstruct_members.push_back(
                        std::format("std::vector<{}> {}_vec;", field.type, field.name));
                    field_cpp.assign_to_cstruct =
                        std::format(R"(
    cstruct.{0}_vec = this->{0} | std::views::transform([](auto&& e) {{ return static_cast<{1}>(e.to_cstruct()); }}) | std::ranges::to<std::vector<{1}>>();
    cstruct.{0} = cstruct.{0}_vec.data();
    cstruct.{2} = static_cast<{3}>(cstruct.{0}_vec.size());)",
                                    field.name, field.type, field.counter.value(), counter_type);
                } else {
                    // has extra members, need to also store a cstruct array.
                    struct_cpp.extra_cstruct_members.push_back(
                        std::format("std::vector<{}::CStruct> {}_cstruct_vec;", cpp_type, field.name));
                    struct_cpp.extra_cstruct_members.push_back(
                        std::format("std::vector<{}> {}_vec;", field.type, field.name));
                    field_cpp.assign_to_cstruct =
                        std::format(R"(
    cstruct.{0}_cstruct_vec = this->{0} | std::views::transform([](auto&& e) {{ return e.to_cstruct(); }}) | std::ranges::to<std::vector<{4}::CStruct>>();
    cstruct.{0}_vec = cstruct.{0}_cstruct_vec | std::views::transform([](auto&& e) {{ return static_cast<{1}>(e); }}) | std::ranges::to<std::vector<{1}>>();
    cstruct.{0} = cstruct.{0}_vec.data();
    cstruct.{2} = static_cast<{3}>(cstruct.{0}_vec.size());)",
                                    field.name, field.type, field.counter.value(), counter_type, cpp_type);
                }

                // setter
                std::string setter_name = field.name;
                setter_name[0]          = std::toupper(setter_name[0]);
                setter_name             = "set" + setter_name;
                struct_cpp.methods_decl.push_back(std::format(R"(
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, {}>
    {}& {}(T&& values) &;)",
                                                              cpp_type, struct_cpp.name, setter_name));
                struct_cpp.methods_template_impl.push_back(std::format(R"(
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, {0}>
{1}& {1}::{2}(T&& values) & {{
    this->{3} = values | std::views::transform([](auto&& e) {{ return static_cast<{0}>(e); }}) | std::ranges::to<std::vector<{0}>>();
    return *this;
}})",
                                                                       cpp_type, struct_cpp.name, setter_name,
                                                                       field.name));
                struct_cpp.methods_decl.push_back(std::format(R"(
    template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, {}>
    {}&& {}(T&& values) &&;)",
                                                              cpp_type, struct_cpp.name, setter_name));
                struct_cpp.methods_template_impl.push_back(std::format(R"(
template <std::ranges::range T> requires std::convertible_to<std::ranges::range_value_t<T>, {0}>
{1}&& {1}::{2}(T&& values) && {{
    this->{3} = values | std::views::transform([](auto&& e) {{ return static_cast<{0}>(e); }}) | std::ranges::to<std::vector<{0}>>();
    return std::move(*this);
}})",
                                                                       cpp_type, struct_cpp.name, setter_name,
                                                                       field.name));

                struct_cpp.fields.push_back(std::move(field_cpp));
                continue;
            }
            if (field.is_pointer && field.type == "void" && field.name.starts_with("userdata") &&
                struct_cpp.name.ends_with("CallbackInfo")) {
                // userdata in callback info struct, should be skipped, since we are using std::function for
                // callback, which can store lambda captures.
                struct_cpp.binary_compatible = false;
                continue;
            }
            if (struct_cpp.name.ends_with("CallbackInfo") && field.name == "callback") {
                // callback function will be a separate callback type, not std::function for performance reason
                struct_cpp.binary_compatible = false;
                StructFieldCpp field_cpp;
                field_cpp.name = field.name;
                field_cpp.type = get_cpp_field_type(field.type, struct_api.owning);  // will be wgpu::<...>Callback
                const CallbackApi& callback_api = *std::ranges::find_if(
                    api.callbacks, [&](const CallbackApi& c) { return c.name + "Callback" == field.type.substr(4); });

                // TODO: converter and setter will be added after callback is completed
                field_cpp.assign_from_native = std::format(
                    R"(
    if (native.{0} != nullptr) {{
        this->{0} = {1}(native.{0}, {2});
    }})",
                    field.name, field_cpp.type,
                    struct_api.fields |
                        std::views::filter([](const FieldApi& f) { return f.name.starts_with("userdata"); }) |
                        std::views::transform([](const FieldApi& f) { return "native." + f.name; }) |
                        std::views::join_with(std::string(", ")) | std::ranges::to<std::string>());
                field_cpp.assign_to_cstruct = std::format(
                    R"(
    if (this->{0}) {{
        cstruct.{0} = []({1}) {{
            auto callback = std::move(*reinterpret_cast<{4}*>({2}));
            callback({3});
        }};
        new (cstruct.{2}) {4}(this->{0});
    }} else {{
        cstruct.{0} = nullptr;
    }})",
                    field.name,
                    callback_api.params |
                        std::views::transform([](const FuncParamApi& p) { return p.full_type() + " " + p.name; }) |
                        std::views::join_with(std::string(", ")) | std::ranges::to<std::string>(),
                    (callback_api.params |
                     std::views::filter([](const FuncParamApi& p) { return p.name.starts_with("userdata"); }) |
                     std::views::take(1) | std::views::transform([](const FuncParamApi& p) { return p.name; }) |
                     std::ranges::to<std::vector>())[0],
                    callback_api.params |
                        std::views::filter([](const FuncParamApi& p) { return !p.name.starts_with("userdata"); }) |
                        std::views::transform([](const FuncParamApi& p) { return p.name; }) |
                        std::views::join_with(std::string(", ")) | std::ranges::to<std::string>(),
                    field_cpp.type);

                // setter
                std::string setter_name = field.name;
                setter_name[0]          = std::toupper(setter_name[0]);
                setter_name             = "set" + setter_name;
                struct_cpp.methods_decl.push_back(std::format(R"(
    {}& {}({} value) &;)",
                                                              struct_cpp.name, setter_name, field_cpp.type));
                struct_cpp.methods_impl.push_back(std::format(R"(
{0}& {0}::{1}({2} value) & {{
    this->{3} = std::move(value);
    return *this;
}})",
                                                              struct_cpp.name, setter_name, field_cpp.type,
                                                              field.name));

                struct_cpp.methods_decl.push_back(std::format(R"(
    {}&& {}({} value) &&;)",
                                                              struct_cpp.name, setter_name, field_cpp.type));
                struct_cpp.methods_impl.push_back(std::format(R"(
{0}&& {0}::{1}({2} value) && {{
    this->{3} = std::move(value);
    return std::move(*this);
}})",
                                                              struct_cpp.name, setter_name, field_cpp.type,
                                                              field.name));

                struct_cpp.fields.push_back(std::move(field_cpp));
                continue;
            }
            if (field.is_pointer && field.nullable && field.type.starts_with("WGPU") &&
                std::ranges::contains(api.structs, field.type.substr(4), [&](const StructApi& s) {
                    return s.name;
                })) {  // optional struct field, use std::optional
                struct_cpp.binary_compatible = false;
                StructFieldCpp field_cpp;
                field_cpp.name       = field.name;
                std::string cpp_type = get_cpp_field_type(field.type, struct_api.owning);
                field_cpp.type       = std::format("std::optional<{}>", cpp_type);

                // converter
                field_cpp.assign_from_native = std::format(
                    R"(
    if (native.{0} != nullptr) {{
        this->{0} = static_cast<{1}>(*(native.{0}));
    }} else {{
        this->{0}.reset();
    }})",
                    field.name, cpp_type);
                // assign to cstruct
                if (field_cpp.type == field.type ||
                    std::ranges::contains(api.enums, field.type.substr(4), [](const EnumApi& e) { return e.name; }) ||
                    std::ranges::contains(api.handles, field.type.substr(4),
                                          [](const HandleApi& h) { return h.name; }) ||
                    get_struct_api_cpp(field.type.substr(4), nullptr).binary_compatible) {
                    // binary compatible, reinterpret cast
                    field_cpp.assign_to_cstruct = std::format(
                        R"(
    if (this->{0}.has_value()) {{
        cstruct.{0} = reinterpret_cast<{1}>(&(*(this->{0})));
    }} else {{
        cstruct.{0} = nullptr;
    }})",
                        field.name, field.full_type());
                } else {
                    // need to store a cstruct member
                    struct_cpp.extra_cstruct_members.push_back(
                        std::format("{}::CStruct {}_cstruct;", cpp_type, field.name));
                    field_cpp.assign_to_cstruct = std::format(R"(
    if (this->{0}.has_value()) {{
        cstruct.{0}_cstruct = this->{0}->to_cstruct();
        cstruct.{0} = &(cstruct.{0}_cstruct);
    }} else {{
        cstruct.{0} = nullptr;
    }})",
                                                              field.name);
                }

                // setter
                std::string setter_name = field.name;
                setter_name[0]          = std::toupper(setter_name[0]);
                setter_name             = "set" + setter_name;
                // two setter, rvalue and lvalue
                std::string method_signature =
                    std::format("\t{}& {}(const {}& value) &", struct_cpp.name, setter_name, cpp_type);
                struct_cpp.methods_decl.push_back(std::format(R"(
    {}& {}(const {}& value) &;)",
                                                              struct_cpp.name, setter_name, cpp_type));
                struct_cpp.methods_impl.push_back(std::format(R"(
{0}& {0}::{1}(const {2}& value) & {{
    this->{3}.emplace(value);
    return *this;
}})",
                                                              struct_cpp.name, setter_name, cpp_type, field.name));

                struct_cpp.methods_decl.push_back(std::format(R"(
    {}&& {}(const {}& value) &&;)",
                                                              struct_cpp.name, setter_name, cpp_type));
                struct_cpp.methods_impl.push_back(std::format(R"(
{0}&& {0}::{1}(const {2}& value) && {{
    this->{3}.emplace(value);
    return std::move(*this);
}})",
                                                              struct_cpp.name, setter_name, cpp_type, field.name));

                struct_cpp.methods_decl.push_back(std::format(R"(
    {}& {}({}&& value) &;)",
                                                              struct_cpp.name, setter_name, cpp_type));
                struct_cpp.methods_impl.push_back(std::format(R"(
{0}& {0}::{1}({2}&& value) & {{
    this->{3}.emplace(std::move(value));
    return *this;
}})",
                                                              struct_cpp.name, setter_name, cpp_type, field.name));
                struct_cpp.methods_decl.push_back(std::format(R"(
    {}&& {}({}&& value) &&;)",
                                                              struct_cpp.name, setter_name, cpp_type));
                struct_cpp.methods_impl.push_back(std::format(R"(
{0}&& {0}::{1}({2}&& value) && {{
    this->{3}.emplace(std::move(value));
    return std::move(*this);
}})",
                                                              struct_cpp.name, setter_name, cpp_type, field.name));

                struct_cpp.fields.push_back(std::move(field_cpp));
                continue;
            }
            if (field.is_pointer && (field.name == "nextInChain" || field.name == "next")) {
                struct_cpp.binary_compatible = false;

                StructFieldCpp field_cpp;
                field_cpp.name = field.name;
                field_cpp.type = "NextInChain";
                // converter
                field_cpp.assign_from_native = std::format(R"(
    this->{0}.setNext(native.{0});)",
                                                           field.name);
                field_cpp.assign_to_cstruct  = std::format(R"(
    cstruct.{0} = this->{0}.getNext();)",
                                                           field.name);
                // setter
                std::string setter_name = field.name;
                setter_name[0]          = std::toupper(setter_name[0]);
                setter_name             = "set" + setter_name;
                struct_cpp.methods_decl.push_back(std::format(R"(
    template <typename T>
    {}& {}(T&& value) &;)",
                                                              struct_cpp.name, setter_name));
                struct_cpp.methods_template_impl.push_back(std::format(R"(
template <typename T>
{0}& {0}::{1}(T&& value) & {{
    this->{2}.setNext(std::forward<T>(value));
    return *this;
}})",
                                                                       struct_cpp.name, setter_name, field.name));
                struct_cpp.methods_decl.push_back(std::format(R"(
    template <typename T>
    {}&& {}(T&& value) &&;)",
                                                              struct_cpp.name, setter_name));
                struct_cpp.methods_template_impl.push_back(std::format(R"(
template <typename T>
{0}&& {0}::{1}(T&& value) && {{
    this->{2}.setNext(std::forward<T>(value));
    return std::move(*this);
}})",
                                                                       struct_cpp.name, setter_name, field.name));

                struct_cpp.fields.push_back(std::move(field_cpp));
                continue;
            }
            // normal field
            StructFieldCpp field_cpp;
            field_cpp.name = field.name;
            field_cpp.type = get_cpp_field_type(field.type, struct_api.owning);
            if (field.is_const) {
                field_cpp.type += " const";
            }
            if (field.is_pointer) {
                field_cpp.type += "*";
            }

            // converter and setter
            field_cpp.assign_from_native = std::format(R"(
    this->{} = static_cast<{}>(native.{});)",
                                                       field.name, field_cpp.type, field.name);
            if (field.type.starts_with("WGPU") &&
                std::ranges::contains(api.structs, field.type.substr(4), [](const StructApi& s) { return s.name; })) {
                if (field.is_pointer)
                    throw std::runtime_error(std::format("Unhandled pointer field {} of wgpu struct in struct {}",
                                                         field.name, struct_cpp.name));
                struct_cpp.binary_compatible &= get_struct_api_cpp(field.type.substr(4), nullptr).binary_compatible;
                // field is struct, update binary compatibility
                if (!get_struct_api_cpp(field.type.substr(4), nullptr).extra_cstruct_members.empty()) {
                    // we have extra data in this field's cstruct, need to store it in cstruct
                    struct_cpp.extra_cstruct_members.push_back(std::format(
                        "{}::CStruct {}_cstruct;", get_cpp_field_type(field.type, struct_api.owning), field.name));
                    field_cpp.assign_to_cstruct = std::format(R"(
    cstruct.{0}_cstruct = this->{0}.to_cstruct();
    cstruct.{0} = static_cast<{1}>(cstruct.{0}_cstruct);)",
                                                              field.name, field.type);
                } else {
                    field_cpp.assign_to_cstruct = std::format(R"(
    cstruct.{0} = static_cast<{1}>(this->{0}.to_cstruct());)",
                                                              field.name, field.type);
                }

                if (field.name == "chain" &&
                    field.type == "WGPUChainedStruct") {  // ChainedStruct should not change its SType field, so we
                                                          // offer setNext instead.
                    std::string setter_name = "setNext";
                    struct_cpp.methods_decl.push_back(std::format(R"(
    template <typename T>
    {}& {}(T&& value) &;)",
                                                                  struct_cpp.name, setter_name));
                    struct_cpp.methods_template_impl.push_back(std::format(R"(
template <typename T>
{0}& {0}::{1}(T&& value) & {{
    this->{2}.setNext(std::forward<T>(value));
    return *this;
}})",
                                                                           struct_cpp.name, setter_name, field.name));
                    struct_cpp.methods_decl.push_back(std::format(R"(
    template <typename T>
    {}&& {}(T&& value) &&;)",
                                                                  struct_cpp.name, setter_name));
                    struct_cpp.methods_template_impl.push_back(std::format(R"(
template <typename T>
{0}&& {0}::{1}(T&& value) && {{
    this->{2}.setNext(std::forward<T>(value));
    return std::move(*this);
}})",
                                                                           struct_cpp.name, setter_name, field.name));
                } else {
                    // struct setter should use const ref and rvalue ref
                    std::string setter_name = field.name;
                    setter_name[0]          = std::toupper(setter_name[0]);
                    setter_name             = "set" + setter_name;
                    // two setter, rvalue and lvalue
                    struct_cpp.methods_decl.push_back(std::format(R"(
    {}& {}(const {}& value) &;)",
                                                                  struct_cpp.name, setter_name, field_cpp.type));
                    struct_cpp.methods_impl.push_back(std::format(R"(
{0}& {0}::{1}(const {2}& value) & {{
    this->{3} = value;
    return *this;
}})",
                                                                  struct_cpp.name, setter_name, field_cpp.type,
                                                                  field.name));

                    struct_cpp.methods_decl.push_back(std::format(R"(
    {}&& {}(const {}& value) &&;)",
                                                                  struct_cpp.name, setter_name, field_cpp.type));
                    struct_cpp.methods_impl.push_back(std::format(R"(
{0}&& {0}::{1}(const {2}& value) && {{
    this->{3} = value;
    return std::move(*this);
}})",
                                                                  struct_cpp.name, setter_name, field_cpp.type,
                                                                  field.name));

                    struct_cpp.methods_decl.push_back(std::format(R"(
    {}& {}({}&& value) &;)",
                                                                  struct_cpp.name, setter_name, field_cpp.type));
                    struct_cpp.methods_impl.push_back(std::format(R"(
{0}& {0}::{1}({2}&& value) & {{
    this->{3} = std::move(value);
    return *this;
}})",
                                                                  struct_cpp.name, setter_name, field_cpp.type,
                                                                  field.name));

                    struct_cpp.methods_decl.push_back(std::format(R"(
    {}&& {}({}&& value) &&;)",
                                                                  struct_cpp.name, setter_name, field_cpp.type));
                    struct_cpp.methods_impl.push_back(std::format(R"(
{0}&& {0}::{1}({2}&& value) && {{
    this->{3} = std::move(value);
    return std::move(*this);
}})",
                                                                  struct_cpp.name, setter_name, field_cpp.type,
                                                                  field.name));
                }
                struct_cpp.fields.push_back(std::move(field_cpp));
                continue;
            }
            // not struct, direct static cast
            field_cpp.assign_to_cstruct = std::format(R"(
    cstruct.{} = static_cast<{}>(this->{});)",
                                                      field.name, field.full_type(), field.name);

            // setter
            std::string setter_name = field.name;
            setter_name[0]          = std::toupper(setter_name[0]);
            setter_name             = "set" + setter_name;
            std::string method_signature =
                std::format("\t{}& {}({} value) &", struct_cpp.name, setter_name, field_cpp.type);
            struct_cpp.methods_decl.push_back(std::format(R"(
    {}& {}({} value) &;)",
                                                          struct_cpp.name, setter_name, field_cpp.type));
            struct_cpp.methods_impl.push_back(std::format(R"(
{0}& {0}::{1}({2} value) & {{
    this->{3} = value;
    return *this;
}})",
                                                          struct_cpp.name, setter_name, field_cpp.type, field.name));

            struct_cpp.methods_decl.push_back(std::format(R"(
    {}&& {}({} value) &&;)",
                                                          struct_cpp.name, setter_name, field_cpp.type));
            struct_cpp.methods_impl.push_back(std::format(R"(
{0}&& {0}::{1}({2} value) && {{
    this->{3} = value;
    return std::move(*this);
}})",
                                                          struct_cpp.name, setter_name, field_cpp.type, field.name));

            struct_cpp.fields.push_back(std::move(field_cpp));
        }
        return struct_cpp_map.emplace(name, std::move(struct_cpp)).first->second;
    };
    for (const auto& struct_api : api.structs) {
        result.structs.push_back(get_struct_api_cpp(struct_api.name, &struct_api));
    }
    // update callback struct param as well, since they need the binary compatible field.
    for (auto& callback_cpp : result.callbacks) {
        for (auto& param : callback_cpp.params) {
            if (param.is_struct) {
                std::string type_without_wgpu = param.type;
                if (type_without_wgpu.starts_with(ns + "::"))
                    type_without_wgpu = type_without_wgpu.substr(ns.size() + 2);
                param.binary_compatible = get_struct_api_cpp(type_without_wgpu, nullptr).binary_compatible;
            }

            std::println(log_file,
                         "Callback param: {} {}, is_handle: {}, is_struct: {}, binary_compatible: {}, is_pointer: {}, "
                         "nullable: {}",
                         param.type, param.name, param.is_handle, param.is_struct, param.binary_compatible,
                         param.is_pointer, param.nullable);
        }
    }

    // functions
    // unlike assign from native, we need write back to cpp struct, mainly for functions
    auto get_assign_to_native = [&](const FuncParamApiCpp& param) -> std::tuple<std::string, std::string, std::string> {
        std::string assign;
        std::string temp_data;
        std::string write_back;
        std::string type_without_wgpu = param.type;
        bool is_wgpu_type             = type_without_wgpu.starts_with(ns + "::");
        if (!is_wgpu_type) return {param.name, "", ""};
        if (is_wgpu_type) {
            type_without_wgpu = type_without_wgpu.substr(ns.size() + 2);
            if (type_without_wgpu.starts_with("raw::")) {
                type_without_wgpu = type_without_wgpu.substr(5);
            }
        }
        bool has_free_members = std::ranges::any_of(
            api.functions, [&](const FuncApi& f) { return f.name == type_without_wgpu + "FreeMembers"; });
        std::string free_members = has_free_members ? std::format("wgpu{}FreeMembers", type_without_wgpu) : "";
        if (param.is_handle) {
            assign = std::format("{}reinterpret_cast<WGPU{}{}*>({}{})", param.is_pointer ? "" : "*", type_without_wgpu,
                                 (param.is_const || !param.is_pointer) ? " const" : "",
                                 param.nullable && param.is_pointer ? "" : "&", param.name);
        } else if (param.is_pointer && param.is_struct) {
            if (param.nullable) {  // cpp full type is also pointer
                if (param.binary_compatible) {
                    assign = std::format("reinterpret_cast<WGPU{}{}*>({})", type_without_wgpu,
                                         param.is_const ? " const" : "", param.name);
                } else if (param.is_const) {
                    temp_data = std::format(
                        R"(
    {1}::CStruct {0}_cstruct;
    if ({0}) {0}_cstruct = {0}->to_cstruct();)",
                        param.name, param.type);
                    assign = std::format("{}? &{}_cstruct : nullptr", param.name, param.name);
                } else {
                    temp_data = std::format(
                        R"(
    WGPU{1} {0}_native;)",
                        param.name, type_without_wgpu);
                    assign = std::format("{}? &{}_native : nullptr", param.name, param.name);
                    if (!param.is_const) {
                        write_back = std::format(
                            R"(
    if ({0}) *{0} = static_cast<{1}>({0}_native);)",
                            param.name, param.type);
                        if (has_free_members) {
                            write_back += std::format(R"(
    if ({0}) {1}({0}_native);)",
                                                      param.name, free_members);
                        }
                    }
                }
            } else {
                if (param.binary_compatible) {
                    assign = std::format("reinterpret_cast<WGPU{}{}*>(&{})", type_without_wgpu,
                                         param.is_const ? " const" : "", param.name);
                } else if (param.is_const) {
                    temp_data = std::format(R"(
    {1}::CStruct {0}_cstruct = {0}.to_cstruct();)",
                                            param.name, param.type);
                    assign    = std::format("&{0}_cstruct", param.name);
                } else {
                    temp_data = std::format(R"(
    WGPU{1} {0}_native;)",
                                            param.name, type_without_wgpu);
                    assign    = std::format("&{0}_native", param.name);
                    if (!param.is_const) {
                        write_back = std::format(R"(
    {0} = static_cast<{1}>({0}_native);)",
                                                 param.name, param.type);
                        if (has_free_members) {
                            write_back += std::format(R"(
    {1}({0}_native);)",
                                                      param.name, free_members);
                        }
                    }
                }
            }
        } else if (param.is_struct) {
            temp_data = std::format(R"(
    {1}::CStruct {0}_cstruct = {0}.to_cstruct();)",
                                    param.name, param.type);
            assign    = std::format("{}_cstruct", param.name);
        } else {  // not handle, not pointer, not struct, maybe enum passed by value, static_cast
            assign = std::format("static_cast<WGPU{}>({})", type_without_wgpu, param.name);
        }
        return {assign, temp_data, write_back};
    };
    for (const auto& func_api : api.functions) {
        if (func_api.name.ends_with("FreeMembers") || func_api.name.ends_with("ProcAddress")) {
            continue;  // skip internal functions for free members and get proc address
        }
        if (!func_api.parent.has_value()) {  // not bind to any handle
            FuncApiCpp func_cpp;
            func_cpp.name                           = func_api.name;
            func_cpp.name[0]                        = std::tolower(func_cpp.name[0]);  // make first letter lower case
            std::vector<FuncParamApiCpp> params_cpp = func_api.params | std::views::transform(get_func_param_cpp_type) |
                                                      std::ranges::to<std::vector<FuncParamApiCpp>>();
            std::string return_type = func_api.return_type;
            if (return_type.starts_with("WGPU")) {
                return_type = ns + "::" + return_type.substr(4);
            }
            bool nullable_overload = false;
            if (!params_cpp.empty() && params_cpp.back().nullable && params_cpp.back().is_pointer) {
                // the last param is a nullable pointer, need two overloads, one be ref, one be nullptr.
                params_cpp.back().nullable = false;  // force as ref. later add overload.
                nullable_overload          = true;
            }
            func_cpp.func_decl    = std::format("{} {}({});", return_type, func_cpp.name,
                                                params_cpp | std::views::transform([](const FuncParamApiCpp& p) {
                                                 return p.full_type() + " " + p.name;
                                             }) | std::views::join_with(std::string(", ")) |
                                                    std::ranges::to<std::string>());
            std::string impl_str1 = R"(
{2} {0}({3}) {{
{4}
    {2} res = static_cast<{2}>(wgpu{1}({5}));
{6}
    return res;
}})";
            std::string impl_str2 = R"(
{2} {0}({3}) {{
{4}
    wgpu{1}({5});
{6}
}})";

            auto arg3 = params_cpp |
                        std::views::transform([](const FuncParamApiCpp& p) { return p.full_type() + " " + p.name; }) |
                        std::views::join_with(std::string(", ")) | std::ranges::to<std::string>();
            auto arg4 = params_cpp | std::views::transform(get_assign_to_native) | std::views::elements<1> |
                        std::views::join_with(std::string("\n")) | std::ranges::to<std::string>();
            auto arg5 = params_cpp | std::views::transform(get_assign_to_native) | std::views::elements<0> |
                        std::views::join_with(std::string(", ")) | std::ranges::to<std::string>();
            auto arg6 = params_cpp | std::views::transform(get_assign_to_native) | std::views::elements<2> |
                        std::views::join_with(std::string("\n")) | std::ranges::to<std::string>();
            func_cpp.func_impl =
                std::vformat(return_type == "void" ? impl_str2 : impl_str1,
                             std::make_format_args(func_cpp.name, func_api.name, return_type, arg3, arg4, arg5, arg6));

            result.functions.push_back(std::move(func_cpp));

            if (nullable_overload) {
                params_cpp.pop_back();  // remove the last param
                FuncApiCpp func_cpp;
                func_cpp.name         = func_api.name;
                func_cpp.name[0]      = std::tolower(func_cpp.name[0]);  // make first letter lower case
                func_cpp.func_decl    = std::format("{} {}({});", return_type, func_cpp.name,
                                                    params_cpp | std::views::transform([](const FuncParamApiCpp& p) {
                                                     return p.full_type() + " " + p.name;
                                                 }) | std::views::join_with(std::string(", ")) |
                                                        std::ranges::to<std::string>());
                std::string impl_str1 = R"(
{2} {0}({3}) {{
{4}
    {2} res = static_cast<{2}>(wgpu{1}({5}{7}));
{6}
    return res;
}})";
                std::string impl_str2 = R"(
{2} {0}({3}) {{
{4}
    wgpu{1}({5}{7});
{6}
}})";

                auto arg3 =
                    params_cpp |
                    std::views::transform([](const FuncParamApiCpp& p) { return p.full_type() + " " + p.name; }) |
                    std::views::join_with(std::string(", ")) | std::ranges::to<std::string>();
                auto arg4 = params_cpp | std::views::transform(get_assign_to_native) | std::views::elements<1> |
                            std::views::join_with(std::string("\n")) | std::ranges::to<std::string>();
                auto arg5 = params_cpp | std::views::transform(get_assign_to_native) | std::views::elements<0> |
                            std::views::join_with(std::string(", ")) | std::ranges::to<std::string>();
                auto arg6 = params_cpp | std::views::transform(get_assign_to_native) | std::views::elements<2> |
                            std::views::join_with(std::string("\n")) | std::ranges::to<std::string>();
                auto arg7          = params_cpp.empty() ? "nullptr" : ", nullptr";
                func_cpp.func_impl = std::vformat(
                    return_type == "void" ? impl_str2 : impl_str1,
                    std::make_format_args(func_cpp.name, func_api.name, return_type, arg3, arg4, arg5, arg6, arg7));

                result.functions.push_back(std::move(func_cpp));
            }
        } else {  // make this a member function.
            auto& handle_cpp      = *std::ranges::find(result.handles, func_api.parent.value(),
                                                       [](const HandleApiCpp& h) { return h.name; });
            std::string func_name = func_api.name.substr(handle_cpp.name.size());  // remove handle prefix
            func_name[0]          = std::tolower(func_name[0]);                    // make first letter lower case
            std::vector<FuncParamApiCpp> params_cpp = func_api.params | std::views::drop(1) |
                                                      std::views::transform(get_func_param_cpp_type) |
                                                      std::ranges::to<std::vector<FuncParamApiCpp>>();
            std::string return_type = func_api.return_type;
            if (return_type.starts_with("WGPU")) {
                return_type = ns + "::" + return_type.substr(4);
            }
            bool nullable_overload = false;
            if (!params_cpp.empty() && params_cpp.back().nullable && params_cpp.back().is_pointer) {
                // the last param is a nullable pointer, need two overloads, one be ref, one be nullptr.
                params_cpp.back().nullable = false;  // force as ref. later add overload.
                nullable_overload          = true;
            }
            handle_cpp.methods_decl.push_back(std::format(
                R"(
    {} {}({}) const;)",
                return_type, func_name,
                params_cpp |
                    std::views::transform([](const FuncParamApiCpp& p) { return p.full_type() + " " + p.name; }) |
                    std::views::join_with(std::string(", ")) | std::ranges::to<std::string>()));

            std::string impl_str1 = R"(
{2} {0}::{1}({3}) const {{
{4}
    {2} res = static_cast<{2}>(wgpu{5}({6}{7}));
{8}
    return res;
}})";
            std::string impl_str2 = R"(
{2} {0}::{1}({3}) const {{
{4}
    wgpu{5}({6}{7});
{8}
}})";

            auto arg3 = params_cpp |
                        std::views::transform([](const FuncParamApiCpp& p) { return p.full_type() + " " + p.name; }) |
                        std::views::join_with(std::string(", ")) | std::ranges::to<std::string>();
            auto arg4 = params_cpp | std::views::transform(get_assign_to_native) | std::views::elements<1> |
                        std::views::join_with(std::string("\n")) | std::ranges::to<std::string>();
            auto arg5 = func_api.name;
            auto arg6 = params_cpp.empty() ? "m_raw" : "m_raw, ";
            auto arg7 = params_cpp | std::views::transform(get_assign_to_native) | std::views::elements<0> |
                        std::views::join_with(std::string(", ")) | std::ranges::to<std::string>();
            auto arg8 = params_cpp | std::views::transform(get_assign_to_native) | std::views::elements<2> |
                        std::views::join_with(std::string("\n")) | std::ranges::to<std::string>();
            handle_cpp.methods_impl.push_back(std::vformat(
                return_type == "void" ? impl_str2 : impl_str1,
                std::make_format_args(handle_cpp.name, func_name, return_type, arg3, arg4, arg5, arg6, arg7, arg8)));

            if (nullable_overload) {
                params_cpp.pop_back();  // remove the last param
                handle_cpp.methods_decl.push_back(std::format(
                    R"(
    {} {}({}) const;)",
                    return_type, func_name,
                    params_cpp |
                        std::views::transform([](const FuncParamApiCpp& p) { return p.full_type() + " " + p.name; }) |
                        std::views::join_with(std::string(", ")) | std::ranges::to<std::string>()));

                std::string impl_str1 = R"(
{2} {0}::{1}({3}) const {{
{4}
    {2} res = static_cast<{2}>(wgpu{5}({6}{7}, nullptr));
{8}
    return res;
}})";
                std::string impl_str2 = R"(
{2} {0}::{1}({3}) const {{
{4}
    wgpu{5}({6}{7}, nullptr);
{8}
}})";

                auto arg3 =
                    params_cpp |
                    std::views::transform([](const FuncParamApiCpp& p) { return p.full_type() + " " + p.name; }) |
                    std::views::join_with(std::string(", ")) | std::ranges::to<std::string>();
                auto arg4 = params_cpp | std::views::transform(get_assign_to_native) | std::views::elements<1> |
                            std::views::join_with(std::string("\n")) | std::ranges::to<std::string>();
                auto arg5 = func_api.name;
                auto arg6 = params_cpp.empty() ? "m_raw" : "m_raw, ";
                auto arg7 = params_cpp | std::views::transform(get_assign_to_native) | std::views::elements<0> |
                            std::views::join_with(std::string(", ")) | std::ranges::to<std::string>();
                auto arg8 = params_cpp | std::views::transform(get_assign_to_native) | std::views::elements<2> |
                            std::views::join_with(std::string("\n")) | std::ranges::to<std::string>();
                handle_cpp.methods_impl.push_back(
                    std::vformat(return_type == "void" ? impl_str2 : impl_str1,
                                 std::make_format_args(handle_cpp.name, func_name, return_type, arg3, arg4, arg5, arg6,
                                                       arg7, arg8)));
            }
        }
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

    // {{type_aliases}}
    std::string type_aliases_text;
    for (const auto& type_alias_api : api_cpp.type_aliases) {
        if (type_alias_api.original.starts_with("WGPU")) {
            continue;  // skip WGPU* to WGPU* aliases
        }
        std::string cpp_type = type_alias_api.alias;
        if (cpp_type.starts_with("WGPU")) {
            cpp_type = cpp_type.substr(4);
        }
        type_aliases_text += std::format("using {} = {};\n", cpp_type, type_alias_api.original);
    }
    output = std::regex_replace(output, std::regex(R"(\{\{type_aliases\}\})"),
                                std::format("namespace {} {{\n{}\n}}", namespace_name, type_aliases_text));

    // {{webgpu_includes}}
    std::string includes_text;
    for (const auto& header : *parser.get_all("--header")) {
        std::string file_name = header | std::views::reverse |
                                std::views::take_while([](char c) { return c != '/' && c != '\\'; }) |
                                std::views::reverse | std::ranges::to<std::string>();
        includes_text += std::format("#include <webgpu/{}>\n", file_name);
    }
    includes_text += "\n#define WEBGPU_CPP_NAMESPACE " + namespace_name + "\n";
    output = std::regex_replace(output, std::regex(R"(\{\{webgpu_includes\}\})"), includes_text);

    // {{enums}}
    std::string enums_text;
    for (const auto& enum_api : api_cpp.enums) {
        enums_text += std::format("enum class {} {{\n", enum_api.flags ? enum_api.name + " : Flags" : enum_api.name);
        for (const auto& entry : enum_api.entries) {
            enums_text += std::format("    {} = {},\n", entry.name, entry.value);
        }
        enums_text += "};\n\n";
    }
    for (const auto& ets : api_cpp.enum_to_string) {
        enums_text += ets.func_decl + "\n";
    }
    for (const auto& enum_api : api_cpp.enums) {
        // bitmask enum operators
        if (enum_api.flags) {
            enums_text += std::format("{} operator|({} lhs, {} rhs);\n", enum_api.name, enum_api.name, enum_api.name);
        }
    }
    output = std::regex_replace(output, std::regex(R"(\{\{enums\}\})"),
                                std::format("namespace {} {{\n{}\n}}", namespace_name, enums_text));

    // {{enums_impl}}
    std::string enums_impl_text;
    for (const auto& ets : api_cpp.enum_to_string) {
        enums_impl_text += ets.func_impl + "\n";
    }
    for (const auto& enum_api : api_cpp.enums) {
        // bitmask enum operators
        if (enum_api.flags) {
            enums_impl_text += std::format(R"(
{0} operator|({0} lhs, {0} rhs) {{
    using T = std::underlying_type_t<{0}>;
    return static_cast<{0}>(static_cast<T>(lhs) | static_cast<T>(rhs));
}})",
                                           enum_api.name);
        }
    }
    output = std::regex_replace(output, std::regex(R"(\{\{enums_impl\}\})"),
                                std::format("namespace {} {{\n{}\n}}", namespace_name, enums_impl_text));

    // {{callbacks_decl}}
    std::string callbacks_decl_text;
    for (const auto& callback_cpp : api_cpp.callbacks) {
        callbacks_decl_text += std::format("struct {};\n", callback_cpp.name);
    }
    output = std::regex_replace(output, std::regex(R"(\{\{callbacks_decl\}\})"),
                                std::format("namespace {} {{\n{}\n}}", namespace_name, callbacks_decl_text));

    // {{callbacks}}
    std::string callbacks_def_text;
    for (const auto& callback_cpp : api_cpp.callbacks) {
        callbacks_def_text += callback_cpp.gen_definition() + "\n\n";
    }
    output = std::regex_replace(output, std::regex(R"(\{\{callbacks\}\})"),
                                std::format("namespace {} {{\n{}\n}}", namespace_name, callbacks_def_text));

    // {{callbacks_template_impl}}
    std::string callbacks_template_impl_text;
    for (const auto& callback_cpp : api_cpp.callbacks) {
        callbacks_template_impl_text += callback_cpp.gen_template_impl() + "\n\n";
    }
    output = std::regex_replace(output, std::regex(R"(\{\{callbacks_template_impl\}\})"),
                                std::format("namespace {} {{\n{}\n}}", namespace_name, callbacks_template_impl_text));

    // {{callbacks_impl}}
    std::string callbacks_impl_text;
    for (const auto& callback_cpp : api_cpp.callbacks) {
        callbacks_impl_text += callback_cpp.gen_impl() + "\n\n";
    }
    output = std::regex_replace(output, std::regex(R"(\{\{callbacks_impl\}\})"),
                                std::format("namespace {} {{\n{}\n}}", namespace_name, callbacks_impl_text));

    // {{structs_decl}}
    std::string structs_decl_text;
    for (const auto& struct_cpp : api_cpp.structs) {
        structs_decl_text += std::format("struct {};\n", struct_cpp.name);
    }
    output = std::regex_replace(output, std::regex(R"(\{\{structs_decl\}\})"),
                                std::format("namespace {} {{\n{}\n}}", namespace_name, structs_decl_text));

    // {{structs}}
    std::string structs_def_text;
    structs_def_text += nextInChainHelper;
    for (auto struct_cpp : api_cpp.structs) {
        for (auto&& [name, injects] : template_meta.injections.members) {
            if (name == struct_cpp.name) {
                struct_cpp.methods_decl.push_back(
                    "\n" + (injects | std::views::join_with(std::string("\n")) | std::ranges::to<std::string>()));
            }
        }
        structs_def_text += struct_cpp.gen_definition() + "\n\n";
    }
    output = std::regex_replace(output, std::regex(R"(\{\{structs\}\})"),
                                std::format("namespace {} {{\n{}\n}}", namespace_name, structs_def_text));

    // {{structs_template_impl}}
    std::string structs_template_impl_text;
    for (const auto& struct_cpp : api_cpp.structs) {
        structs_template_impl_text += struct_cpp.gen_template_impl() + "\n\n";
    }
    output = std::regex_replace(output, std::regex(R"(\{\{structs_template_impl\}\})"),
                                std::format("namespace {} {{\n{}\n}}", namespace_name, structs_template_impl_text));

    // {{structs_impl}}
    std::string structs_impl_text;
    for (const auto& struct_cpp : api_cpp.structs) {
        structs_impl_text += struct_cpp.gen_impl() + "\n\n";
    }
    output = std::regex_replace(output, std::regex(R"(\{\{structs_impl\}\})"),
                                std::format("namespace {} {{\n{}\n}}", namespace_name, structs_impl_text));

    // {{handles_decl}}
    std::string handles_decl_text;
    for (const auto& handle_cpp : api_cpp.handles) {
        handles_decl_text += std::format("class {};\n", handle_cpp.name);
    }
    handles_decl_text = parser.contains("--use-raii")
                            ? std::format("namespace {0}::raw {{\n{1}\n}}\nnamespace {0} {{\n{1}\n}}", namespace_name,
                                          handles_decl_text)
                            : std::format("namespace {0} {{\n{1}\n}}", namespace_name, handles_decl_text);
    output            = std::regex_replace(output, std::regex(R"(\{\{handles_decl\}\})"), handles_decl_text);

    // {{handles}}
    std::string handles_def_text;
    for (auto handle_cpp : api_cpp.handles) {
        for (auto&& [name, injects] : template_meta.injections.members) {
            if (name == handle_cpp.name) {
                handle_cpp.methods_decl.push_back(
                    "\n" + (injects | std::views::join_with(std::string("\n")) | std::ranges::to<std::string>()));
            }
        }
        handles_def_text += handle_cpp.gen_definition() + "\n\n";
    }
    if (parser.contains("--use-raii")) {
        handles_def_text = std::format("namespace {}::raw {{\n{}\n}}\n", namespace_name, handles_def_text);
        std::string raii_def_text;
        std::string raii_friends_text = "#define WEBGPU_RAII_FRIENDS";
        for (const auto& handle_cpp : api_cpp.handles) {
            raii_friends_text += std::format(" \\\n    friend class raw::{};", handle_cpp.name);
        }
        for (const auto& struct_cpp : api.structs) {
            if (struct_cpp.owning) {
                raii_friends_text += std::format(" \\\n    friend struct {};", struct_cpp.name);
            }
        }
        for (const auto& func_cpp : api_cpp.functions) {
            std::string decl = func_cpp.func_decl;
            decl             = std::regex_replace(decl, std::regex("\n"), " ");
            if (func_cpp.func_impl.find("wgpu") != std::string::npos) {
                raii_friends_text += std::format(" \\\n    friend {}", decl);
            }
        }
        for (const auto& handle_cpp : api_cpp.handles) {
            raii_def_text += handle_cpp.gen_raii_definition() + "\n\n";
        }
        handles_def_text += raii_friends_text + "\n" +
                            std::format("namespace {} {{\n{}\n}}", namespace_name, raii_def_text) +
                            "\n#undef WEBGPU_RAII_FRIENDS\n";
    } else {
        handles_def_text = std::format("namespace {} {{\n{}\n}}", namespace_name, handles_def_text);
    }
    output = std::regex_replace(output, std::regex(R"(\{\{handles\}\})"), handles_def_text);

    // {{handles_template_impl}}
    std::string handles_template_impl_text;
    for (const auto& handle_cpp : api_cpp.handles) {
        handles_template_impl_text += handle_cpp.gen_template_impl() + "\n\n";
    }
    handles_template_impl_text =
        parser.contains("--use-raii")
            ? std::format("namespace {}::raw {{\n{}\n}}\n", namespace_name, handles_template_impl_text)
            : std::format("namespace {} {{\n{}\n}}", namespace_name, handles_template_impl_text);
    output = std::regex_replace(output, std::regex(R"(\{\{handles_template_impl\}\})"), handles_template_impl_text);

    // {{handles_impl}}
    std::string handles_impl_text;
    for (const auto& handle_cpp : api_cpp.handles) {
        handles_impl_text += handle_cpp.gen_impl() + "\n\n";
    }
    handles_impl_text = parser.contains("--use-raii")
                            ? std::format("namespace {}::raw {{\n{}\n}}\n", namespace_name, handles_impl_text)
                            : std::format("namespace {} {{\n{}\n}}", namespace_name, handles_impl_text);
    output            = std::regex_replace(output, std::regex(R"(\{\{handles_impl\}\})"), handles_impl_text);

    // {{functions_decl}}
    std::string functions_decl_text;
    for (const auto& func_cpp : api_cpp.functions) {
        functions_decl_text += func_cpp.func_decl + "\n";
    }
    output = std::regex_replace(output, std::regex(R"(\{\{functions_decl\}\})"),
                                std::format("namespace {} {{\n{}\n}}", namespace_name, functions_decl_text));

    // {{functions_template_impl}}
    std::string functions_template_impl_text;
    for (const auto& func_cpp : api_cpp.functions) {
        functions_template_impl_text += func_cpp.func_template_impl + "\n\n";
    }
    output = std::regex_replace(output, std::regex(R"(\{\{functions_template_impl\}\})"),
                                std::format("namespace {} {{\n{}\n}}", namespace_name, functions_template_impl_text));

    // {{functions_impl}}
    std::string functions_impl_text;
    for (const auto& func_cpp : api_cpp.functions) {
        functions_impl_text += func_cpp.func_impl + "\n\n";
    }
    output = std::regex_replace(output, std::regex(R"(\{\{functions_impl\}\})"),
                                std::format("namespace {} {{\n{}\n}}", namespace_name, functions_impl_text));

    output += "#undef WEBGPU_CPP_NAMESPACE";

    // remove all non used binding
    output = std::regex_replace(output, std::regex(R"(\{\{\w+\}\})"), "");
    // remove empty lines
    output = std::regex_replace(output, std::regex(R"((\s*\n\s*){1,}\n)"), "\n");
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
    std::regex typedef_re(R"(typedef +(\w+) +WGPU(\w+)\s*;)");

    for (; !lines.empty(); lines = lines.subspan(1)) {
        std::smatch match;
        if (std::regex_search(lines[0], match, typedef_re)) {
            // Parse typedef
            std::string original_type = match[1];
            std::string alias_name    = match[2];
            std::println(log_file, "Parsing typedef: {} -> {}", alias_name, original_type);
            api.type_aliases.push_back(TypeAliasApi{alias_name, original_type});
        } else if (std::regex_search(lines[0], match, struct_re)) {
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
            api.enums.emplace_back(EnumApi{enum_name, true, {}});
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
                api.enums.emplace_back(EnumApi{enum_name, true, {std::move(entry)}});
            }
        } else if (std::regex_search(lines[0], match, callback_re)) {
            std::println(log_file, "Parsing callback: {}", (std::string)match[1]);
            auto params = parse_params(match[2]);
            api.callbacks.push_back(CallbackApi{match[1], params});
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

    // update function parent
    for (auto& func_api : api.functions) {
        std::size_t max_len = 0;
        for (const auto& handle_api : api.handles) {
            if (func_api.name.starts_with(handle_api.name) && handle_api.name.size() > max_len) {
                max_len         = handle_api.name.size();
                func_api.parent = handle_api.name;
            }
        }
        for (const auto& struct_api : api.structs) {
            if (func_api.name.starts_with(struct_api.name) && struct_api.name.size() > max_len) {
                max_len         = struct_api.name.size();
                func_api.parent = std::nullopt;
            }
        }
        if (func_api.parent.has_value()) {
            std::println(log_file, "Function {} is a method of {}", func_api.name, func_api.parent.value());
        } else {
            std::println(log_file, "Function {} is a non-member function", func_api.name);
        }
    }

    // update owning struct
    for (const auto& func_api : api.functions) {
        if (func_api.return_type.starts_with("WGPU")) {
            std::string possible_struct_name = func_api.return_type.substr(4);
            for (auto& struct_api : api.structs) {
                if (struct_api.name == possible_struct_name) {
                    struct_api.owning = true;
                }
            }
        }
        for (const auto& param : func_api.params) {
            if (param.type.starts_with("WGPU") && param.is_pointer && !param.is_const) {
                std::string possible_struct_name = param.type.substr(4);
                for (auto& struct_api : api.structs) {
                    if (struct_api.name == possible_struct_name) {
                        struct_api.owning = true;
                    }
                }
            }
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
            func_param.type.replace(func_param.type.find('*'), 1, "");
        }
        if (func_param.type.contains("const ") || func_param.type.contains(" const")) {
            func_param.is_const = true;
            func_param.type.replace(func_param.type.find("const"), 5, "");
        }
        if (func_param.type.contains("struct ") || func_param.type.contains(" struct")) {
            func_param.type.replace(func_param.type.find("struct"), 6, "");  // remove struct keyword
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
        api.return_type = strip(api.return_type);
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
                field.type.replace(field.type.find('*'), 1, "");
            }
            if (field.type.contains("const ") || field.type.contains(" const")) {
                field.is_const = true;
                field.type.replace(field.type.find("const"), 5, "");
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
    std::string current_type;
    while (std::getline(file, line)) {
        std::smatch match;
        static std::regex begin_inject_re(R"(\{\{begin_inject\}\})");
        static std::regex end_inject_re(R"(\{\{end_inject\}\})");
        static std::regex inject_typename_re(R"(\s*typename +(\w+):)");
        if (std::regex_search(line, match, begin_inject_re)) {
            current_type.clear();
        } else if (std::regex_search(line, match, end_inject_re)) {
            current_type.clear();
        } else if (std::regex_search(line, match, inject_typename_re)) {
            current_type = match[1];
        } else {
            if (!current_type.empty()) {
                meta.injections.members[current_type].push_back(line);
            } else {
                meta.text += line + "\n";
            }
        }
    }
    std::println(log_file, "Loaded template:\ntext:\n{}\n-----------\ninjections:\n{}\n\n", meta.text,
                 meta.injections.members);
    return meta;
}