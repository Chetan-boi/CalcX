#pragma once

#include <cctype>
#include <optional>
#include <ostream>
#include <string>
#include <vector>

std::vector<std::string> parseCommand(const std::string&);
std::optional<std::vector<std::string>> getInput(bool isError = false);
void printHelp();
void printVersion();

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vec) {
    for (const auto& i : vec) {
        out << i << '\n';
    }
    return out;
}
