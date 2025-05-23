cmake_minimum_required(VERSION 3.10)
project(ncurses_find_app C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(HEADERS_DIR "${CMAKE_SOURCE_DIR}/headers")

file(GLOB SOURCES "${SRC_DIR}/*.c")

include_directories(${HEADERS_DIR})

add_executable(ncurses_find_app ${SOURCES})

find_package(Curses REQUIRED)
target_link_libraries(ncurses_find_app ${CURSES_LIBRARIES})
target_include_directories(ncurses_find_app PRIVATE ${CURSES_INCLUDE_DIR})
