# Disk Usage Analyzer (`duana`)

**duana** è un tool da linea di comando per analizzare lo spazio su disco all'interno di una singola directory, fornendo un riepilogo del numero di file e della loro dimensione totale. Supporta inoltre un filtro per estensioni di file.

---

## Caratteristiche

- **Conteggio file**: conta i file regolari presenti nella directory specificata.
- **Somma dimensioni**: calcola la somma delle dimensioni in byte di tutti i file conteggiati.
- **Filtro per estensioni**: opzione `--types` per considerare solo file con specifiche estensioni, ad esempio `--types=.c,.txt`.
- **Modularità**: architettura basata su moduli `cli`, `config`, `scanner`, `utils` e `output`.
- **Testing**: suite di test unitari in C e test di integrazione in Python (pytest).
- **Stile di codice**: formattazione con `.clang-format` (LLVM style).

## Requisiti

- Ubuntu (o altra distribuzione Linux compatibile)
- Compilatore C che supporti C11 (es. `gcc` >= 5.0)
- [CMake](https://cmake.org/) >= 3.10
- Python 3 + [pytest](https://docs.pytest.org/)

## Installazione e build

Esegui i comandi seguenti dalla radice del progetto:

```bash
# Crea la cartella di build out-of-source
mkdir -p build && cd build
# Genera i file di build con CMake
cmake ..
# Compila l'eseguibile e i test
cmake --build .
```

## Uso

```bash
./duana <directory> [--types=.ext1,.ext2,...]
```

- `<directory>`: path della directory da analizzare.
- `--types=<ext_list>`: (opzionale) lista di estensioni separate da virgola. Solo i file con queste estensioni saranno conteggiati.

### Esempi

```bash
# Analizza /home/user/docs senza filtro
./duana /home/user/docs

# Analizza /home/user/src filtrando solo .c e .h
./duana /home/user/src --types=.c,.h
```

In output vedrai:

```plaintext
Directory: /home/user/docs
Total files: 42
Total size: 1234567 bytes
```

## Test

Dalla cartella `build`, esegui:

```bash
# Test unitari in C
test_scanner
# oppure con CTest (richiede cmake --build . prima)
ctest --output-on-failure

# Test di integrazione Python
desde la root del progetto
pytest tests/integration_py
```

## Formattazione del codice

Per applicare lo stile definito in `.clang-format`:

```bash
clang-format -i $(find . -name "*.c" -o -name "*.h")
```

## Struttura del progetto

```plaintext
duana/
├── include/         # file header pubblici
├── src/             # sorgenti C
├── tests/
│   ├── unit/        # test unitari C
│   └── integration/ # test di integrazione Python
├── .clang-format    # configurazione formatter
├── CMakeLists.txt   # script di build
├── .gitignore
└── build/           # directory di build (non versionata)
```

## Roadmap

- Scansione ricorsiva delle sottodirectory
- Esportazione del report in formato JSON
- Gestione di segnali (es. `SIGINT` per terminazione pulita)

---

*Progetto realizzato nell'ambito del corso di Sistemi Operativi, Università degli studi di Camerino.*
