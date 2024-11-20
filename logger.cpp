#include "logger.hpp"

Logger::Logger(const std::string& filename)
{
    log_file.open(filename, std::ios::app);
    if (!log_file.is_open())
    {
        std::cerr << "Failed to open log file." << std::endl;
    }
}

Logger::~Logger()
{
    if (log_file.is_open())
    {
        log_file.close();
    }
}

void
Logger::logInfo(const std::string& message)
{
    log("INFO", message);
}

void
Logger::logError(const std::string& message)
{
    log("ERROR", message);
}

void
Logger::logTimebase(const std::string& message, uint32_t timebase)
{
    const std::string new_message = message + std::to_string(timebase);
    log("INFO", new_message);
}

void
Logger::log_output(const uint32_t& reval_open)
{
    std::string pico_state = return_fun(reval_open);
    pico_state == "PICO_OK" ? logInfo(pico_state) : logError(pico_state);
}

void
Logger::log(const std::string& level, const std::string& message)
{
    log_file.open("logfile.log", std::ios::app);
    if (log_file.is_open())
    {
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        log_file << std::ctime(&now_c) << "[" << level << "]: " << message << std::endl;
    }
}