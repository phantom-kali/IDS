#include "file_watcher.h"
#include "logger.h"
#include "diff_engine.h"
#include <unistd.h>
#include <limits.h>

FileWatcher::FileWatcher(const std::vector<std::string>& directories, bool recursive)
    : watched_directories(directories), recursive(recursive) {
    inotify_fd = inotify_init();
    if (inotify_fd == -1) {
        throw std::runtime_error("Could not initialize inotify");
    }

    for (const auto& dir : watched_directories) {
        add_watch_recursive(std::filesystem::path(dir));
    }
}

FileWatcher::~FileWatcher() {
    stop_watching();
    close(inotify_fd);
}

void FileWatcher::add_watch_recursive(const std::filesystem::path& path) {
    int wd = inotify_add_watch(inotify_fd, path.string().c_str(), 
        IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVE);
    
    if (wd == -1) {
        Logger::error("Could not watch directory: " + path.string());
        return;
    }

    watch_descriptors[wd] = path.string();

    if (recursive) {
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            if (entry.is_directory()) {
                add_watch_recursive(entry.path());
            }
        }
    }
}

void FileWatcher::start_watching() {
    const int EVENT_SIZE = sizeof(struct inotify_event);
    const int BUF_LEN = 1024 * (EVENT_SIZE + 16);
    char buffer[BUF_LEN];

    while (true) {
        int length = read(inotify_fd, buffer, BUF_LEN);
        if (length < 0) {
            Logger::error("Error reading inotify events");
            break;
        }

        for (int i = 0; i < length;) {
            struct inotify_event* event = (struct inotify_event*)&buffer[i];
            
            if (event->mask & IN_CREATE) {
                Logger::info("File created: " + std::string(event->name));
            }
            
            if (event->mask & IN_MODIFY) {
                std::string full_path = watch_descriptors[event->wd] + "/" + event->name;
                DiffEngine::track_file_change(full_path);
            }
            
            i += EVENT_SIZE + event->len;
        }
    }
}

void FileWatcher::stop_watching() {
    for (const auto& [wd, path] : watch_descriptors) {
        inotify_rm_watch(inotify_fd, wd);
    }
    close(inotify_fd);
}

