cmake_minimum_required(VERSION 3.18)
project(sockety C)

set(CMAKE_C_STANDARD 99)

# Konfigurácia súboru Makefile
configure_file(${CMAKE_SOURCE_DIR}/Makefile.in ${CMAKE_BINARY_DIR}/Makefile @ONLY)

# Zdrojové súbory pre klienta
set(CLIENT_SOURCES
        src/client.c
        src/hra.c
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

# Zdrojové súbory pre server
set(SERVER_SOURCES
        src/server.c
        src/hra.c
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

# Vytvorenie cieľa pre klienta
add_executable(client ${CLIENT_SOURCES})

# Vytvorenie cieľa pre server
add_executable(server ${SERVER_SOURCES})

# Pridanie linkovania pre knižnicu pthread pre server a klienta
target_link_libraries(server pthread)
target_link_libraries(client pthread)
