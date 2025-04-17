cmake_minimum_required(VERSION 3.10)
project(duana)

# Use C11 standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include project headers
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files for main executable
set(SOURCES
    src/main.c
    src/scanner.c
    src/output.c
    src/utils.c
)
add_executable(duana ${SOURCES})

enable_testing()

# Unit test: scanner
add_executable(test_scanner
    tests/unit/test_scanner.c
    src/scanner.c
    src/utils.c
)
target_include_directories(test_scanner PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_test(NAME test_scanner COMMAND test_scanner)

# Unit test: extension matching
add_executable(test_utils
    tests/unit/test_utils.c
    src/utils.c
)
target_include_directories(test_utils PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_test(NAME test_utils COMMAND test_utils)