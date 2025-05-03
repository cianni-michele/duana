#ifndef FS_ANALYZER_H
#define FS_ANALYZER_H

#include "filter.h"
#include "stats_collector.h"

/**
 * @brief Scans recursively a directory tree starting at @p root.
 *
 * Ogni file regolare viene passato a du_stats_update_file() con:
 * - il percorso della directory
 * - estensione (malloc’ed, da liberare internamente)
 * - dimensione in byte
 *
 * Filtri ed eventuale cancellazione (SIGINT) vengono rispettati.
 *
 * @param root  Path della directory di partenza (deve esistere e essere leggibile).
 * @param opt   Puntatore a DuOptions (filtro estensioni, etc.).
 * @param stats Handle opaco restituito da du_stats_init().
 * @return 0 se la scansione è completa o interrotta, -1 su errore grave (errno impostato).
 */
int du_scan_directory(const char *root, const DuOptions *opt, DuStats *stats);

/**
 * @brief Segnala di interrompere la scansione in corso.
 *
 * Chiamata dal signal handler (SIGINT) per impostare un flag interno.
 */
void du_scan_abort(void);

#endif //FS_ANALYZER_H
