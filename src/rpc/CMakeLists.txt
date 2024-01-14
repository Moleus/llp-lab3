project(rpc C)

add_library(my_rpc_lib STATIC common.pb-c.c)
target_link_libraries(my_rpc_lib LINK_PUBLIC protobuf-c)
target_include_directories(my_rpc_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})