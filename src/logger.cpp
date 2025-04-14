#include "logger.hpp"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <cstdlib>

Logger::Logger(const std::string &filename, bool console_output)
    : console_output(console_output),
      min_severity(LogSeverity::Trace)
{
    if (!console_output)
    {
        log_file.open(filename, std::ios::out);
        if (!log_file.is_open())
        {
            std::cerr << "Failed to open log file." << std::endl;
            std::exit(-1);
        }
    }
}

Logger::~Logger()
{
    if (log_file.is_open())
    {
        log_file.close();
    }
}

void Logger::setMinimumSeverity(LogSeverity severity)
{
    min_severity = severity;
}

void Logger::logInfo(const std::string &message)
{
    log(LogSeverity::Info, message);
}

void Logger::logError(const std::string &message)
{
    log(LogSeverity::Error, message);
    std::cerr << "ERROR while running program, check logs for Pico status" << std::endl;
    std::exit(-1);
}

void Logger::logTimebase(const std::string &message, uint32_t timebase)
{
    std::string new_message = message + std::to_string(timebase);
    log(LogSeverity::Info, new_message);
}

void Logger::log_output(const uint32_t &reval_open)
{
    std::string pico_state = return_fun(reval_open);
    if (pico_state == "PICO_OK")
    {
        logInfo(pico_state);
    }
    else
    {
        logError(pico_state);
    }
}

void Logger::log(LogSeverity severity, const std::string &message)
{
    if (severity < min_severity)
    {
        return;
    }
    std::stringstream ss;
    ss << getTimestamp() << " [";
    switch (severity)
    {
    case LogSeverity::Trace:
        ss << "TRACE";
        break;
    case LogSeverity::Debug:
        ss << "DEBUG";
        break;
    case LogSeverity::Info:
        ss << "INFO";
        break;
    case LogSeverity::Warning:
        ss << "WARNING";
        break;
    case LogSeverity::Error:
        ss << "ERROR";
        break;
    }
    ss << "]: " << message;
    writeLog(severity, ss.str());
}

void Logger::writeLog(LogSeverity severity, const std::string &formatted_message)
{
    if (console_output)
    {
        std::cout << formatted_message << std::endl;
    }
    else
    {
        if (log_file.is_open())
        {
            log_file << formatted_message << std::endl;
        }
    }
}

std::string Logger::getTimestamp() const
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm = *std::localtime(&now_c);
    std::stringstream time_stream;
    time_stream << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S");
    return time_stream.str();
}
