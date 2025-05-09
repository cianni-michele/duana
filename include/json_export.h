#ifndef JSON_EXPORT_H
#define JSON_EXPORT_H

#include "stats_collector.h"
#include "stdbool.h"

/**
 * @file json_export.h
 * @brief Esportazione del report in formato JSON.
 */

/**
 * Serializza il report in JSON e lo scrive su file in modo atomico.
 *
 * Crea un file temporaneo "<path>.tmp", vi scrive l'intero JSON,
 * chiama fsync(), poi rinomina in "path". In caso di errore
 * rimuove il .tmp e restituisce -1 (errno settato).
 *
 * @param path      Percorso del file di destinazione.
 * @param d         Array di DirStatsArr (può essere NULL o count=0).
 * @param dcount    Numero di directory.
 * @param e         Array di ExtStatsArr (può essere NULL o count=0).
 * @param ecount    Numero di estensioni.
 * @param avg       Media dimensione file.
 * @param formatted Se true, il JSON sarà formattato in modo leggibile.
 * @return 0 se OK, -1 su errore (errno).
 */
int du_json_export_file(
    const char* path,
    const DirStatsArr* d, size_t dcount,
    const ExtStatsArr* e,size_t ecount,
    double avg, bool formatted
    );

/**
 * Serializza il report in JSON e lo scrive su stdout.
 *
 * @param d         Array di DirStatsArr (può essere NULL o count=0).
 * @param dcount    Numero di directory.
 * @param e         Array di ExtStatsArr (può essere NULL o count=0).
 * @param ecount    Numero di estensioni.
 * @param avg       Media dimensione file.
 * @param formatted Se true, il JSON sarà formattato in modo leggibile.
 * @return 0 se OK, -1 su errore.
 */
int du_json_export_stdout(
    const DirStatsArr* d, size_t dcount,
    const ExtStatsArr* e, size_t ecount,
    double avg, bool formatted
    );

#endif // JSON_EXPORT_H
