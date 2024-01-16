project(client C)

add_subdirectory(net_client)
add_subdirectory(parser)

add_executable(client main.c)
target_link_libraries(client client_lib parser_lib common_lib)
target_include_directories(client PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} .)
