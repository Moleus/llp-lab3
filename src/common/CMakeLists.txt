#add_library(common_lib STATIC public/util/common.h)
#target_include_directories(common_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# common/CMakeLists.txt

# Set the project name
project(common C)

# Define the library
add_library(common_lib INTERFACE)

# Specify the include directories for the library
target_include_directories(common_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Add any source files or other dependencies if needed
# For example:
# target_sources(common INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/util/common.c)

# This is a header-only library, so no need to add any source files
