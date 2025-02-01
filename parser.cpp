#include "parser.hpp"

Parser::Parser (int argc, char** argv) {
    for(int i = 0; i < argc; i++) {
        flags.push_back(argv[i]);
    }

}

bool Parser::cmdOption (const std::string& option) {
    return std::find(flags.begin(), flags.end(), option) != flags.end();
}

const std::string& Parser::setFilename(const std::string& option, const int32_t points, const uint32_t number_channels, const uint32_t freq) {
    if(cmdOption(option)) {
        auto it = std::find(flags.begin(), flags.end(), option) - flags.begin();
        if(it - 1 == flags.size()) {
            return std::to_string(points) + std::to_string(number_channels) + std::to_string(freq);
        }
        else {
            return flags[it + 1];
        }
    }

}

