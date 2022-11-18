cmake_minimum_required(VERSION 3.10)

# set the project name
project(lox)

set(EXEC_NAME lox)

# set the cpp standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../../)

set(sources 
    FileUtils.cpp
    CPPLox.cpp
    Environment.cpp
    Expr.cpp
    Interpreter.cpp
    LoxParser.cpp
    LoxValue.cpp
    Token.cpp
    TokenScanner.cpp
    LoxFunction.cpp
    LoxInstance.cpp
    LoxClass.cpp
    Analyzer.cpp
)

#Add better enums
include_directories("../../libs/better-enums")

# add the executable
add_executable(${EXEC_NAME} 
    ${sources} Main.cpp
)

target_compile_options(${EXEC_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)

