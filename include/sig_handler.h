#ifndef SIG_HANDLER_H
#define SIG_HANDLER_H

#include <stdbool.h>

/**
 * @brief  Imposta il gestore per SIGINT (Ctrl+C) con SA_RESTART.
 * @return 0 se ha successo, -1 in caso di errore (errno settato).
 */
int du_setup_signals(void);

/**
 * @brief Imposta direttamente il flag di cancellazione senza generare segnali.
 */
void du_set_cancelled(void);

/**
 * @brief  Controlla se è stato ricevuto SIGINT.
 * @return true se il segnale è arrivato, false altrimenti.
 */
bool du_is_cancelled(void);

/**
 * @brief  Ripristina il gestore SIGINT originale.
 */
void du_signal_cleanup(void);

#endif //SIG_HANDLER_H
