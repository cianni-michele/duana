cmake_minimum_required(VERSION 3.10)
project(duana C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/main.c
    src/cli_parser.c
    src/utils.c
    src/filter.c
    src/options.c
)
add_executable(duana ${SOURCES})

enable_testing()

# Unity framework setup
include(FetchContent)
FetchContent_Declare(
        unity
        GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
        GIT_TAG v2.5.2
)
FetchContent_MakeAvailable(unity)
include_directories(${unity_SOURCE_DIR}/src)

# Unit tests
add_executable(test_filter
    tests/unit/test_filter.c
    src/filter.c
    src/options.c
    src/utils.c
    ${unity_SOURCE_DIR}/src/unity.c
)
target_include_directories(test_filter PRIVATE include)
add_test(NAME test_filter COMMAND test_filter)

add_executable(test_utils
    tests/unit/test_utils.c
    src/utils.c
    ${unity_SOURCE_DIR}/src/unity.c
)
target_include_directories(test_utils PRIVATE include)
add_test(NAME test_utils COMMAND test_utils)

add_executable(test_cli_parser
    tests/unit/test_cli_parser.c
    src/cli_parser.c
    src/options.c
    src/utils.c
    src/filter.c
    ${unity_SOURCE_DIR}/src/unity.c
)
target_include_directories(test_cli_parser PRIVATE include)
add_test(NAME test_cli_parser COMMAND test_cli_parser)