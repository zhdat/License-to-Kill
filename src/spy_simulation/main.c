#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#include "character_factory.h"
#include "spy_simulation.h"
#include "simulation_signals.h"


int main(void) {
    memory_t* mem = create_shared_memory();
    sem_unlink(SEMAPHORE_NAME);
    sem_t* move_sem = create_semaphore();
    set_content_memory(mem);
    set_memory(mem);
    while (1) {
        if (mem->simulation_has_ended != 0) {
            break;
        }
    }
    close_semaphore(move_sem);
    destroy_semaphore(move_sem);
    end_shared_memory(mem);
    destroy_shared_memory();

    exit(EXIT_SUCCESS);
}
