cmake_minimum_required(VERSION 3.26)
project(lab3 C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fblocks -lBlocksRuntime")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fblocks -lBlocksRuntime")

set(SERVER_SRC src/server/server.c)
set(CLIENT_SRC src/client/client.c)

add_subdirectory(src/common)
add_subdirectory(src/rpc)
add_subdirectory(src/client)
add_subdirectory(src/database)
add_subdirectory(src/server)
add_subdirectory(test)