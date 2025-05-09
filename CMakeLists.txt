cmake_minimum_required(VERSION 3.14)
project(duana C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

include(FetchContent)

# Unity framework setup
FetchContent_Declare(unity GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git GIT_TAG v2.5.2)
FetchContent_MakeAvailable(unity)
include_directories(${unity_SOURCE_DIR}/src)

# cJSON library setup
set(ENABLE_CJSON_TEST OFF CACHE INTERNAL "Disable cJSON tests")
FetchContent_Declare(cJSON GIT_REPOSITORY https://github.com/DaveGamble/cJSON.git GIT_TAG v1.7.18)
FetchContent_MakeAvailable(cJSON)
include_directories(${cjson_SOURCE_DIR})
link_directories(${cjson_BINARY_DIR})

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
        src/sorter.c
)
add_executable(duana ${SOURCES})
target_include_directories(duana PRIVATE ${cJSON_SOURCE_DIR})
target_link_libraries(duana PRIVATE cjson)

enable_testing()

# Helper macro for unit tests
macro(add_unit_test TEST_NAME TEST_SRC DEPENDENCIES)
    add_executable(${TEST_NAME} ${TEST_SRC} ${DEPENDENCIES} ${unity_SOURCE_DIR}/src/unity.c)
    target_include_directories(${TEST_NAME} PRIVATE include)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endmacro()

# Unit tests
add_unit_test(test_filter tests/unit/test_filter.c "src/filter.c;src/options.c;src/utils.c")
add_unit_test(test_utils tests/unit/test_utils.c "src/utils.c")
add_unit_test(test_cli_parser tests/unit/test_cli_parser.c "src/cli_parser.c;src/options.c;src/utils.c;src/filter.c")
add_unit_test(test_fs_analyzer tests/unit/test_fs_analyzer.c "src/fs_analyzer.c;src/stats_collector.c;src/sig_handler.c;src/filter.c;src/utils.c")
add_unit_test(test_stats_collector tests/unit/test_stats_collector.c "src/stats_collector.c;src/utils.c")
add_unit_test(test_sig_handler tests/unit/test_sig_handler.c "src/sig_handler.c")
add_unit_test(test_report_writer tests/unit/test_report_writer.c "src/report_writer.c;src/utils.c")
add_unit_test(test_json_export tests/unit/test_json_export.c "src/json_export.c;src/utils.c;${cJSON_SOURCE_DIR}/cJSON.c")
add_unit_test(test_sorter tests/unit/test_sorter.c "src/sorter.c")