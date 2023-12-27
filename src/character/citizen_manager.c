#include "citizen_manager.h"

void move_citizen(memory_t* mem, int row, int column, int id) {
    for (int i = 0; i < MAX_CITIZEN_COUNT; i++) {
        if (mem->citizens[i].id == id) {
            pthread_mutex_lock(&mem->mutex);
            decrements_population_in_cell(mem, mem->citizens[i].column, mem->citizens[i].row);
            mem->citizens[i].row = row;
            mem->citizens[i].column = column;
            increments_population_in_cell(mem, column, row);
            mem->memory_has_changed = 1;
            pthread_mutex_unlock(&mem->mutex);
            break;
        }
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

void* citizen_thread_func(void* arg) {
    citizen_thread_args_t* args = (citizen_thread_args_t*) arg;
    int end_column = 0;
    int end_row = 0;
    int start_column = 0;
    int start_row = 0;
    for (int i = 0; i < MAX_CITIZEN_COUNT; i++) {
        if (args->mem->citizens[i].id == args->id) {
            end_column = args->mem->citizens[i].work_column;
            end_row = args->mem->citizens[i].work_row;
            start_row = args->mem->citizens[i].row;
            start_column = args->mem->citizens[i].column;
            break;
        }
    }

    while (args->mem->simulation_has_ended == 0) {
        log_info("Citizen %d is waiting for the signal to move", args->id);
        sem_wait(&move_sem);
        log_info("Citizen %d received the signal to move", args->id);
        int current_column = start_column;
        int current_row = start_row;

        while (!is_valid_move(current_column, current_row, args->mem)) {
            if (current_column < end_column) {
                current_column++;
            } else if (current_column > end_column) {
                current_column--;
            } else if (current_row < end_row) {
                current_row++;
            } else if (current_row > end_row) {
                current_row--;
            }
        }
        move_citizen(args->mem, current_row, current_column, args->id);
    }
    sleep(1);
    pthread_exit(NULL);
}

void create_and_run_citizen_threads(memory_t* mem, all_threads_t* threads) {
    pthread_attr_t attr;
    citizen_thread_args_t* ptr;
    for (int i = 0; i < MAX_CITIZEN_COUNT; i++) {
        ptr = &threads->citizen_args[i];
        threads->citizen_args[i].id = mem->citizens[i].id;
        threads->citizen_args[i].mem = mem;

        pthread_attr_init(&attr);
        if (pthread_create(&threads->citizen_threads[i], &attr, citizen_thread_func, ptr) != 0) {
            perror("Error creating thread");
            exit(EXIT_FAILURE);
        } else {
        }
    }
}

