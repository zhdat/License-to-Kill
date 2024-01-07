#include "tools.h"
#include "logger.h"
#include "minunit.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "debug.h"

MU_TEST(test_euclidean_distance) {
    double dist = euclidean_distance(0, 0, 3, 4);
    mu_check(fabs(dist - 5.0) < 0.0001);
}

MU_TEST(test_manhattan_distance) {
    int dist = manhattan_distance(0, 0, 3, 4);
    mu_check(dist == 7);
}

MU_TEST(test_caesarCipher) {
    char message[] = "abc";
    caesarCipher(message, 3);
    mu_check(strcmp(message, "def") == 0);
}

MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_euclidean_distance);
    MU_RUN_TEST(test_manhattan_distance);
    MU_RUN_TEST(test_caesarCipher);
    // Ajoutez d'autres tests ici
}

int main(int argc, char *argv[]) {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}

