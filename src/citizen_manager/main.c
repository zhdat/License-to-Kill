#include "citizen_manager.h"
#include "debug.h"

/**
 * \file main.c
 * \brief Uses functions for initializing and managing the citizen manager.
 */

int main(void) {
    //log_info("Starting citizen_manager");
    memory_t* mem = open_shared_memory();
    sem_t* move_sem = open_semaphore();
    set_semaphore(move_sem);
    set_timer_citizens();
    set_signal_handler();
    create_citizens_thread(mem);
    close_semaphore(move_sem);
    end_shared_memory(mem);
    return 0;
}
