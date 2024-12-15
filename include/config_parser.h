#pragma once

#include <string>
#include <vector>
#include <json/json.h>

class ConfigParser {
public:
    static bool load_config(const std::string& config_path);
    static std::vector<std::string> get_watched_directories();
    static std::string get_log_path();
    static bool is_recursive_watch_enabled();

private:
    static Json::Value config_root;
};