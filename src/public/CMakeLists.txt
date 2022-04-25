cmake_minimum_required(VERSION 3.0)
project(a01world-public C)

include_directories(.)
include_directories(windows)

add_library(a01world-public
        aw_types.c
        aw_memory.c
        aw_vector.c
        aw_logger.c
        )

if(WIN32)
    target_sources(a01world-public PRIVATE windows/aw_mutex.c windows/aw_cond.c windows/aw_thread.c)
else()
    target_sources(a01world-public PRIVATE posix/aw_mutex.c posix/aw_cond.c)
endif()

include(Tests.cmake)