cmake_minimum_required(VERSION 3.10)
project(duana C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/main.c
    src/cli.c
    src/config.c
    src/scanner.c
    src/utils.c
    src/output.c
)
add_executable(duana ${SOURCES})

enable_testing()

# Unit tests
add_executable(test_scanner
    tests/unit/test_scanner.c
    src/scanner.c
    src/utils.c
)
target_include_directories(test_scanner PRIVATE include)
add_test(NAME test_scanner COMMAND test_scanner)

add_executable(test_utils
    tests/unit/test_utils.c
    src/utils.c
)
target_include_directories(test_utils PRIVATE include)
add_test(NAME test_utils COMMAND test_utils)