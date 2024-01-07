#include "citizen_manager.h"
#include "debug.h"

/**
 * \file citizen_manager.c
 * \brief Defines functions for initializing and managing the citizen manager.
 */

volatile int timer_citizens[MAX_CITIZEN_COUNT];

sem_t* move_sem;

void set_semaphore(sem_t* sem) {
    move_sem = sem;
}

void timer_handler(int signum) {
    int i;
    signum = signum;
    for (i = 0; i < MAX_CITIZEN_COUNT; i++) {
        timer_citizens[i] = 1;
    }
}

void set_timer_citizens(void) {
    int i;
    for (i = 0; i < MAX_CITIZEN_COUNT; i++) {
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
    int start_row;
    int start_column;
    coordinate_t start_cell;
    coordinate_t end_cell;
    memory_t* mem;
    character_t* citizen;

    if (arg == NULL) {
        return;
    }

    mem = arg->mem;
    if (mem == NULL) {
        return;
    }

    citizen = &(mem->citizens[arg->index]);
    if (citizen == NULL) {
        return;
    }

    start_row = citizen->row;
    start_column = citizen->column;

    start_cell.row = start_row;
    start_cell.column = start_column;
    end_cell.row = row;
    end_cell.column = column;

    sem_wait(move_sem);
    decrements_population_in_cell(mem, start_column, start_row);
    next_move(&(mem->city_map), start_cell, end_cell, &citizen->column, &citizen->row, *citizen);
    increments_population_in_cell(mem, citizen->column, citizen->row);
    sem_post(move_sem);

}

void* citizen_to_work(void* args) {
    citizen_monitor_args_t* arg;
    memory_t* mem;
    character_t* citizen;
    int index;
    int row;
    int column;

    arg = (citizen_monitor_args_t*) args;
    if (arg == NULL) {
        return NULL;
    }

    index = arg->index;
    if (index < 0 || index >= MAX_CITIZEN_COUNT) {
        return NULL;
    }

    mem = arg->mem;
    if (mem == NULL) {
        return NULL;
    }

    citizen = &(mem->citizens[index]);
    row = citizen->work_row;
    column = citizen->work_column;


    while (!is_at_work(*citizen)) {
        if (timer_citizens[index] == 1) {
            move_citizen(arg, row, column);
            timer_citizens[index] = 0;
            usleep(10);
        }
    }
    return NULL;
}

void* citizen_to_home(void* args) {
    citizen_monitor_args_t* arg;
    memory_t* mem;
    character_t* citizen;
    int index;
    int row;
    int column;

    arg = (citizen_monitor_args_t*) args;
    if (arg == NULL) {
        return NULL;
    }

    index = arg->index;
    if (index < 0 || index >= MAX_CITIZEN_COUNT) {
        return NULL;
    }

    mem = arg->mem;
    if (mem == NULL) {
        return NULL;
    }

    citizen = &(mem->citizens[index]);
    if (citizen == NULL) {
        return NULL;
    }

    row = citizen->home_row;
    column = citizen->home_column;

    while (!is_at_home(*citizen)) {
        if (timer_citizens[index] == 1) {
            move_citizen(arg, row, column);
            timer_citizens[index] = 0;
            usleep(10);
        }
    }
    return NULL;
}

void* citizen_to_home_supermarket(void* args) {
    int have_to_go_to_supermarket;
    int supermarket_id;
    int index;
    int row;
    int column;
    citizen_monitor_args_t* arg;
    memory_t* mem;
    character_t* citizen;
    coordinate_t* supermarket_coordinates;
    city_t city;

    arg = (citizen_monitor_args_t*) args;
    index = arg->index;
    mem = arg->mem;
    citizen = &(mem->citizens[index]);
    city = mem->city_map;
    row = citizen->home_row;
    column = citizen->home_column;

    supermarket_id = selectRandomNumberUnder(2);
    have_to_go_to_supermarket = (selectRandomNumberUnder(4)) == 2;

    supermarket_coordinates = findTypeOfBuilding(&city, SUPERMARKET, NUMBER_OF_SUPERMARKETS);

    if (have_to_go_to_supermarket) {
        while (!character_is_at(*citizen, supermarket_coordinates[supermarket_id])) {
            if (timer_citizens[index] == 1) {
                move_citizen(arg, supermarket_coordinates[supermarket_id].row,
                             supermarket_coordinates[supermarket_id].column);
                timer_citizens[index] = 0;
            }
        }
    }

    while (!is_at_home(*citizen)) {
        if (timer_citizens[index] == 1) {
            move_citizen(arg, row, column);
            timer_citizens[index] = 0;
        }
    }
    return NULL;
}

int work_in_supermarket(memory_t mem, character_t citizen) {
    coordinate_t* supermarket_coordinates;
    supermarket_coordinates = findTypeOfBuilding(&mem.city_map, SUPERMARKET, NUMBER_OF_SUPERMARKETS);
    return (character_is_at(citizen, supermarket_coordinates[0]) ||
            character_is_at(citizen, supermarket_coordinates[1]));
}

void create_morning_thread(memory_t* mem, pthread_t ids[MAX_CITIZEN_COUNT],
                           citizen_monitor_args_t* args[MAX_CITIZEN_COUNT]) {
    int i;
    int j;
    if (mem->timer.hours == 8 && mem->timer.minutes == 0) {
#if DEBUG
        //printf("Création des threads du matin\n");
#endif
        for (i = 0; i < MAX_CITIZEN_COUNT; i++) {
            if (pthread_create(&ids[i], NULL, citizen_to_work, (void*) args[i]) == 1) {
                perror("pthread_create");
                exit(EXIT_FAILURE);
            }
        }
        for (j = 0; j < MAX_CITIZEN_COUNT; j++) {
            pthread_join(ids[j], NULL);
        }
    }
}

void create_evening_company_thread(memory_t* mem, pthread_t ids[MAX_CITIZEN_COUNT],
                                   citizen_monitor_args_t* args[MAX_CITIZEN_COUNT]) {
    int i;
    int j;
    if (mem->timer.hours == 17 && mem->timer.minutes == 0) {
        for (i = 0; i < MAX_CITIZEN_COUNT; i++) {
            if (!work_in_supermarket(*mem, mem->citizens[i])) {
                if (pthread_create(&ids[i], NULL, citizen_to_home_supermarket, (void*) args[i]) == 1) {
                    perror("pthread_create");
                    exit(EXIT_FAILURE);
                }
            }
        }
        for (j = 0; j < MAX_CITIZEN_COUNT; j++) {
            if (!work_in_supermarket(*mem, mem->citizens[j])) {
                pthread_join(ids[j], NULL);
            }
        }
    }
}

void create_evening_supermarket_thread(memory_t* mem, pthread_t ids[MAX_CITIZEN_COUNT],
                                       citizen_monitor_args_t* args[MAX_CITIZEN_COUNT]) {
    int i;
    int j;
    if (mem->timer.hours == 20 && mem->timer.minutes == 0) {
        for (i = 0; i < MAX_CITIZEN_COUNT; i++) {
            if (work_in_supermarket(*mem, mem->citizens[i])) {
                if (pthread_create(&ids[i], NULL, citizen_to_home, (void*) args[i]) == 1) {
                    perror("pthread_create");
                    exit(EXIT_FAILURE);
                }
            }
        }
        for (j = 0; j < MAX_CITIZEN_COUNT; j++) {
            if (work_in_supermarket(*mem, mem->citizens[j])) {
                pthread_join(ids[j], NULL);
            }
        }
    }
}

void create_citizens_thread(memory_t* mem) {
    pthread_t ids[MAX_CITIZEN_COUNT];
    citizen_monitor_args_t* args[MAX_CITIZEN_COUNT];
    int i;
    srand(time(NULL));

    for (i = 0; i < MAX_CITIZEN_COUNT; i++) {
        args[i] = (citizen_monitor_args_t*) malloc(sizeof(citizen_monitor_args_t));
        args[i]->index = i;
        args[i]->mem = mem;
    }

    while (mem->simulation_has_ended == 0) {
        create_morning_thread(mem, ids, args);
        create_evening_company_thread(mem, ids, args);
        create_evening_supermarket_thread(mem, ids, args);
    }

    for (i = 0; i < MAX_CITIZEN_COUNT; i++) {
        free(args[i]);
    }
}

