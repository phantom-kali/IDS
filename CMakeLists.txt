cmake_minimum_required(VERSION 3.10)
project(FileIDS)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)

# Add executable
add_executable(file_ids 
    src/main.cpp
    src/file_watcher.cpp
    src/diff_engine.cpp
    src/logger.cpp
    src/config_parser.cpp
)

# Include directories
target_include_directories(file_ids PRIVATE 
    include
    ${JSONCPP_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(file_ids 
    Threads::Threads
    ${JSONCPP_LIBRARIES}
)

# Optional: Install configuration
install(TARGETS file_ids DESTINATION bin)
install(FILES config.json DESTINATION /etc/file_ids)