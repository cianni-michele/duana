#include "sig_handler.h"
#include <errno.h>
#include <signal.h>
#include <stddef.h>

static volatile sig_atomic_t cancelled = 0;
static struct sigaction old_action;

/**
 * @brief  Handler interno per SIGINT: setta il flag di cancellazione.
 */
static void handle_sigint(const int signo) {
    (void)signo;
    cancelled = 1;
}

int du_setup_signals(void) {
    struct sigaction action;
    action.sa_handler = handle_sigint;
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_RESTART;

    // Registra il nuovo handler, salva il precedente in old_action
    if (sigaction(SIGINT, &action, &old_action) == -1) {
        return -1;
    }

    // Azzeriamo il flag all’avvio
    cancelled = 0;
    return 0;
}

void du_set_cancelled(void) {
    cancelled = 1;
}

bool du_is_cancelled(void) {
    return cancelled != 0;
}

void du_signal_cleanup(void) {
    // Ripristina il gestore SIGINT originale
    sigaction(SIGINT, &old_action, NULL);
}
