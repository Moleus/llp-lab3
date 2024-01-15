project(client C)

#set(CLIENT_SRC client.c)
#
#add_executable(client ${CLIENT_SRC})
#target_link_libraries(client LINK_PRIVATE my_rpc_lib protobuf-c protobuf-c-rpc)
#target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../rpc )

# Find Flex and Bison
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

# Set the paths to your lexer and parser source files
flex_target(Lexer parser/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.c)
bison_target(Parser parser/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c)
add_flex_bison_dependency(Lexer Parser)

# Add your source files
set(SOURCE_FILES
        ${FLEX_Lexer_OUTPUTS}
        ${BISON_Parser_OUTPUTS}
        parser/main.c
        parser/types.c
)

# Include directories if needed
include_directories(${CMAKE_CURRENT_BINARY_DIR} parser)

# Create the executable
add_executable(parser ${SOURCE_FILES})

# Link against any additional libraries if needed
target_link_libraries(parser)

