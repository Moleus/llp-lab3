cmake_minimum_required(VERSION 3.26)
project(lab3 C)

set(CMAKE_C_STANDARD 17)

set(SERVER_SRC src/server/server.c)
set(CLIENT_SRC src/client/client.c)

add_subdirectory(src/common)
add_subdirectory(src/rpc)
add_subdirectory(src/client)
add_subdirectory(src/server)
