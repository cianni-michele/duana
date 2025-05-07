cmake_minimum_required(VERSION 3.14)
project(duana C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

include(FetchContent)

# Unity framework setup
FetchContent_Declare(
        unity
        GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
        GIT_TAG v2.5.2
)
FetchContent_MakeAvailable(unity)
include_directories(${unity_SOURCE_DIR}/src)

# CJSON library setup
set(ENABLE_CJSON_TEST OFF CACHE INTERNAL "Disable cJSON tests")
FetchContent_Declare(
        cJSON
        GIT_REPOSITORY https://github.com/DaveGamble/cJSON.git
        GIT_TAG        v1.7.18
)
FetchContent_MakeAvailable(cJSON)
include_directories(${cjson_SOURCE_DIR})

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
    src/report_writer.c
    src/json_export.c
)
add_executable(duana ${SOURCES})
target_include_directories(duana PRIVATE ${cJSON_SOURCE_DIR})
target_link_libraries(duana PRIVATE cJSON)

enable_testing()

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

add_executable(test_report_writer
        tests/unit/test_report_writer.c
        src/report_writer.c
        src/utils.c
        ${unity_SOURCE_DIR}/src/unity.c
)
target_include_directories(test_report_writer PRIVATE include)
add_test(NAME test_report_writer COMMAND test_report_writer)

add_executable(test_json_export
        tests/unit/test_json_export.c
        src/json_export.c
        src/utils.c
        ${cJSON_SOURCE_DIR}/cJSON.c
        ${unity_SOURCE_DIR}/src/unity.c
)
target_include_directories(test_json_export PRIVATE include)
add_test(NAME test_json_export COMMAND test_json_export)