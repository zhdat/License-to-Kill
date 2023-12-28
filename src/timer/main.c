#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>

#include "timer.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Insufficient parameters : .my_timer time(s) pids... \n");
        return 1;
    }
    float interval = strtof(argv[1], NULL);
    memory_t* sharedMemory = open_shared_memory();
    set_memory(sharedMemory);
    set_pids(argc, argv);

    my_set_timer(interval);
    while (!simulationFinished(sharedMemory)) {

    }

    free_timer();

    end_shared_memory(sharedMemory);

    printf("Timer : end \n");
    return 0;
}
