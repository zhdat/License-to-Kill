#include "citizen_manager.h"

volatile int timer_citizens[MAX_CITIZEN_COUNT];

sem_t* move_sem;

void set_semaphore(sem_t* sem) {
    move_sem = sem;
}

void timer_handler(int signum) {
    for (int i = 0; i < MAX_CITIZEN_COUNT; i++) {
        timer_citizens[i] = 1;
    }
}

void set_timer_citizens(void) {
    for (int i = 0; i < MAX_CITIZEN_COUNT; i++) {
        timer_citizens[i] = 0;
    }
}

void set_signal_handler(void) {
    struct sigaction sa;
    sa.sa_handler = timer_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGALRM, &sa, NULL);
}

void move_citizen(citizen_monitor_args_t* arg, int row, int column) {
    int start_row, start_column;
    memory_t* mem = arg->mem;
    character_t* citizen = &(mem->citizens[arg->index]);

    start_row = citizen->row;
    start_column = citizen->column;

    coordinate_t start_cell;
    start_cell.row = start_row;
    start_cell.column = start_column;
    coordinate_t end_cell;
    end_cell.row = row;
    end_cell.column = column;

    sem_wait(move_sem);
    decrements_population_in_cell(mem, start_column, start_row);
    next_move(&(mem->city_map), start_cell, end_cell, &citizen->column, &citizen->row);
    increments_population_in_cell(mem, citizen->column, citizen->row);
    sem_post(move_sem);

}

void* citizen_to_work(void* args) {
    citizen_monitor_args_t* arg = (citizen_monitor_args_t*) args;
    memory_t* mem = arg->mem;
    character_t* citizen = &(mem->citizens[arg->index]);

    while (!is_at_work(*citizen)) {
        if (timer_citizens[arg->index] == 1) {
            move_citizen(arg, citizen->work_row, citizen->work_column);
            timer_citizens[arg->index] = 0;
        }
    }
    return NULL;
}

void* citizen_to_home(void* args) {
    citizen_monitor_args_t* arg = (citizen_monitor_args_t*) args;
    memory_t* mem = arg->mem;
    character_t* citizen = &(mem->citizens[arg->index]);

    while (!is_at_home(*citizen)) {
        if (timer_citizens[arg->index] == 1) {
            move_citizen(arg, citizen->home_row, citizen->home_column);
            timer_citizens[arg->index] = 0;
        }
    }
    return NULL;
}

void* citizen_to_home_supermarket(void* args) {
    int to_supermarket, supermarket;
    citizen_monitor_args_t* arg = (citizen_monitor_args_t*) args;
    memory_t* mem = arg->mem;
    character_t* citizen = &(mem->citizens[arg->index]);

    supermarket = rand() % 2;
    to_supermarket = rand() % 4;

    coordinate_t* supermarket_coordinates = findTypeOfBuilding(&mem->city_map, SUPERMARKET, NUMBER_OF_SUPERMARKETS);

    if (to_supermarket == 2) {
        while (!character_is_at(*citizen, supermarket_coordinates[supermarket])) {
            if (timer_citizens[arg->index] == 1) {
                move_citizen(arg, supermarket_coordinates[supermarket].row, supermarket_coordinates[supermarket].column);
                timer_citizens[arg->index] = 0;
            }
        }
    }

    while (!is_at_home(*citizen)) {
        if (timer_citizens[arg->index] == 1) {
            move_citizen(arg, citizen->home_row, citizen->home_column);
            timer_citizens[arg->index] = 0;
        }
    }
    return NULL;
}

int work_in_supermarket(memory_t mem, character_t citizen) {
    coordinate_t* supermarket_coordinates = findTypeOfBuilding(&mem.city_map, SUPERMARKET, NUMBER_OF_SUPERMARKETS);
    return (character_is_at(citizen, supermarket_coordinates[0]) || character_is_at(citizen, supermarket_coordinates[1]));
}

void create_morning_thread(memory_t* mem, pthread_t ids[MAX_CITIZEN_COUNT], citizen_monitor_args_t* args[MAX_CITIZEN_COUNT]) {
    if (mem->my_timer.hours == 8 && mem->my_timer.minutes == 0) {
        for (int i = 0; i < MAX_CITIZEN_COUNT; i++) {
            pthread_create(&ids[i], NULL, citizen_to_work, (void *) args[i]);
        }
        for (int j = 0; j < MAX_CITIZEN_COUNT; j++) {
            pthread_join(ids[j], NULL);
        }
    }
}

void create_evening_company_thread(memory_t* mem, pthread_t ids[MAX_CITIZEN_COUNT],
                                   citizen_monitor_args_t* args[MAX_CITIZEN_COUNT]) {
    if (mem->my_timer.hours == 17 && mem->my_timer.minutes == 0) {
        for (int i = 0; i < MAX_CITIZEN_COUNT; i++) {
            if (!work_in_supermarket(*mem, mem->citizens[i])) {
                pthread_create(&ids[i], NULL, citizen_to_home_supermarket, (void*) args[i]);
            }
        }
        for (int j = 0; j < MAX_CITIZEN_COUNT; j++) {
            if (!work_in_supermarket(*mem, mem->citizens[j])) {
                pthread_join(ids[j], NULL);
            }
        }
    }
}

void create_evening_supermarket_thread(memory_t* mem, pthread_t ids[MAX_CITIZEN_COUNT], citizen_monitor_args_t* args[MAX_CITIZEN_COUNT]) {
    if (mem->my_timer.hours == 20 && mem->my_timer.minutes == 0) {
        for (int i = 0; i < MAX_CITIZEN_COUNT; i++) {
            if (work_in_supermarket(*mem, mem->citizens[i])) {
                pthread_create(&ids[i], NULL, citizen_to_home, (void*) args[i]);
            }
        }
        for (int j = 0; j < MAX_CITIZEN_COUNT; j++) {
            if (work_in_supermarket(*mem, mem->citizens[j])) {
                pthread_join(ids[j], NULL);
            }
        }
    }
}

void create_citizens_thread(memory_t* mem){
    pthread_t ids[MAX_CITIZEN_COUNT];
    citizen_monitor_args_t* args[MAX_CITIZEN_COUNT];
    srand(time(NULL));

    for (int i = 0; i < MAX_CITIZEN_COUNT; i++) {
        args[i] = malloc(sizeof(citizen_monitor_args_t));
        args[i]->index = i;
        args[i]->mem = mem;
    }

    while (mem->simulation_has_ended == 0) {
        create_morning_thread(mem, ids, args);
        create_evening_company_thread(mem, ids, args);
        create_evening_supermarket_thread(mem, ids, args);
    }
}

