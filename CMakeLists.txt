cmake_minimum_required(VERSION 3.10)

# set the project name
project(CPlayground)

# set the cpp standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(PROJECT_SOURCE_DIR, src)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY build)
set(CMAKE_BUILD_TYPE Debug)

# include_directories(${PROJECT_SOURCE_DIR})

file(GLOB sources
     "src/*.h"
     "src/*.cpp"
     "src/**/*.h"
     "src/**/*.cpp"
)

# Add SDL2 library
find_package(SDL2 REQUIRED)
include_directories(CPlayground ${SDL2_INCLUDE_DIRS})

# Add OpenCL library
find_package(OpenCL REQUIRED)
include_directories( ${OPENCL_INCLUDE_DIR})     

# add the executable
add_executable(CPlayground 
    ${sources}
)

# Add SDL2 lib
target_link_libraries(CPlayground ${SDL2_LIBRARIES})

# Add OpenCL lib
target_link_libraries(CPlayground "-framework OpenCL" )
