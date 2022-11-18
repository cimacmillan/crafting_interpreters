cmake_minimum_required(VERSION 3.10)

# set the project name
project(clox)

set(EXEC_NAME clox)

# set the c standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../../build)


set(sources 
    clox.c
)

# add the executable
add_executable(${EXEC_NAME} ${sources})

