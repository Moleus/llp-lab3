project(parser C)

# Find Flex and Bison
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

# Set the paths to your lexer and parser source files
flex_target(Lexer lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.c)
bison_target(Parser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c)
add_flex_bison_dependency(Lexer Parser)

# Add your source files
set(SOURCE_FILES
        ${FLEX_Lexer_OUTPUTS}
        ${BISON_Parser_OUTPUTS}
        main.c
        types.c
)

add_library(parser_lib STATIC ${SOURCE_FILES})
target_include_directories(parser_lib PRIVATE ${CMAKE_CURRENT_BINARY_DIR} .)