#include "enemy_country.h"


int main() {
    signal(SIGALRM, SIG_IGN);
    //log_info("Starting enemy_country");
    memory_t* mem = open_shared_memory();
    writeToSharedMemory(mem);
    end_shared_memory(mem);
    return 0;
}
