cmake_minimum_required(VERSION 3.10)
project(duana)

# Use C11 standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include project headers
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/main.c
    src/scanner.c
    src/output.c
    src/utils.c
)

# Define the executable
add_executable(duana ${SOURCES})

# Enable testing
enable_testing()

# Simple unit test executable (uses <assert.h>)
add_executable(test_scanner
    tests/unit/test_scanner.c
    src/scanner.c
)
target_include_directories(test_scanner PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_test(NAME test_scanner COMMAND test_scanner)