#include "counterintelligence_officer.h"

int main(void) {
    memory_t* mem = open_shared_memory();
    sem_t* move_sem = open_semaphore();
    set_semaphore(move_sem);
    set_signals();
    create_counter_intelligence_officer_threads(mem);
    close_semaphore(move_sem);
    end_shared_memory(mem);

    return 0;
}
