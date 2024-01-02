#include "enemy_spy_network.h"

int main(void) {
    //log_info("Starting enemy_spy_network");
    memory_t* mem = open_shared_memory();
    sem_t* move_sem = open_semaphore();
    sem_t* semaphore_message = create_semaphore_message();
    set_semaphore(move_sem);
    set_signals();
    set_signals_weak_bullet();
    set_signals_strong_bullet();
    create_enemy_spy_thread(mem);

    while(1) {
        if(mem->simulation_has_ended != 0) {
            break;
        }
    }

    close_semaphore(semaphore_message);
    close_semaphore(move_sem);
    end_shared_memory(mem);

    return 0;
}
