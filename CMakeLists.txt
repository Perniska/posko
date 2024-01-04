cmake_minimum_required(VERSION 3.18)
project(sockety C)

set(CMAKE_C_STANDARD 99)


add_executable(client client.c hra.c
        Game.h
        Game.c
        Snake.h
        Snake.c
        Position.h
        Position.c
        Food.h
        Food.c
        Direction.h
        Board.h
        Board.c)
add_executable(server server.c hra.c
        Game.h
        Game.c
        Snake.h
        Snake.c
        Position.h
        Position.c
        Food.h
        Food.c
        Direction.h
        Board.h
        Board.c)