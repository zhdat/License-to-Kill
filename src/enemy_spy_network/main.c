#include "enemy_spy_network.h"

int main(void) {
    memory_t* mem = open_shared_memory();
    printf("BOP\n");
    create_and_run_source_agent_threads(mem);
    printf("BAP\n");
    create_and_run_attending_officer_threads(mem);
    return 0;
}
