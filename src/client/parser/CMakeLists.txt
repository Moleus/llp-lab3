project(parser C)

# Find Flex and Bison
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

set(PARSER_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen)
set(LEXER_OUT ${CMAKE_CURRENT_BINARY_DIR}/lexer.c)
set(PARSER_OUT ${CMAKE_CURRENT_BINARY_DIR}/parser.c)

# Set the paths to your lexer and parser source files
flex_target(Lexer lexer.l "${LEXER_OUT}" DEFINES_FILE "${PARSER_DIR}/lexer.h")
bison_target(Parser parser.y "${PARSER_OUT}" DEFINES_FILE "${PARSER_DIR}/parser.h")
add_flex_bison_dependency(Lexer Parser)

# Add your source files
set(SOURCE_FILES
        ${FLEX_Lexer_OUTPUTS}
        ${BISON_Parser_OUTPUTS}
        my_parser.c
        types.c
)

add_library(parser_lib STATIC ${SOURCE_FILES})
target_include_directories(parser_lib PRIVATE ${CMAKE_CURRENT_BINARY_DIR} ${PARSER_DIR} .)

add_executable(parser main.c)
target_link_libraries(parser parser_lib)
target_include_directories(parser PRIVATE ${CMAKE_CURRENT_BINARY_DIR} ${PARSER_DIR} .)
