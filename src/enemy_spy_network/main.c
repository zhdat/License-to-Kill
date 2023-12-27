#include "enemy_spy_network.h"

int main(void) {
    all_threads_t threads;
    memory_t* mem = open_shared_memory();
    create_and_run_source_agent_threads(mem);
    create_and_run_attending_officer_threads(mem);
    sleep(1);
    printf("Enemy spy network has started\n");
    for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; ++i) {
        pthread_join(threads.source_agent_threads[i], NULL);
    }
    printf("Source agents have ended\n");
    pthread_join(threads.attending_officer_threads[0], NULL);
    printf("Enemy spy network has ended\n");
    return 0;
}
