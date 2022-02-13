cmake_minimum_required(VERSION 3.10)

# set the project name
project(CPlayground)

# set the cpp standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(PROJECT_SOURCE_DIR, src)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY build)


# include_directories(${PROJECT_SOURCE_DIR})

file(GLOB sources
     "src/*.h"
     "src/*.cpp"
     "src/**/*.h"
     "src/**/*.cpp"
)

# add the executable
add_executable(CPlayground 
    ${sources}
)