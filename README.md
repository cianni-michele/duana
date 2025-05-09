# duana – Disk Usage Analyzer (CLI, C/Linux)

**duana** is a command-line application written in C for Linux systems (Ubuntu), designed to recursively analyze a directory and generate a detailed disk usage report. It supports advanced options such as extension filters, sorting, and JSON export.

---

## 📂 Project Structure

```
duana/
├── include/       # Header files (.h)
├── src/           # Source code (.c)
├── tests/         # Unit and manual tests
├── CMakeLists.txt # Build configuration
├── Makefile       # Alternative to CMake
└── README.md      # Documentation
```

### Main Modules:

* `cli_parser` – CLI argument parsing
* `fs_analyzer` – recursive filesystem traversal
* `stats_collector` – data collection and aggregation
* `report_writer` – textual report generation
* `json_export` – JSON report export
* `filter` – extension filtering
* `sorter` – data sorting
* `sig_handler` – safe signal handling (SIGINT)
* `utils` – utility functions (e.g., size formatting)

---

## ⚙️ Compilation

You can compile the project with **CMake**:

```bash
cmake -B build
cmake --build build
```


### Optional Dependencies

For JSON export, the project uses the [`cJSON`](https://github.com/DaveGamble/cJSON) library. Check if it is installed:

```bash
pkg-config --exists libcjson && echo "cJSON available"
```

---

## 🚀 Execution

Basic command:

```bash
./duana <path> [--sort={size,count}] [--desc] [--filter=ext1,ext2] [ --json[=file] [--formatted] ]
```

Example:

```bash
./duana /home/user/docs --sort=size --filter=.pdf,.docx --json=report.json
```

---

## 🛠️ Available Options

| Option                         | Description                                         |
|--------------------------------|-----------------------------------------------------|
| `--sort=size` / `--sort=count` | Sort by size or file count                          |
| `--desc`                       | Sort in descending order                            |
| `--filter=.txt,.c,...`         | Analyze only files with the listed extensions (CSV) |
| `--json`                       | Export to stdout in JSON format                     |
| `--json=report.json`           | Export to `report.json` file                        |
| `--formatted`                  | Formatted output (human-readable)                   |

---

## 🧲 Signal Handling

Pressing `Ctrl+C`, the program handles the `SIGINT` signal properly, aborting the scan safely without corrupting partial data.

---

## 🧪 Testing

### Manual Tests

You can run manually compiled tests:

```bash
./build/test_cli_parser
```

---

## ❗ Error Handling and Logging

* Errors are **printed to `stderr`**, never to `stdout`
* The program **does not stop** in case of:

    * missing permissions (`EACCES`)
    * unreadable special files
    * disk space exhausted during export

---

## 📦 JSON Format

Example structure:

```json
{
  "directories": [
    { "path": "/home/user/docs", "totalSize": 1234567, "fileCount": 42 },
    "..."
  ],
  "extensions": [
    { "ext": ".pdf", "totalSize": 456789, "fileCount": 10 },
    "..."
  ],
  "avgFileSize": 29400.34
}
```

* UTF-8 encoding
* Proper escaping for special characters
* Atomic file writing to temporary `.tmp` file (with final `rename()`)

---

## 👨‍💻 License and Author

This project is licensed under the **MIT License**.  
See the [LICENSE](./LICENCE) file for more information.

**Author:** Michele Cianni

**University Course:** Operating Systems – 2024/25

**University:** University of Camerino

**Email:** michele.cianni@studenti.unicam.it
