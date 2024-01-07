#include "minunit.h"
#include "citizen_manager.h"
#include "spy_simulation.h"
#include "memory.h"

volatile int timer_citizens_test[MAX_CITIZEN_COUNT];

volatile int test_signal_handled = 0;

sem_t* move_sem_test;

void fake_timer_handler(int signum) {
    test_signal_handled = 1;
}

void set_timer_citizens_test(void) {
    int i;
    for (i = 0; i < MAX_CITIZEN_COUNT; i++) {
        timer_citizens_test[i] = 0;
    }
}

void timer_handler_test(int signum) {
    int i;
    signum = signum;
    for (i = 0; i < MAX_CITIZEN_COUNT; i++) {
        timer_citizens_test[i] = 1;
    }
}

MU_TEST(test_set_timer_citizens) {
    // Initialiser timer_citizens avec des valeurs non nulles
    for (int i = 0; i < MAX_CITIZEN_COUNT; i++) {
        timer_citizens_test[i] = 1;
    }

    // Appeler la fonction
    set_timer_citizens_test();

    // Vérifier si toutes les valeurs sont définies à 0
    for (int i = 0; i < MAX_CITIZEN_COUNT; i++) {
        mu_check(timer_citizens_test[i] == 0);
    }
}

MU_TEST(test_timer_handler) {
    set_timer_citizens_test();
    timer_handler_test(0);  // L'argument de signal est ignoré dans la fonction

    for (int i = 0; i < MAX_CITIZEN_COUNT; i++) {
        mu_check(timer_citizens_test[i] == 1);
    }
}

MU_TEST(test_set_signal_handler) {
    test_signal_handled = 0;

    // Configure le faux gestionnaire de signal
    struct sigaction sa_test;
    sa_test.sa_handler = fake_timer_handler;
    sigemptyset(&sa_test.sa_mask);
    sa_test.sa_flags = 0;
    sigaction(SIGALRM, &sa_test, NULL);

    // Déclenche le signal
    raise(SIGALRM);

    // Vérifie si le faux gestionnaire a été appelé
    mu_check(test_signal_handled == 1);
}


MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_set_signal_handler);
    MU_RUN_TEST(test_timer_handler);
    MU_RUN_TEST(test_set_timer_citizens);
    MU_RUN_TEST(test_set_signal_handler);
    // Autres tests...
}

int main(int argc, char *argv[]) {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}
