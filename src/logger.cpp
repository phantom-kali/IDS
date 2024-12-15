#include "logger.h"

std::ofstream Logger::log_file("/var/log/file_ids.log", std::ios::app);

void Logger::log(LogLevel level, const std::string& message) {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    
    std::stringstream log_message;
    log_message << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S") 
                << " | ";

    switch (level) {
        case INFO: log_message << "INFO: "; break;
        case WARNING: log_message << "WARNING: "; break;
        case ERROR: log_message << "ERROR: "; break;
    }

    log_message << message;

    // Write to file log
    log_file << log_message.str() << std::endl;

    // Also send to syslog
    openlog("FileIDS", LOG_PID, LOG_LOCAL0);
    switch (level) {
        case INFO: syslog(LOG_INFO, "%s", log_message.str().c_str()); break;
        case WARNING: syslog(LOG_WARNING, "%s", log_message.str().c_str()); break;
        case ERROR: syslog(LOG_ERR, "%s", log_message.str().c_str()); break;
    }
    closelog();
}

void Logger::info(const std::string& message) {
    log(INFO, message);
}

void Logger::warning(const std::string& message) {
    log(WARNING, message);
}

void Logger::error(const std::string& message) {
    log(ERROR, message);
}
