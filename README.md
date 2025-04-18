# Disk Usage Analyzer (`duana`)

**duana** is a command-line tool that analyzes disk usage within a single directory by counting files and summing their sizes. It also supports filtering files by extension.

---

## Features

- **File counting**: Counts the regular files in the specified directory.
- **Size aggregation**: Computes the total size (in bytes) of all counted files.
- **Extension filter**: `--types` option to include only files with given extensions (e.g. `--types=.c,.txt`).
- **Modular design**: Components separated into `cli`, `config`, `scanner`, `utils`, and `output` modules.
- **Testing**: Unit tests in C and integration tests in Python (pytest).
- **Code style**: Formatted with `.clang-format` using the LLVM style.

## Requirements

- Ubuntu (or compatible Linux distribution)
- C compiler with C11 support (e.g., `gcc` ≥ 5.0)
- [CMake](https://cmake.org/) ≥ 3.10
- Python 3 and [pytest](https://docs.pytest.org/)

## Build Instructions

From the project root, run:

```bash
mkdir -p build && cd build
cmake ..
cmake --build .
```

This creates the `duana` executable and unit tests.

## Usage

```bash
./duana <directory> [--types=.ext1,.ext2,...]
```

- `<directory>`: Path to the directory to analyze.
- `--types=<ext_list>`: (optional) Comma-separated list of extensions to include.

### Examples

```bash
# Analyze /home/user/docs without filtering
./duana /home/user/docs

# Analyze /home/user/src, counting only .c and .h files
./duana /home/user/src --types=.c,.h
```

Output format:

```plaintext
Directory: /home/user/docs
Total files: 42
Total size: 1234567 bytes
```

## Tests

From the `build` directory:

```bash
# Run C unit tests with CTest
ctest --output-on-failure

# Run Python integration tests from project root
pytest tests/integration_py
```

## Code Formatting

Ensure consistent style with:

```bash
clang-format -i $(find . -name '*.c' -o -name '*.h')
```

## Project Structure

```plaintext
duana/
├── include/            # Public header files
├── src/                # C source files
├── tests/              # Unit and integration tests
├── .clang-format       # clang-format configuration
├── CMakeLists.txt      # Build script
├── .gitignore
└── build/              # Out-of-source build directory
```

## Roadmap

- Recursive scanning of subdirectories
- Export results to JSON
- Signal handling (e.g., SIGINT for graceful termination)

*Originally created for the Operating Systems course at XYZ University.*
