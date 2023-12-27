#include "citizen_manager.h"

int main(void){
    signal(SIGALRM, signal_handler);
    all_threads_t *threads;
    threads = malloc(sizeof(all_threads_t));
    memory_t* mem = open_shared_memory();
    create_and_run_citizen_threads(mem, threads);
    sleep(1);
    for (int i = 0; i < MAX_CITIZEN_COUNT; ++i) {
        pthread_join(threads->citizen_threads[i], NULL);
    }
    return 0;
}
