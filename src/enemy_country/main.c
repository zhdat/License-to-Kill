#include "enemy_country.h"


int main() {
    signal(SIGALRM, SIG_IGN);
    memory_t* mem = open_shared_memory();
    writeToSharedMemory(mem);
    end_shared_memory(mem);
    return 0;
}
