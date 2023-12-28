#include <stdlib.h>
#include <unistd.h>
#include "enemy_spy_network.h"

volatile int signal_received_spies[MAX_SOURCE_AGENT_COUNT] = {0,0,0};
volatile int signal_received_officer = 0;


void move_source_agent(memory_t* mem, int row, int column, int id) {
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

void move_attending_officer(memory_t* mem, int row, int column) {
    pthread_mutex_lock(&mem->mutex);
    decrements_population_in_cell(mem, mem->attending_officers[0].character.column,
                                  mem->attending_officers[0].character.row);
    mem->attending_officers[0].character.row = row;
    mem->attending_officers[0].character.column = column;
    increments_population_in_cell(mem, column, row);
    mem->memory_has_changed = 1;
    pthread_mutex_unlock(&mem->mutex);
}

int is_valid_move(int column_end, int row_end, memory_t* mem) {
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



    while (args->mem->simulation_has_ended == 0) {
        if (signal_received_spies[(args->id - 1)]) {

            int current_row = args->mem->source_agents[args->id].character.row;
            int current_column = args->mem->source_agents[args->id].character.column;

            int next_row = rand()%7;
            int next_column = rand()%7;

            move_source_agent(args->mem, next_row, next_column, args->id);
            signal_received_spies[(args->id - 1)] = 0;
        }
    }


    // Mettez en pause le thread pour un certain temps si nécessaire

    sleep(1);

    // }
    pthread_exit(NULL);
}

void *attending_officer_thread_func(void *arg) {
    agent_thread_args_t *args = (agent_thread_args_t *) arg;
    while (!args->mem->simulation_has_ended) {
        if(signal_received_officer) {
            int current_row = args->mem->attending_officers[args->id].character.row;
            int current_column = args->mem->attending_officers[args->id].character.column;

            int next_row = rand()%7;
            int next_column = rand()%7;
            move_attending_officer(args->mem, next_row, next_column);
            signal_received_officer = 0;
        }

    }
    // Mettez en pause le thread pour un certain temps si nécessaire
    sleep(1);

    // }
    pthread_exit(NULL);
}

void handle_signal(int sig)
{
    for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; ++i) {
        signal_received_spies[i] = 1;
    }
    signal_received_officer = 1;
}

void set_signals(void) {
    struct sigaction action;

    action.sa_handler = handle_signal;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);

    sigaction(SIGALRM, &action, NULL);
}


void create_and_run_source_agent_threads(memory_t *mem, all_threads_t *threads) {
    pthread_attr_t attr;
    agent_thread_args_t *ptr;


    for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; ++i) {
        ptr = &threads->source_agent_args[i];
        threads->source_agent_args[i].id = mem->source_agents[i].character.id;
        threads->source_agent_args[i].mem = mem;

        pthread_attr_init(&attr);
        if (pthread_create(&threads->source_agent_threads[i], &attr, source_agent_thread_func,
                           ptr) == 0) {
        } else {
            printf("thread not created\n");
        }
        sleep(1);

    }
}

void create_and_run_attending_officer_threads(memory_t* mem, all_threads_t* threads) {
    pthread_attr_t attr;
    agent_thread_args_t *ptr;

    for (int i = 0; i < MAX_ATTENDING_OFFICER_COUNT; ++i) {
        ptr = &threads->source_agent_args[i];
        threads->attending_officer_args[i].id = mem->attending_officers[i].character.id;
        threads->attending_officer_args[i].mem = mem;

        pthread_attr_init(&attr);
        if (pthread_create(&threads->attending_officer_threads[i], &attr, attending_officer_thread_func,
                           ptr) == 0) {
        } else {
            printf("thread not created\n");
        }
        sleep(1);
    }


}




