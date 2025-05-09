# duana – Disk Usage Analyzer (CLI, C/Linux)

**duana** è un'applicazione da linea di comando scritta in C per sistemi Linux (Ubuntu), 
progettata per analizzare ricorsivamente una directory e generare un report dettagliato sull'utilizzo del disco. 
Supporta opzioni avanzate come filtro per estensione, ordinamento e esportazione in JSON.

---

## 📂 Struttura del progetto

```
duana/
├── include/       # Header file (.h)
├── src/           # Codice sorgente (.c)
├── tests/         # Test unitari e integrazione
└── CMakeLists.txt # Configurazione build
```

### Moduli principali:

* `cli_parser` – parsing argomenti CLI
* `fs_analyzer` – traversing ricorsivo del filesystem
* `stats_collector` – raccolta e aggregazione statistiche
* `report_writer` – generazione report testuale
* `json_export` – esportazione report in formato JSON
* `filter` – filtro per estensioni
* `sorter` – ordinamento dinamico dei dati
* `sig_handler` – gestione sicura dei segnali (SIGINT)
* `utils` – funzioni di supporto (es. formattazione dimensioni)
* `options` – gestione opzioni CLI (es. `--sort`, `--filter`)

---

## ⚙️ Compilazione

È possibile compilare il progetto con **CMake**:

```bash
cmake -B build
cmake --build build
```


### Dipendenze opzionali

Per l'esportazione in JSON viene usata la libreria [`cJSON`](https://github.com/DaveGamble/cJSON). Verifica se è installata:

```bash
pkg-config --exists libcjson && echo "cJSON disponibile"
```

---

## 🚀 Esecuzione

Comando base:

```bash
./duana <path> [--sort={size,count}] [--desc] [--filter=ext1,ext2] [ --json[=file] [--formatted] ]
```

Esempio:

```bash
./duana /home/user/docs --sort=size --filter=.pdf,.docx --json=report.json
```

---

## 🛠️ Opzioni disponibili

| Opzione                        | Descrizione                                        |
|--------------------------------|----------------------------------------------------|
| `--sort=size` / `--sort=count` | Ordina per dimensione o numero di file             |
| `--desc`                       | Ordina in ordine decrescente                       |
| `--filter=.txt,.c,...`         | Analizza solo file con estensioni specifiche (CSV) |
| `--json`                       | Esporta su stdout in formato JSON                  |
| `--json=report.json`           | Esporta su file `report.json`                      |
| `--formatted`                  | Esporta in formato JSON formattato                 |

---

## 🧲 Gestione segnali

Premendo `Ctrl+C`, il programma gestisce correttamente il segnale `SIGINT`, abortendo la scansione in modo sicuro senza corrompere i dati parziali raccolti.

---

## 🧪 Testing

### Test manuali

Puoi eseguire i test compilati manualmente:

```bash
./build/tests/test_cli_parser
```

### Script automatico (se disponibile)

```bash
./run_tests.sh
```

---

## ❗ Error Handling e Logging

* Gli errori sono **stampati su `stderr`**, mai su `stdout`
* Il programma **non si interrompe** in caso di:

    * permessi mancanti (`EACCES`)
    * file speciali non leggibili
    * spazio su disco esaurito durante l’export

---

## 📦 Formato JSON

Esempio di struttura:

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

* Encoding UTF-8
* Escape corretti per caratteri speciali
* Scrittura atomica in file temporaneo `.tmp` (con `rename()` finale)

---

## 🧑‍💻 Licenza e Autore

Questo progetto è rilasciato sotto la licenza MIT.
Guarda il file [LICENCE](./LICENCE) per i dettagli.

**Autore:** Michele Cianni

**Corso Universitario:** Sistemi Operativi - 2024/25

**Università:** Università degli Studi di Camerino

**Email:** michele.cianni@studenti.unicam.it
