module;

{{webgpu_includes}}

#include <iostream>
#include <vector>
#include <functional>
#include <cassert>
#include <concepts>
#include <cmath>
#include <memory>
#include <string_view>
#include <span>

export module webgpu;

{{begin_inject}}
typename StringView:
    StringView(const std::string_view& sv) : data(sv.data()), length(sv.size()) {}
    StringView(const char* str) : data(str), length(WGPU_STRLEN) {}
    operator std::string_view() const {
        return length == WGPU_STRLEN ? std::string_view(data) : std::string_view(data, length);
    }
{{end_inject}}

export {

{{enums}}
{{enums_to_string_decl}}

{{structs_decl}}
{{handles_decl}}
// Non member functions
{{functions_decl}}

{{handles}}
{{handles_raii}}
{{structs}}
{{callbacks}}

}

{{structs_template_impl}}
{{handle_template_impl}}

{{enums_to_string_impl}}
{{structs_impl}}
{{functions_impl}}