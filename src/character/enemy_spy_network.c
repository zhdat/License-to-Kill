#include <stdlib.h>
#include <unistd.h>
#include "enemy_spy_network.h"


void move_source_agent(memory_t *mem, int row, int column, int id) {
    for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; ++i) {
        if (mem->source_agents[i].character.id == id) {
            pthread_mutex_lock(&mem->mutex);
            decrements_population_in_cell(mem, mem->source_agents[i].character.column,
                                          mem->source_agents[i].character.row);
            mem->source_agents[i].character.row = row;
            mem->source_agents[i].character.column = column;
            increments_population_in_cell(mem, column, row);
            mem->memory_has_changed = 1;
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
    // int old_timer = args->mem->my_timer.turns;



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
    memory_t *mem = open_shared_memory();
    // int old_timer = args->mem->my_timer.turns;
    while (!args->mem->simulation_has_ended) {
        // if (args->mem->my_timer.turns != old_timer) {
        // old_timer = args->mem->my_timer.turns;
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

void create_and_run_source_agent_threads(memory_t *mem, all_threads_t *threads) {
    pthread_attr_t attr;
    agent_thread_args_t * ptr;
    for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; ++i) {
        ptr = &threads->source_agent_args[i];
        threads->source_agent_args[i].id = mem->source_agents[i].character.id;
        printf("id : %d\n", threads->source_agent_args[i].id);
        threads->source_agent_args[i].mem = mem;

        // Créez le thread
        pthread_attr_init(&attr);
        if (pthread_create(&threads->source_agent_threads[i], &attr, source_agent_thread_func,
                           ptr) == 0) {
            printf("thread created\n");
        } else {
            printf("thread not created\n");
        }

    }

    // Joignez les threads après leur achèvement

}

void create_and_run_attending_officer_threads(memory_t *mem, all_threads_t *threads) {
    pthread_attr_t attr;
    agent_thread_args_t * ptr;

    for (int i = 0; i < MAX_ATTENDING_OFFICER_COUNT; ++i) {
        ptr = &threads->source_agent_args[i];
        threads->attending_officer_args[i].id = mem->attending_officers[i].character.id;
        threads->attending_officer_args[i].mem = mem;

        // Créez le thread
        pthread_attr_init(&attr);
        pthread_create(&threads->attending_officer_threads[i], &attr, attending_officer_thread_func,
                       ptr);
    }



}




