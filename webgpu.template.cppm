module;

{{webgpu_includes}}

#include <atomic>
#include <iostream>
#include <vector>
#include <functional>
#include <cassert>
#include <concepts>
#include <cmath>
#include <memory>
#include <string_view>
#include <span>
#include <optional>
#include <ranges>

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

{{type_aliases}}

{{enums}}

{{structs_decl}}
{{handles_decl}}
{{callbacks_decl}}

{{handles}}
{{callbacks}}
{{structs}}

// Non member functions
{{functions_decl}}

}

{{structs_template_impl}}
{{handles_template_impl}}
{{callbacks_template_impl}}

{{enums_impl}}
{{structs_impl}}
{{handles_impl}}
{{callbacks_impl}}
{{functions_impl}}