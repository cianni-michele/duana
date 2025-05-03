#include <stdio.h>
#include <signal.h>
#include "unity.h"
#include "sig_handler.h"

void setUp(void) {
    du_setup_signals();
}

void tearDown(void) {
    du_signal_cleanup();
}

// Test per verificare che setup_signals restituisca il valore corretto
void test_setup_signals_should_return_success(void) {
    // Cleanup per testare nuovamente il setup
    du_signal_cleanup();
    TEST_ASSERT_EQUAL_INT(0, du_setup_signals());
}

// Test per verificare lo stato iniziale dopo il setup
void test_is_cancelled_should_be_false_initially(void) {
    TEST_ASSERT_FALSE(du_is_cancelled());
}

// Test per verificare che un singolo segnale attivi il flag
void test_is_cancelled_should_be_true_after_signal(void) {
    raise(SIGINT);
    TEST_ASSERT_TRUE(du_is_cancelled());
}

// Test per verificare che segnali multipli non causino problemi
void test_is_cancelled_should_remain_true_after_multiple_signals(void) {
    raise(SIGINT);
    TEST_ASSERT_TRUE(du_is_cancelled());

    // Un secondo SIGINT non deve alterare lo stato né causare crash
    raise(SIGINT);
    TEST_ASSERT_TRUE(du_is_cancelled());
}

// Test per verificare che una nuova registrazione resetti il flag
void test_new_setup_should_reset_cancelled_flag(void) {
    raise(SIGINT);
    TEST_ASSERT_TRUE(du_is_cancelled());

    // Re-inizializza i segnali
    du_setup_signals();
    TEST_ASSERT_FALSE(du_is_cancelled());
}

// Test per verificare che il cleanup non modifichi lo stato
void test_cleanup_should_not_modify_cancelled_state(void) {
    TEST_ASSERT_FALSE(du_is_cancelled());

    du_signal_cleanup();
    TEST_ASSERT_FALSE(du_is_cancelled());
}

// Test per verificare il funzionamento di du_set_cancelled
void test_set_cancelled_should_activate_flag(void) {
    TEST_ASSERT_FALSE(du_is_cancelled());

    du_set_cancelled();
    TEST_ASSERT_TRUE(du_is_cancelled());
}

// Test per verificare che du_set_cancelled sia idempotente
void test_set_cancelled_should_be_idempotent(void) {
    du_set_cancelled();
    TEST_ASSERT_TRUE(du_is_cancelled());

    du_set_cancelled();
    TEST_ASSERT_TRUE(du_is_cancelled());
}

// Test per verificare l'interazione tra du_set_cancelled e setup_signals
void test_setup_signals_should_reset_after_set_cancelled(void) {
    du_set_cancelled();
    TEST_ASSERT_TRUE(du_is_cancelled());

    du_setup_signals();
    TEST_ASSERT_FALSE(du_is_cancelled());
}

// Test per verificare l'interazione tra set_cancelled e signal
void test_set_cancelled_and_signal_interaction(void) {
    du_set_cancelled();
    TEST_ASSERT_TRUE(du_is_cancelled());

    raise(SIGINT);
    TEST_ASSERT_TRUE(du_is_cancelled());
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_setup_signals_should_return_success);
    RUN_TEST(test_is_cancelled_should_be_false_initially);
    RUN_TEST(test_is_cancelled_should_be_true_after_signal);
    RUN_TEST(test_is_cancelled_should_remain_true_after_multiple_signals);
    RUN_TEST(test_new_setup_should_reset_cancelled_flag);
    RUN_TEST(test_cleanup_should_not_modify_cancelled_state);
    RUN_TEST(test_set_cancelled_should_activate_flag);
    RUN_TEST(test_set_cancelled_should_be_idempotent);
    RUN_TEST(test_setup_signals_should_reset_after_set_cancelled);
    RUN_TEST(test_set_cancelled_and_signal_interaction);

    return UNITY_END();
}