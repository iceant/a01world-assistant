cmake_minimum_required(VERSION 3.0)
project(a01world-assistan-win C)

include_directories(../public)
include_directories(.)

add_executable(a01world-assistant WIN32 main.c)
target_link_libraries(a01world-assistant a01world-public)
target_link_options(a01world-assistant PRIVATE -mwindows)
