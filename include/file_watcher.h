#pragma once

#include <sys/inotify.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <filesystem>

class FileWatcher {
public:
    FileWatcher(const std::vector<std::string>& directories, bool recursive = true);
    ~FileWatcher();

    void start_watching();
    void stop_watching();

private:
    int inotify_fd;
    std::unordered_map<int, std::string> watch_descriptors;
    std::vector<std::string> watched_directories;
    bool recursive;

    void add_watch_recursive(const std::filesystem::path& path);
    void process_events();
    void handle_file_event(const std::string& path, uint32_t mask);
};