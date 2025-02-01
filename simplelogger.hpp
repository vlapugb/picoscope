#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <sstream>
#include <fstream>
#include <sstream>
#include <fstream>

#if _WIN32

#include <windows.h>
#include <winbase.h>

#endif // _WIN32

#if defined(_LINUX)

#include <sys/stat.h>
#include <unistd.h>

#endif // defined

#define ERROR -1 // Logger flags
#define DEBUG_ERROR 6
#define WARN 1
#define INFO 0
#define OK 4
#define DEBUG 7
#define DONE 10
#define NO_LOG_STRING 2

#ifndef SIMPLELOGGER_HPP
#define SIMPLELOGGER_HPP

namespace LogPref
{
class Flag {
public:
    int flag;
    Flag(int flag) {this->flag = flag;}
    Flag() {this->flag = INFO;}
};
}

class SimpleLogger {

protected:
    std::ostream& log_stream;
    std::ofstream file_stream;

    std::string log_name;
    std::string log_name_prefix;
    std::string log_string;
    LogPref::Flag log_flag;
    bool console_enabled;

    std::string generateFileName();
    std::string generateLogString(int flag);

public:
    SimpleLogger(std::ostream& ls, std::string prefix, std::string dir);
    SimpleLogger(std::ostream& ls, std::string prefix);
    SimpleLogger(std::ostream& ls);
    SimpleLogger(const SimpleLogger& other);
    void initializeLogger(std::string filename, std::string prefix);
    void closeLogger();
    void enableConsoleOutput(bool enable_console);
    void setLoggerFlag(LogPref::Flag log_flag);
    ~SimpleLogger();

    template<typename T>
    SimpleLogger operator<<(T val) {
    if (console_enabled) {
        std::cout << (this->log_flag.flag == NO_LOG_STRING ? "" : generateLogString(this->log_flag.flag)) << val;
        std::cout << std::forward<T>(val);
    }
    log_stream << (this->log_flag.flag == NO_LOG_STRING ? "" : generateLogString(this->log_flag.flag)) << val;
    return *this << LogPref::Flag(NO_LOG_STRING);
    }

    SimpleLogger operator<<(LogPref::Flag flag);
    SimpleLogger operator<<(std::ostream& (*func)(std::ostream&));
};

#endif // SIMPLELOGGER_HPP
