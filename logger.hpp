#pragma once
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include "picofunctions.h"

class Logger
{
   public:
    Logger(const std::string &filename, bool output_format);
    ~Logger();

    void
    logInfo(const std::string &message);
    void
    logError(const std::string &message);
    void
    logTimebase(const std::string &message, uint32_t timebase);
    void
    log_output(const uint32_t &reval_open);

   private:
    std::fstream log_file;
    bool output_format;
    void
    log(const std::string &level, const std::string &message);
};
