project(callox)

cmake_minimum_required(VERSION 3.24)

set(PROJECT_SOURCE_DIR, src)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY build)
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(src/cpplox)
add_subdirectory(src/clox)
add_subdirectory(src/advent)

