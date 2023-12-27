#include "citizen_manager.h"

sem_t move_sem;
volatile int move_signal_flag = 0;

void signal_handler(int signum) {
    if (signum == SIGALRM) {
        sem_post(&move_sem); // Signaler le sémaphore pour autoriser le mouvement
    }
}

int main(void){
    sem_init(&move_sem, 0, 0);
    signal(SIGALRM, signal_handler);
    all_threads_t *threads;
    threads = malloc(sizeof(all_threads_t));
    memory_t* mem = open_shared_memory();
    create_and_run_citizen_threads(mem, threads);
    sleep(1);
    for (int i = 0; i < MAX_CITIZEN_COUNT; ++i) {
        pthread_join(threads->citizen_threads[i], NULL);
    }
    sem_destroy(&move_sem);
    return 0;
}
