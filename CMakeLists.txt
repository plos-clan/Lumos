cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Lumos VERSION 0.0.0)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

enable_testing()

include_directories(${PROJECT_SOURCE_DIR}/include)

set(CMAKE_C_FLAGS "-w")
set(CMAKE_CXX_FLAGS "-w")

include(cmake/show-flags) 
include(cmake/mklib)
include(cmake/mkheader)

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(test)
