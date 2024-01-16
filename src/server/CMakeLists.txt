project(server C)

set(SERVER_SRC server.c)

add_executable(server ${SERVER_SRC})
target_link_libraries(server LINK_PRIVATE my_rpc_lib protobuf-c protobuf-c-rpc database_lib common_lib)
target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../rpc )
