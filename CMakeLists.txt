cmake_minimum_required(VERSION 3.20)
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

project(Workspace)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Release")

add_compile_options(
    -Wall
    -Wextra
)

option(BUILD_CLIENT "Build client" ON)
option(BUILD_SERVER "Build server" ON)

if(BUILD_CLIENT)
    add_subdirectory("src/client")
endif()

if(BUILD_SERVER)
    add_subdirectory("src/server")
endif()
