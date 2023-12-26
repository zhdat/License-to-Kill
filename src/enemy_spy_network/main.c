#include "enemy_spy_network.h"

int main(void) {
    memory_t* mem = open_shared_memory();
    create_and_run_source_agent_threads(mem);
    return 0;
}
