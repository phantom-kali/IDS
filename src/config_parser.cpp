#include "config_parser.h"
#include "logger.h"  
#include <fstream>

Json::Value ConfigParser::config_root;

bool ConfigParser::load_config(const std::string& config_path) {
    std::ifstream config_file(config_path);
    Json::Reader reader;
    
    if (!reader.parse(config_file, config_root)) {
        Logger::error("Failed to parse configuration file");
        return false;
    }

    return true;
}

std::vector<std::string> ConfigParser::get_watched_directories() {
    std::vector<std::string> directories;
    
    if (config_root.isMember("watched_directories")) {
        for (const auto& dir : config_root["watched_directories"]) {
            directories.push_back(dir.asString());
        }
    }

    return directories;
}

std::string ConfigParser::get_log_path() {
    return config_root.get("log_path", "/var/log/file_ids.log").asString();
}

bool ConfigParser::is_recursive_watch_enabled() {
    return config_root.get("recursive_watch", true).asBool();
}