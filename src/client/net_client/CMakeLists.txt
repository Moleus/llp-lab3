project(net_client C)

set(CLIENT_SRC net_client.c)

add_library(client_lib STATIC ${CLIENT_SRC})
target_link_libraries(client_lib LINK_PRIVATE my_rpc_lib protobuf-c protobuf-c-rpc parser_lib common_lib)
target_include_directories(client_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../rpc .)
