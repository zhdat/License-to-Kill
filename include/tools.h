#ifndef PROJECT_TOOLS_H
#define PROJECT_TOOLS_H

#include "memory.h"

typedef struct thread_s thread_t;

struct thread_s {
    memory_t* mem;
    int id_character;
};

int euclidean_distance(int x1, int y1, int x2, int y2);

int calculate_minutes(int turn);

int calculate_hour(int turn);

int calculate_day(int turn);

void increments_population_in_cell(memory_t *mem, int col, int row);

void decrements_population_in_cell(memory_t *mem, int col, int row);

int is_cell_filled(cell_t cells[MAX_COLUMNS][MAX_ROWS], int row, int col);

void next_move(city_t* cells, coordinate_t cell_start, coordinate_t cell_end, int* new_pos_col, int* new_pos_row);

coordinate_t * findNeighbouringCells(city_t *city, int row, int col, int *neighbouring_cells_count);

#endif //PROJECT_TOOLS_H
