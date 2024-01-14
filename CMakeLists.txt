cmake_minimum_required(VERSION 3.26)
project(lab3 C)

set(CMAKE_C_STANDARD 17)

include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(SERVER_SRC
        src/server/server.c
)

set(CLIENT_SRC
        src/client/client.c
)

set(COMMON_SRC
        src/rpc/common.pb-c.c
)

add_library(my-rpc STATIC ${COMMON_SRC})

link_directories(/usr/local/lib)

# Server executable
add_executable(server ${SERVER_SRC})
target_include_directories(server PRIVATE ${PROTOBUF_C_INCLUDE_DIRS} src/rpc)
target_link_libraries(server PRIVATE protobuf-c my-rpc protobuf-c-rpc)

# Client executable
add_executable(client ${CLIENT_SRC})
target_include_directories(client PRIVATE ${PROTOBUF_C_INCLUDE_DIRS} src/rpc)
target_link_libraries(client PRIVATE protobuf-c my-rpc protobuf-c-rpc)
