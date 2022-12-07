cmake_minimum_required(VERSION 3.10)

# set the project name
project(advent)

set(EXEC_NAME advent)

# set the c standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../../)


set(sources 
    7.cpp
)

# add the executable
add_executable(${EXEC_NAME} ${sources})

target_compile_options(${EXEC_NAME} PRIVATE)

