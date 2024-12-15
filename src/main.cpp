#include "config_parser.h"
#include "file_watcher.h"
#include "logger.h"
#include <iostream>
#include <signal.h>

FileWatcher* global_watcher = nullptr;

void signal_handler(int signal) {
    if (global_watcher) {
        Logger::info("Received signal to stop watching.");
        global_watcher->stop_watching();
        exit(0);
    }
}

int main(int argc, char* argv[]) {
    const std::string config_path = argc > 1 ? argv[1] : "/etc/file_ids/config.json";

    try {
        if (!ConfigParser::load_config(config_path)) {
            Logger::error("Could not load configuration file");
            return 1;
        }

        auto directories = ConfigParser::get_watched_directories();
        bool recursive = ConfigParser::is_recursive_watch_enabled();

        signal(SIGINT, signal_handler);
        signal(SIGTERM, signal_handler);

        FileWatcher watcher(directories, recursive);
        global_watcher = &watcher;

        Logger::info("File IDS started. Watching " + std::to_string(directories.size()) + " directories.");
        
        watcher.start_watching();
    }
    catch (const std::exception& e) {
        Logger::error("Fatal error: " + std::string(e.what()));
        return 1;
    }

    return 0;
}
