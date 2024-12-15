# IDS

IDS is a file integrity monitoring system that tracks changes in specified directories and logs any detected modifications. It uses inotify for file system event monitoring and a simple diff algorithm to detect changes in file contents.

## Features

- Monitors specified directories for file changes
- Logs detected changes to a specified log file
- Supports recursive directory watching
- Configurable through a JSON configuration file

## Requirements

- C++17
- CMake 3.10 or higher
- Threads library
- JSONCPP library

## Installation

1. Clone the repository:
    ```sh
    git clone https://github.com/phantom-kali/IDS.git
    cd IDS
    ```

2. Create a build directory and navigate to it:
    ```sh
    mkdir build
    cd build
    ```

3. Run CMake to configure the project:
    ```sh
    cmake ..
    ```

4. Build the project:
    ```sh
    make
    ```

5. Install the executable and configuration file:
    ```sh
    sudo make install
    ```

## Configuration

The configuration file `config.json` should be placed in `/etc/file_ids`. It contains the following settings:

- `watched_directories`: List of directories to monitor.
- `log_path`: Path to the log file.
- `recursive_watch`: Boolean to enable or disable recursive directory watching.

Example `config.json`:
```json
{
    "watched_directories": [
        "/home/user/sensitive",
        "/etc"
    ],
    "log_path": "/var/log/file_ids.log",
    "recursive_watch": true
}
```

## Usage
Run the file_ids executable to start monitoring the specified directories:

## Logging
The application logs detected changes to the specified log file. Log messages include information about the type of change (added, removed, modified) and the affected file.