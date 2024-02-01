project(server C)

set(SERVER_SRC server.c fs.c)

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,/usr/local/lib")

add_executable(server ${SERVER_SRC} main.c)
target_link_libraries(server LINK_PRIVATE my_rpc_lib protobuf-c protobuf-c-rpc database_lib common_lib)
target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../rpc )

add_library(server_lib STATIC ${SERVER_SRC})
target_link_libraries(server_lib LINK_PUBLIC my_rpc_lib protobuf-c protobuf-c-rpc database_lib common_lib)
target_include_directories(server_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})