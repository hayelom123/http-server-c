cmake_minimum_required(VERSION 3.10)

project(ChatServer C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(server ${SOURCES})

target_compile_options(server PRIVATE -Wall -Wextra -O2)