#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class DiffEngine {
public:
    static void track_file_change(const std::string& file_path);
    static std::vector<std::string> compute_diff(
        const std::string& old_content, 
        const std::string& new_content
    );

private:
    static std::unordered_map<std::string, std::string> file_snapshots;
};