#include "timer.h"

static memory_t *mem;
static int number_of_pids = 0;
static int *pids;

void set_memory(memory_t *memory_start) {
    mem = memory_start;
    mem->timer.days = 0;
    mem->timer.hours = 0;
    mem->timer.minutes = 0;
    mem->timer.step = 0;
    mem->timer.turns = 0;
}

void set_pids(int argc, char **argv) {
    number_of_pids = argc - 2;
    pids = (int *) malloc(number_of_pids * sizeof(int));
    for (int i = 0; i < number_of_pids; i++) {
        pids[i] = (int) strtol(argv[i + 2], NULL, 10);
    }
}

int has_simulation_ended(memory_t *memory) {
    if (memory->simulation_has_ended) {
        return 1;
    }

    if (memory->timer.turns > 2015) {
        memory->simulation_has_ended = 3;
        return 1;
    }

    return 0;
}

void timer_handler(int signum) {
    signum = signum;
    mem->timer.turns++;
    mem->timer.minutes += 10;
    if (mem->timer.minutes >= 60) {
        mem->timer.minutes = 0;
        mem->timer.hours++;
        if (mem->timer.hours >= 24) {
            mem->timer.hours = 0;
            mem->timer.days++;
        }
    }

    mem->memory_has_changed = 1;
    for (int i = 1; i < number_of_pids; i++) {
        kill(pids[i], SIGALRM);
    }
}

void my_set_timer(float interval) {
    struct itimerval it;
    timerclear(&it.it_interval);
    timerclear(&it.it_value);
    if (interval >= 1.0) {
        it.it_interval.tv_sec = (int) interval;
        it.it_value.tv_sec = (int) interval;
    } else {
        it.it_interval.tv_usec = interval * 1000000;
        it.it_value.tv_usec = interval * 1000000;
    }
    signal(SIGALRM, timer_handler);
    setitimer(ITIMER_REAL, &it, NULL);
}

void free_timer(void) {
    free(pids);
}

