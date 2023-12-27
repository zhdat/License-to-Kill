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

void* citizen_thread_func(void* arg){
    citizen_thread_args_t* args = (citizen_thread_args_t*) arg;
    int next_row = 1;
    int next_column = 1;

    move_citizen(args->mem, next_row, next_column, args->id);
    sleep(1);
    pthread_exit(NULL);
}

void create_and_run_citizen_threads(memory_t* mem, all_threads_t* threads){
    pthread_attr_t attr;
    citizen_thread_args_t* ptr;
    for (int i = 0; i < MAX_CITIZEN_COUNT; i++){
        ptr = &threads->citizen_args[i];
        threads->citizen_args[i].id = mem->citizens[i].id;
        threads->citizen_args[i].mem = mem;

        pthread_attr_init(&attr);
        if (pthread_create(&threads->citizen_threads[i], &attr, citizen_thread_func, ptr) != 0){
            perror("Error creating thread");
            exit(EXIT_FAILURE);
        } else {
            printf("Thread created\n");
        }
    }
}

