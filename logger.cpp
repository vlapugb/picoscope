#include "logger.hpp"

Logger::Logger(const std::string& filename, bool output_format) : output_format(output_format)
{
    if(!output_format) {
        log_file.open(filename, std::ios::app);

        if (!log_file.is_open())
        {
            std::cerr << "Failed to open log file." << std::endl;
            exit(-1);
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

void
Logger::logInfo(const std::string& message)
{
    log("INFO", message);
}

void
Logger::logError(const std::string& message)
{
    log("ERROR", message);
    fprintf(stderr, "ERROR while running programm, check logs for Pico status");
    exit(-1);
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
    time_t now = time(0);
    char* dt = ctime(&now);
    if(output_format == true) {
        printf( "%s [%s]: %s\n", dt,level.c_str(),message.c_str());
    } else {
        if (log_file.is_open()) {
            log_file << dt << "[" << level << "]: " << message << std::endl;
        }
    }




}
