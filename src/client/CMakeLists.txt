project(client C)

add_subdirectory(net_client)
add_subdirectory(parser)

add_executable(client main.c fs_directories_data.c)
target_link_libraries(client LINK_PRIVATE client_lib parser_lib common_lib)
target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} .)
