#include <stdio.h>
#include <stdlib.h>

#include "character_factory.h"
#include "spy_simulation.h"
#include "simulation_signals.h"



int main(void)
{

    memory_t *mem = create_shared_memory();
    set_content_memory(mem);
    set_memory(mem);
    while (1){
        if (mem->simulation_has_ended != 0){
            break;
        }
    }

    end_shared_memory(mem);

    exit(EXIT_SUCCESS);
}
