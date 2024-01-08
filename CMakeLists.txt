cmake_minimum_required(VERSION 3.18)
project(sockety C)

set(CMAKE_C_STANDARD 99)


add_executable(client src/client.c src/hra.c
        src/Game.h
        src/Game.c
        src/Snake.h
        src/Snake.c
        src/Position.h
        src/Position.c
        src/Food.h
        src/Food.c
        src/Direction.h
        src/Board.h
        src/Board.c
)
add_executable(server src/server.c src/hra.c
        src/Game.h
        src/Game.c
        src/Snake.h
        src/Snake.c
        src/Position.h
        src/Position.c
        src/Food.h
        src/Food.c
        src/Direction.h
        src/Board.h
        src/Board.c
)

target_link_libraries(server pthread)
target_link_libraries(client pthread)