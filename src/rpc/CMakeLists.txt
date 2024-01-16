project(rpc C)

# Glob the interfaces folder
file(GLOB files *.proto)

set(PROTOBUF_INPUT_DIRECTORY "${PROJECT_SOURCE_DIR}")
set(PROTOBUF_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}")

set(srcs)  # list for c/h generated sources

# https://stackoverflow.com/questions/69483196/cmake-for-protobuff-c-rpc-project-generating-c-and-python-files
foreach (file IN LISTS files)
    get_filename_component(out ${file} NAME_WE)

    # Generate C headers and source files from the *.proto files and place
    # them in ${PROJECT_SOURCE_DIR}/server/include and
    # ${PROJECT_SOURCE_DIR}/server/ respectively
    add_custom_command(
            COMMENT "Generate this and that from ${file} using proto-c"
            COMMAND protoc-c
            --proto_path=${PROTOBUF_INPUT_DIRECTORY}
            --c_out=${PROTOBUF_OUTPUT_DIRECTORY}
            ${file}
            DEPENDS
            ${file}
            OUTPUT
            ${PROTOBUF_OUTPUT_DIRECTORY}/${out}.pb-c.c
            ${PROTOBUF_OUTPUT_DIRECTORY}/${out}.pb-c.h
            VERBATIM
    )
    list(APPEND srcs
            ${PROTOBUF_OUTPUT_DIRECTORY}/${out}.pb-c.h
            ${PROTOBUF_OUTPUT_DIRECTORY}/${out}.pb-c.c
    )
endforeach ()

list(APPEND srcs ${CMAKE_CURRENT_SOURCE_DIR}/common.c)
add_library(my_rpc_lib STATIC ${srcs})

find_package(Protobuf REQUIRED)
target_link_libraries(my_rpc_lib PUBLIC ${Protobuf_LIBRARIES} common_lib)
target_include_directories(my_rpc_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})