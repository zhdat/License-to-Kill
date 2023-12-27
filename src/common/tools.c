#include "tools.h"
#include <math.h>

int euclidean_distance(int x1, int y1, int x2, int y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void increments_population_in_cell(memory_t *mem, int col, int row) {
    (mem->city_map.cells[col][row].nb_of_characters)++;
}

void decrements_population_in_cell(memory_t *mem, int col, int row){
    (mem->city_map.cells[col][row].nb_of_characters)--;
}

int is_cell_filled(cell_t cells[MAX_COLUMNS][MAX_ROWS], int row, int col) {
    if (row < 0 || row >= MAX_ROWS) {
        return 0;
    }

    if (col < 0 || col >= MAX_ROWS) {
        return 0;
    }

    if (cells[col][row].type == CITY_HALL) {
        return MAX_NUMBER_OF_CHARACTERS_ON_CITY_HALL == cells[col][row].nb_of_characters;
    }
    if (cells[col][row].type == COMPANY) {
        return MAX_NUMBER_OF_CHARACTERS_ON_COMPANY == cells[col][row].nb_of_characters;
    }
    if (cells[col][row].type == RESIDENTIAL_BUILDING) {
        return MAX_NUMBER_OF_CHARACTERS_ON_RESIDENTIAL_BUILDING == cells[col][row].nb_of_characters;
    }
    if (cells[col][row].type == SUPERMARKET) {
        return MAX_NUMBER_OF_CHARACTERS_ON_SUPERMARKET == cells[col][row].nb_of_characters;
    }
    return 0;
}

void next_move(cell_t cells[MAX_COLUMNS][MAX_ROWS], cell_t cell_start, cell_t cell_end, int speed, int* new_pos_col,
               int* new_pos_row) {
    int step_row = (cell_end.row > cell_start.row) ? 1 : -1;
    int step_col = (cell_end.column > cell_start.column) ? 1 : -1;

    int i = cell_start.row;
    int j = cell_start.column;

    int current_i = i, current_j = j;

    for (int distance = 0; distance < speed; distance++) {
        if (i != cell_end.row) {
            current_i = i + step_row;
        }

        if (j != cell_end.column) {
            current_j = j + step_col;
        }

        if (is_cell_filled(cells, current_i, current_j)) {
            /* If final cell is full */
            if (current_i == cell_end.row && current_j == cell_end.column) {
                break;
            } else {
                /* If the movement is a diagonal */
                if (current_i != i && current_j != j) {
                    int without_i = euclidean_distance(current_i - step_row, current_j, cell_end.row, cell_end.column);
                    int without_j = euclidean_distance(current_i, current_j - step_col, cell_end.row, cell_end.column);

                    if (without_i < without_j) {
                        current_j -= step_col;
                    } else {
                        current_i -= step_row;
                    }
                }
                    /* If the movement is a row */
                else if (current_i != i) {
                    if (!is_cell_filled(cells, current_i, current_j + 1)) {
                        current_j++;
                    } else if (!is_cell_filled(cells, current_i, current_j - 1)) {
                        current_j--;
                    } else {
                        break;
                    }
                }
                    /* If the movement is a column */
                else {
                    if (!is_cell_filled(cells, current_i + 1, current_j)) {
                        current_i++;
                    } else if (!is_cell_filled(cells, current_i - 1, current_j)) {
                        current_i--;
                    } else {
                        break;
                    }
                }
            }
        }

        i = current_i;
        j = current_j;
    }

    // Mettre à jour les nouvelles positions si nous ne sommes pas à la destination
    if (i != cell_end.row || j != cell_end.column) {
        *new_pos_col = j;
        *new_pos_row = i;
    } else { // Si nous sommes arrivés à la destination
        *new_pos_col = cell_end.column;
        *new_pos_row = cell_end.row;
    }
}
