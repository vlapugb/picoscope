#pragma once
#include <string>
#include <fstream>
#include <mutex>
#include <cstdint>
#include "picofunctions.h"

enum class LogSeverity {
    Trace,
    Debug,
    Info,
    Warning,
    Error
};

class Logger {
public:
    Logger(const std::string &filename, bool console_output);
    ~Logger();
    void setMinimumSeverity(LogSeverity severity);
    void logInfo(const std::string &message);
    void logError(const std::string &message);
    void logTimebase(const std::string &message, uint32_t timebase);
    void log_output(const uint32_t &reval_open);

private:
    std::ofstream log_file;
    bool console_output;
    LogSeverity min_severity;
    std::mutex log_mutex;

    void log(LogSeverity severity, const std::string &message);
    void writeLog(LogSeverity severity, const std::string &formatted_message);
    std::string getTimestamp() const;
};
