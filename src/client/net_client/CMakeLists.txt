project(net_client C)

set(CLIENT_SRC client.c)

add_executable(client ${CLIENT_SRC})
target_link_libraries(client LINK_PRIVATE my_rpc_lib protobuf-c protobuf-c-rpc parser_lib common_lib)
target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../rpc )
