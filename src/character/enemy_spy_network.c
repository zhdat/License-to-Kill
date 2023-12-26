#include <stdlib.h>
#include <unistd.h>
#include "enemy_spy_network.h"

void move_source_agent(memory_t* mem, int row, int column, int id) {
    for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; ++i) {
        if (mem->source_agents[i].character.id == id) {
            decrements_population_in_cell(mem, mem->source_agents[i].character.column,
                                          mem->source_agents[i].character.row);
            mem->source_agents[i].character.row = row;
            mem->source_agents[i].character.column = column;
            printf("Source agent %d moved to (%d, %d)\n", id, column, row);
            break;
        }
    }
    increments_population_in_cell(mem, column, row);
}

void move_case_officer(memory_t* mem, int row, int column, int id) {
    if (is_valid_move(column, row, mem)) {
        for (int i = 0; i < MAX_ATTENDING_OFFICER_COUNT; ++i) {
            if (mem->attending_officers[i].character.id == id) {
                decrements_population_in_cell(mem, mem->attending_officers[i].character.column,
                                              mem->attending_officers[i].character.row);
                mem->attending_officers[i].character.row = row;
                mem->attending_officers[i].character.column = column;
                break;
            }
        }
        increments_population_in_cell(mem, column, row);
    }
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

void source_agent_thread_func(void* arg, memory_t* mem) {
    agent_thread_args_t* args = (agent_thread_args_t*) arg;
    int old_timer = args->mem->my_timer.turns;
    while (!args->mem->simulation_has_ended) {
        // if (args->mem->my_timer.turns != old_timer) {
        old_timer = args->mem->my_timer.turns;
        int current_row = args->mem->source_agents[args->id].character.row;
        int current_column = args->mem->source_agents[args->id].character.column;

        // Déterminez les positions possibles
        int positions[8][2] = {
                {current_row - 1, current_column},     // Nord
                {current_row - 1, current_column + 1}, // Nord-Est
                {current_row,     current_column + 1},     // Est
                {current_row + 1, current_column + 1}, // Sud-Est
                {current_row + 1, current_column},     // Sud
                {current_row + 1, current_column - 1}, // Sud-Ouest
                {current_row,     current_column - 1},     // Ouest
                {current_row - 1, current_column - 1}  // Nord-Ouest
        };

        // Choisissez une position au hasard
        int index = rand() % 8;
        int next_row = positions[index][0];
        int next_column = positions[index][1];

        // Vérifiez si le mouvement est valide
        while (!is_valid_move(next_column, next_row, args->mem)) {
            // Déplacez l'agent
            index = rand() % 8;
            next_row = positions[index][0];
            next_column = positions[index][1];
        }
        move_source_agent(args->mem, next_row, next_column, args->id);
        mem->memory_has_changed = 1;
        sleep(100000000);
    }
    // Mettez en pause le thread pour un certain temps si nécessaire

    // }
    pthread_exit(NULL);
}

void create_and_run_source_agent_threads(memory_t* mem) {
    pthread_t threads[MAX_SOURCE_AGENT_COUNT];
    agent_thread_args_t args[MAX_SOURCE_AGENT_COUNT];

    for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; ++i) {
        args[i].id = mem->source_agents[i].character.id;
        args[i].mem = mem;

        // Créez le thread
        pthread_create(&threads[i], NULL, (void* (*)(void*)) source_agent_thread_func, (void*) &args[i]);
    }

    // Joignez les threads après leur achèvement
    for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; ++i) {
        pthread_join(threads[i], NULL);
    }
}




