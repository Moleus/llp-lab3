project(client C)

add_subdirectory(net_client)
add_subdirectory(parser)

add_executable(client main.c helpers.c)
target_link_libraries(client LINK_PRIVATE net_client_lib parser_lib common_lib)
target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} .)

add_library(client_lib STATIC main.c helpers.c)
target_link_libraries(client_lib LINK_PUBLIC net_client_lib parser_lib common_lib)
target_include_directories(client_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} .)