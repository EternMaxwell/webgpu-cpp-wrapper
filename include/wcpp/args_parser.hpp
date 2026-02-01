#pragma once

#include <array>
#include <charconv>
#include <optional>
#include <ranges>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct ArgsParser {
   public:
    struct ArgSpec {
       public:
        ArgSpec(std::string name, std::string description)
            : names{std::move(name)}, description(std::move(description)) {}
        ArgSpec(std::ranges::range auto&& names, std::string description)
            requires std::convertible_to<std::ranges::range_value_t<decltype(names)>, std::string>
            : names(std::ranges::to<std::unordered_set<std::string>>(names)), description(std::move(description)) {}
        ArgSpec& set_flag(bool is_flag = true) {
            this->is_flag = is_flag;
            return *this;
        }
        ArgSpec& set_description(std::string description) {
            this->description = std::move(description);
            return *this;
        }
        ArgSpec& add_name(std::string name) {
            this->names.emplace(std::move(name));
            return *this;
        }
        ArgSpec& add_default_value(std::string default_value) {
            this->set_flag(false);
            this->default_value.emplace_back(std::move(default_value));
            return *this;
        }

       private:
        std::unordered_set<std::string> names;   // e.g., {"-h", "--help"}
        std::string description;                 // description for help message
        std::vector<std::string> default_value;  // default value for options
        bool is_flag = false;                    // true if it's a flag, false if it's an option

        friend struct ArgsParser;
    };

    ArgsParser(std::string program_description = "");

    void add_flag(const std::string& name, std::string description = "");
    void add_option(const std::string& name, std::string default_value, std::string description = "");
    void add_arg_spec(ArgSpec spec);

    bool contains(const std::string& name) const;
    std::optional<std::string_view> get_single(const std::string& name) const;
    std::optional<std::ranges::ref_view<const std::vector<std::string>>> get_all(const std::string& name) const;
    template <typename T>
    std::optional<T> get_single_as(const std::string& name) const {
        auto str_opt = get_single(name);
        if (!str_opt.has_value()) {
            return std::nullopt;
        }
        T value;
        auto [ptr, ec] = std::from_chars(str_opt->data(), str_opt->data() + str_opt->size(), value);
        if (ec != std::errc()) {
            return std::nullopt;
        }
        return value;
    }
    template <typename T>
    auto get_all_as(const std::string& name) const {
        static auto transform = [](const std::string& s) -> std::optional<T> {
            T value;
            auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value);
            if (ec != std::errc()) {
                return std::nullopt;
            }
            return value;
        };
        using type = decltype(std::declval<const std::vector<std::string>&>() | std::views::transform(transform));
        auto it    = flag_index.find(name);
        if (it == flag_index.end()) {
            return std::optional<type>{std::nullopt};
        }
        if (arg_specs[it->second].is_flag) {
            return std::optional<type>{std::nullopt};
        }
        const ParsedArg& parsed_arg = parsed_args[it->second];
        return std::make_optional(parsed_arg.values | std::views::transform(transform));
    }

    void print_help() const;
    void parse(int argc, char** argv);

   private:
    struct ParsedArg {
       public:
        bool contained = false;           // true if the flag/option was provided
        std::vector<std::string> values;  // use a vector to support multiple values for the same option
    };

    std::string program_description;
    std::unordered_map<std::string, std::size_t> flag_index;
    std::vector<ArgSpec> arg_specs;
    std::vector<ParsedArg> parsed_args;
};