cmake_minimum_required(VERSION 3.10)
project(MessageQueue C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall -Wextra -pthread")

add_executable(message_queue main.c)
