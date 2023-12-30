#include "enemy_spy_network.h"

int main(void) {
    memory_t* mem = open_shared_memory();
    sem_t* move_sem = open_semaphore();
    set_semaphore(move_sem);
    set_signals();
    set_signals_weak_bullet();
    set_signals_strong_bullet();
    create_enemy_spy_thread(mem);
    close_semaphore(move_sem);
    end_shared_memory(mem);

    return 0;
}
