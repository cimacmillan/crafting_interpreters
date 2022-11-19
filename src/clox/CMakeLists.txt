cmake_minimum_required(VERSION 3.10)

# set the project name
project(clox)

set(EXEC_NAME clox)

# set the c standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../../)


set(sources 
    clox.c
    chunk.c
    debug.c
    memory.c
    value.c
    vm.c
)

# add the executable
add_executable(${EXEC_NAME} ${sources})

target_compile_options(${EXEC_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)

