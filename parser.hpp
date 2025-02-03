#pragma once
#include <vector>
#include <string>
#include <algorithm>
class Parser {
public:
    Parser(int argc, char** argv);
    ~Parser() = default;

    bool cmdOption(const std::string& option);
   const std::string& setFilename(const std::string& option, const int32_t points, const uint32_t number_channels, const uint32_t freq) ;


private:
        std::vector<std::string> flags;
};
