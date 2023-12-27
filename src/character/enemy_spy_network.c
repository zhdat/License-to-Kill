#include <stdlib.h>
#include <unistd.h>
#include "enemy_spy_network.h"


void move_source_agent(memory_t *mem, int row, int column, int id) {
    printf("\nmove source agent\n");
    for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; ++i) {
        if (mem->source_agents[i].character.id == id) {
            pthread_mutex_lock(&mem->mutex);
            decrements_population_in_cell(mem, mem->source_agents[i].character.column,
                                          mem->source_agents[i].character.row);
            mem->source_agents[i].character.row = row;
            mem->source_agents[i].character.column = column;
            increments_population_in_cell(mem, column, row);
            mem->memory_has_changed = 1;
            printf("Source agent %d moved to %d %d\n", id, row, column);
            pthread_mutex_unlock(&mem->mutex);

            break;
        }
    }
}

void move_attending_officer(memory_t *mem, int row, int column) {
    pthread_mutex_lock(&mem->mutex);
    decrements_population_in_cell(mem, mem->attending_officers[0].character.column,
                                  mem->attending_officers[0].character.row);
    mem->attending_officers[0].character.row = row;
    mem->attending_officers[0].character.column = column;
    increments_population_in_cell(mem, column, row);
    mem->memory_has_changed = 1;
    pthread_mutex_unlock(&mem->mutex);
}

int is_valid_move(int column_end, int row_end, memory_t *mem) {
    if (column_end < 0 || column_end >= MAX_COLUMNS || row_end < 0 || row_end >= MAX_ROWS) {
        return 0;
    }
    if (mem->city_map.cells[row_end][column_end].type == RESIDENTIAL_BUILDING) {
        if (mem->city_map.cells[row_end][column_end].nb_of_characters >=
            MAX_NUMBER_OF_CHARACTERS_ON_RESIDENTIAL_BUILDING)
            return 0;
    }
    if (mem->city_map.cells[row_end][column_end].type == CITY_HALL) {
        if (mem->city_map.cells[row_end][column_end].nb_of_characters >= MAX_NUMBER_OF_CHARACTERS_ON_CITY_HALL)
            return 0;
    }
    if (mem->city_map.cells[row_end][column_end].type == COMPANY) {
        if (mem->city_map.cells[row_end][column_end].nb_of_characters >= MAX_NUMBER_OF_CHARACTERS_ON_COMPANY) return 0;
    }
    if (mem->city_map.cells[row_end][column_end].type == SUPERMARKET) {
        if (mem->city_map.cells[row_end][column_end].nb_of_characters >= MAX_NUMBER_OF_CHARACTERS_ON_SUPERMARKET)
            return 0;
    }
    return 1;
}

void *source_agent_thread_func(void *arg) {
    agent_thread_args_t *args = (agent_thread_args_t *) arg;
    int old_timer = args->mem->my_timer.turns;
    log_info("out the while");


    int next_row = 1;
    int next_column = 1;

    // pthread_mutex_lock(&mem->mutex);
    move_source_agent(args->mem, next_row, next_column, args->id);
    // pthread_mutex_unlock(&mem->mutex);
    // printf("Source agent %d moved to %d %d\n", args->id, next_row, next_column);

    // Mettez en pause le thread pour un certain temps si nécessaire

    sleep(1);

    // }
    pthread_exit(NULL);
}

void *attending_officer_thread_func(void *arg) {
    agent_thread_args_t *args = (agent_thread_args_t *) arg;
    printf("attending officer thread func\n");
    memory_t *mem = open_shared_memory();
    int old_timer = args->mem->my_timer.turns;
    while (!args->mem->simulation_has_ended) {
        // if (args->mem->my_timer.turns != old_timer) {
        old_timer = args->mem->my_timer.turns;
        int current_row = args->mem->attending_officers[args->id].character.row;
        int current_column = args->mem->attending_officers[args->id].character.column;

        int next_row = 1;
        int next_column = 1;

        // pthread_mutex_lock(&mem->mutex);
        move_attending_officer(args->mem, next_row, next_column);
        // pthread_mutex_unlock(&mem->mutex);
    }
    // Mettez en pause le thread pour un certain temps si nécessaire
    sleep(1);

    // }
    pthread_exit(NULL);
}

void create_and_run_source_agent_threads(memory_t *mem) {
    all_threads_t threads;

    for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; ++i) {
        threads.source_agent_args[i].id = mem->source_agents[i].character.id;
        threads.source_agent_args[i].mem = mem;

        // Créez le thread
        if (pthread_create(&threads.source_agent_threads[i], NULL, source_agent_thread_func,
                           (void *) &threads.attending_officer_args[i]) == 0) {
            printf("thread created\n");
        } else {
            printf("thread not created\n");
        }

    }


    for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; ++i) {
        printf("agent source in position : %d %d\n", mem->source_agents[i].character.row,
               mem->source_agents[i].character.column);
    }

    printf("threads created\n");

    // Joignez les threads après leur achèvement

}

void create_and_run_attending_officer_threads(memory_t *mem) {
    all_threads_t threads;

    for (int i = 0; i < MAX_ATTENDING_OFFICER_COUNT; ++i) {
        threads.attending_officer_args[i].id = mem->attending_officers[i].character.id;
        threads.attending_officer_args[i].mem = mem;

        // Créez le thread
        pthread_create(&threads.attending_officer_threads[i], NULL, attending_officer_thread_func,
                       (void *) &threads.attending_officer_args[i]);
    }


}




