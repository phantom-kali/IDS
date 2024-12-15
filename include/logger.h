#pragma once

#include <string>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <syslog.h>

class Logger {
public:
    enum LogLevel { INFO, WARNING, ERROR };

    static void info(const std::string& message);
    static void warning(const std::string& message);
    static void error(const std::string& message);

private:
    static void log(LogLevel level, const std::string& message);
    static std::ofstream log_file;
};