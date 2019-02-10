cmake_minimum_required(VERSION 3.5)

PROJECT(hydro)

INCLUDE_DIRECTORIES(
include
)

AUX_SOURCE_DIRECTORY(src DIR_SRCS)

SET(TEST_MATH
${DIR_SRCS})

ADD_EXECUTABLE(main ${TEST_MATH})

add_definitions(-std=c++11)

find_package(Threads)

target_link_libraries(main ${CMAKE_THREAD_LIBS_INIT})
