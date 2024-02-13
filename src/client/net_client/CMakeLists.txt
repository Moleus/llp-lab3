project(net_client C)

set(CLIENT_SRC net_client.c)

add_library(${PROJECT_NAME}_lib STATIC ${CLIENT_SRC})
target_link_libraries(${PROJECT_NAME}_lib LINK_PRIVATE my_rpc_lib protobuf-c protobuf-c-rpc parser_lib common_lib)
target_include_directories(${PROJECT_NAME}_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../rpc .)
