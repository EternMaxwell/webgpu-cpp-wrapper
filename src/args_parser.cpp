#include <iostream>
#include <print>

#include "wcpp/args_parser.hpp"

ArgsParser::ArgsParser(std::string program_description) {
    this->program_description = std::move(program_description);
    add_arg_spec(ArgSpec(std::array{"--help", "-h"}, "show help message and exit").set_flag(true));
}

void ArgsParser::add_flag(const std::string& flag, std::string description) {
    if (flag_index.contains(flag)) {
        std::println(std::cout, "Warning: flag '{}' is already registered.", flag);
        return;
    }
    flag_index[flag] = arg_specs.size();
    arg_specs.emplace_back(flag, std::move(description)).set_flag(true);
    parsed_args.emplace_back();
}
void ArgsParser::add_option(const std::string& option, std::string default_value, std::string description) {
    if (flag_index.contains(option)) {
        std::println(std::cout, "Warning: option '{}' is already registered.", option);
        return;
    }
    ArgSpec spec(option, std::move(description));
    if (!default_value.empty()) {
        spec.add_default_value(std::move(default_value));
    }
    flag_index[option] = arg_specs.size();
    arg_specs.emplace_back(std::move(spec.set_flag(false)));
    parsed_args.emplace_back();
}
void ArgsParser::add_arg_spec(ArgSpec spec) {
    for (const auto& flag : spec.names) {
        if (flag_index.contains(flag)) {
            std::println(std::cout, "Warning: flag/option '{}' is already registered.", flag);
            return;
        }
    }
    for (const auto& flag : spec.names) {
        flag_index[flag] = arg_specs.size();
    }
    arg_specs.emplace_back(std::move(spec));
    parsed_args.emplace_back();
}

bool ArgsParser::contains(const std::string& flag) const {
    auto it = flag_index.find(flag);
    if (it == flag_index.end()) {
        return false;
    }
    return parsed_args[it->second].contained;
}
std::optional<std::string_view> ArgsParser::get_single(const std::string& option) const {
    auto it = flag_index.find(option);
    if (it == flag_index.end()) {
        return std::nullopt;
    }
    if (arg_specs[it->second].is_flag) {
        return std::nullopt;
    }
    const ParsedArg& parsed_arg = parsed_args[it->second];
    if (!parsed_arg.contained || parsed_arg.values.empty()) {
        return std::nullopt;
    }
    return parsed_arg.values.front();
}
std::optional<std::ranges::ref_view<const std::vector<std::string>>> ArgsParser::get_all(
    const std::string& option) const {
    auto it = flag_index.find(option);
    if (it == flag_index.end()) {
        return std::nullopt;
    }
    if (arg_specs[it->second].is_flag) {
        return std::nullopt;
    }
    const ParsedArg& parsed_arg = parsed_args[it->second];
    return parsed_arg.values;
}

void ArgsParser::print_help() const {
    std::println(std::cout, "{}", program_description);
    std::println(std::cout, "Available options:");
    for (const auto& spec : arg_specs) {
        std::string flags_combined;
        for (const auto& flag : spec.names) {
            if (!flags_combined.empty()) {
                flags_combined += ", ";
            }
            flags_combined += flag;
        }
        if (spec.is_flag) {
            std::println(std::cout, "  {:<20} {}", flags_combined, spec.description);
        } else {
            std::string default_str;
            if (!spec.default_value.empty()) {
                default_str = " [default: ";
                for (const auto& def_val : spec.default_value) {
                    if (default_str.size() > 11) {
                        default_str += ", ";
                    }
                    default_str += def_val;
                }
                default_str += "]";
            }
            std::println(std::cout, "  {:<20} {}{}", flags_combined, spec.description, default_str);
        }
    }
}

void ArgsParser::parse(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        auto it         = flag_index.find(arg);
        if (it == flag_index.end()) {
            std::println(std::cout, "Warning: Unrecognized argument '{}'", arg);
            std::exit(1);
        }
        if (it->second == 0) {
            print_help();
            std::exit(0);
        }
        ParsedArg& parsed_arg = parsed_args[it->second];
        const ArgSpec& spec   = arg_specs[it->second];
        parsed_arg.contained  = true;
        if (spec.is_flag) {
            // Flag, no value to parse
            continue;
        } else {
            // Option, parse the next argument as value
            if (i + 1 >= argc) {
                std::println(std::cout, "Error: Expected value after option '{}'", arg);
                std::exit(1);
            }
            ++i;
            parsed_arg.values.push_back(argv[i]);
        }
    }

    // Fill in default values for options not provided
    for (std::size_t index = 0; index < arg_specs.size(); ++index) {
        const ArgSpec& spec   = arg_specs[index];
        ParsedArg& parsed_arg = parsed_args[index];
        if (!parsed_arg.contained && !spec.is_flag && !spec.default_value.empty()) {
            parsed_arg.contained = true;
            parsed_arg.values    = spec.default_value;
        }
    }
}