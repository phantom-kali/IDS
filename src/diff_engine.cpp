#include "diff_engine.h"
#include "logger.h"
#include <fstream>
#include <sstream>
#include <algorithm>

std::unordered_map<std::string, std::string> DiffEngine::file_snapshots;

void DiffEngine::track_file_change(const std::string& file_path) {
    std::ifstream file(file_path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    
    std::string new_content = buffer.str();
    
    // First time seeing the file
    if (file_snapshots.find(file_path) == file_snapshots.end()) {
        file_snapshots[file_path] = new_content;
        return;
    }

    auto diff_lines = compute_diff(file_snapshots[file_path], new_content);
    
    // Log the changes
    if (!diff_lines.empty()) {
        Logger::info("Changes detected in " + file_path);
        for (const auto& line : diff_lines) {
            Logger::info(line);
        }
    }

    // Update snapshot
    file_snapshots[file_path] = new_content;
}

std::vector<std::string> DiffEngine::compute_diff(
    const std::string& old_content, 
    const std::string& new_content
) {
    std::vector<std::string> old_lines, new_lines, diff_result;
    std::istringstream old_stream(old_content);
    std::istringstream new_stream(new_content);
    std::string line;

    // Split into lines
    while (std::getline(old_stream, line)) {
        old_lines.push_back(line);
    }
    while (std::getline(new_stream, line)) {
        new_lines.push_back(line);
    }

    // Simple diff algorithm
    size_t i = 0, j = 0;
    while (i < old_lines.size() && j < new_lines.size()) {
        if (old_lines[i] != new_lines[j]) {
            // Line removed
            if (j < new_lines.size() && 
                std::find(old_lines.begin() + i + 1, old_lines.end(), new_lines[j]) != old_lines.end()) {
                diff_result.push_back("- " + old_lines[i]);
                i++;
            }
            // Line added
            else {
                diff_result.push_back("+ " + new_lines[j]);
                j++;
            }
        } else {
            // Unchanged line
            i++;
            j++;
        }
    }

    // Handle remaining lines
    while (i < old_lines.size()) {
        diff_result.push_back("- " + old_lines[i]);
        i++;
    }
    while (j < new_lines.size()) {
        diff_result.push_back("+ " + new_lines[j]);
        j++;
    }

    return diff_result;
}
