cmake_minimum_required(VERSION 3.18)
project(sockety C)

set(CMAKE_C_STANDARD 99)


add_executable(client client.c)
add_executable(server server.c )