cmake_minimum_required(VERSION 3.22)
project(untitled C)

set(CMAKE_C_STANDARD 99)

add_executable(untitled client.c server.c bakery_algorithm.c bakery_algorithm_server.c)
