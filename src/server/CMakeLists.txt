project(server C)

set(SERVER_SRC server.c)

add_subdirectory(database)

add_executable(server ${SERVER_SRC})
target_link_libraries(server LINK_PRIVATE my_rpc_lib protobuf-c protobuf-c-rpc)
target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../rpc )
