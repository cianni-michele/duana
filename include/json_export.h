#ifndef JSON_EXPORT_H
#define JSON_EXPORT_H

#include "stats_collector.h"

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
 * @param path Percorso del file di destinazione.
 * @param d    Array di DirStatsArr (può essere NULL o count=0).
 * @param e    Array di ExtStatsArr (può essere NULL o count=0).
 * @param avg  Media dimensione file.
 * @return 0 se OK, -1 su errore (errno).
 */
int du_json_export_file(const char *path,
                        const DirStatsArr *d,
                        const ExtStatsArr *e,
                        double avg);

#endif //JSON_EXPORT_H
