#include "citizen_manager.h"

int main(void){
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
