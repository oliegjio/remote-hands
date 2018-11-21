#pragma once

#include <string>
#include <regex>

std::vector<std::string> split(const std::string& text, const std::regex& re) {
    const std::vector<std::string> parts(
            std::sregex_token_iterator(text.begin(), text.end(), re, -1),
            std::sregex_token_iterator());
    return parts;
}

std::vector<std::string> split_by_spaces(const std::string& text) {
    return split(text, std::regex("\\s+"));
}

static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}