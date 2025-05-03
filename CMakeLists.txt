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
    src/fs_analyzer.c
    src/stats_collector.c
    src/sig_handler.c
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

add_executable(test_fs_analyzer
        tests/unit/test_fs_analyzer.c
        src/fs_analyzer.c
        src/stats_collector.c
        src/sig_handler.c
        src/filter.c
        src/utils.c
        ${unity_SOURCE_DIR}/src/unity.c
)
target_include_directories(test_fs_analyzer PRIVATE include)
add_test(NAME test_fs_analyzer COMMAND test_fs_analyzer)

add_executable(test_stats_collector
        tests/unit/test_stats_collector.c
        src/stats_collector.c
        src/utils.c
        ${unity_SOURCE_DIR}/src/unity.c
)
target_include_directories(test_stats_collector PRIVATE include)
add_test(NAME test_stats_collector COMMAND test_stats_collector)

add_executable(test_sig_handler
        tests/unit/test_sig_handler.c
        src/sig_handler.c
        ${unity_SOURCE_DIR}/src/unity.c
)
target_include_directories(test_sig_handler PRIVATE include)
add_test(NAME test_sig_handler COMMAND test_sig_handler)