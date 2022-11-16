cmake_minimum_required(VERSION 3.10)

# set the project name
project(CPlayground)

set(EXEC_NAME lox)

# set the cpp standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(PROJECT_SOURCE_DIR, src)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY build)
set(CMAKE_BUILD_TYPE Debug)

# include_directories(${PROJECT_SOURCE_DIR})

set(sources 
    src/Main.cpp
    src/FileUtils.cpp

    src/cpplox/CPPLox.cpp
    src/cpplox/Environment.cpp
    src/cpplox/Expr.cpp
    src/cpplox/Interpreter.cpp
    src/cpplox/LoxParser.cpp
    src/cpplox/LoxValue.cpp
    src/cpplox/Token.cpp
    src/cpplox/TokenScanner.cpp
    src/cpplox/LoxFunction.cpp
    src/cpplox/LoxInstance.cpp
    src/cpplox/LoxClass.cpp
    src/cpplox/Analyzer.cpp
)

# file(GLOB sources
#      "src/*.h"
#      "src/*.cpp"
#      "src/**/*.h"
#      "src/**/*.cpp"
# )

# Add SDL2 library
find_package(SDL2 REQUIRED)
include_directories(${EXEC_NAME} ${SDL2_INCLUDE_DIRS})

# Add OpenCL library
find_package(OpenCL REQUIRED)
include_directories( ${OPENCL_INCLUDE_DIR})     

#Add better enums
include_directories("libs/better-enums")

# add the executable
add_executable(${EXEC_NAME} 
    ${sources}
)

# Add SDL2 lib
target_link_libraries(${EXEC_NAME}  ${SDL2_LIBRARIES})

# Add OpenCL lib
target_link_libraries(${EXEC_NAME}  "-framework OpenCL" )
