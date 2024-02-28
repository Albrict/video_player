cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(video_player)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/externals/sanitizers-cmake/cmake" ${CMAKE_MODULE_PATH})
set (CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined")

# Dependencies
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED

# Our Project
include(src/src.cmake)
add_executable(${PROJECT_NAME} ${source_code})
target_link_libraries(${PROJECT_NAME} raylib)
