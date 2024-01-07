#include "enemy_country.h"
#include "debug.h"

/**
 * \file main.c
 * \brief Uses functions for initializing and managing the enemy country.
 */

int main() {
    signal(SIGALRM, SIG_IGN);
    memory_t* mem = open_shared_memory();
    writeToSharedMemory(mem);
    end_shared_memory(mem);
    return 0;
}
