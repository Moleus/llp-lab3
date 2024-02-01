project(lab_test LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG       v1.14.0
)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE TEST_SOURCES ${CMAKE_CURRENT_LIST_DIR}/**.cpp)
file(GLOB_RECURSE TEST_HEADERS ${CMAKE_CURRENT_LIST_DIR}/**.h)

enable_testing()

add_executable(
        ${PROJECT_NAME}
        ${TEST_SOURCES}
        ${TEST_HEADERS}
)
#target_include_directories(${PROJECT_NAME})

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,/usr/local/lib")

target_link_libraries(
        ${PROJECT_NAME}
        LINK_PRIVATE
        GTest::gtest_main
        GTest::gmock_main
        common_lib
        database_lib
        server_lib
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})