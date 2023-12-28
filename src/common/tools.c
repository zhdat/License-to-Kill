#include "tools.h"
#include "logger.h"
#include <math.h>
#include <stdbool.h>

int euclidean_distance(int x1, int y1, int x2, int y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void increments_population_in_cell(memory_t *mem, int col, int row) {
    (mem->city_map.cells[col][row].nb_of_characters)++;
}

void decrements_population_in_cell(memory_t *mem, int col, int row){
    (mem->city_map.cells[col][row].nb_of_characters)--;
}

int is_cell_filled(cell_t cells[MAX_ROWS][MAX_COLUMNS], int row, int col) {
    if (row < 0 || row >= MAX_ROWS || col < 0 || col >= MAX_COLUMNS) {
        return 1; // Retourner vrai si en dehors des limites, pour éviter le déplacement
    }

    switch (cells[row][col].type) {
        case CITY_HALL:
            return cells[row][col].nb_of_characters >= MAX_NUMBER_OF_CHARACTERS_ON_CITY_HALL;
        case COMPANY:
            return cells[row][col].nb_of_characters >= MAX_NUMBER_OF_CHARACTERS_ON_COMPANY;
        case RESIDENTIAL_BUILDING:
            return cells[row][col].nb_of_characters >= MAX_NUMBER_OF_CHARACTERS_ON_RESIDENTIAL_BUILDING;
        case SUPERMARKET:
            return cells[row][col].nb_of_characters >= MAX_NUMBER_OF_CHARACTERS_ON_SUPERMARKET;
        default:
            return 0; // Si aucun type n'est spécifié, considérer comme non rempli
    }
}


void next_move(city_t* city, coordinate_t cell_start, coordinate_t cell_end, int* new_pos_col, int* new_pos_row) {
    int step_row = (cell_end.row > cell_start.row) ? 1 : ((cell_end.row < cell_start.row) ? -1 : 0);
    int step_col = (cell_end.column > cell_start.column) ? 1 : ((cell_end.column < cell_start.column) ? -1 : 0);

    int current_row = cell_start.row, current_column = cell_start.column;

    // Essayez de bouger dans la direction principale
    if (!is_cell_filled(city->cells, current_row + step_row, current_column + step_col)) {
        current_row += step_row;
        current_column += step_col;
    } else {
        // Essayez de bouger verticalement ou horizontalement si directement bloqué
        if (step_row != 0 && !is_cell_filled(city->cells, current_row + step_row, current_column)) {
            current_row += step_row;
        } else if (step_col != 0 && !is_cell_filled(city->cells, current_row, current_column + step_col)) {
            current_column += step_col;
        } else {
            // Prendre un détour
            bool detour_taken = false;
            for (int d_row = -1; d_row <= 1 && !detour_taken; d_row++) {
                for (int d_col = -1; d_col <= 1; d_col++) {
                    if (d_row != 0 || d_col != 0) { // Éviter la cellule actuelle
                        if (!is_cell_filled(city->cells, current_row + d_row, current_column + d_col)) {
                            current_row += d_row;
                            current_column += d_col;
                            detour_taken = true;
                            break;
                        }
                    }
                }
            }
        }
    }

    // Mettre à jour les nouvelles positions
    *new_pos_row = current_row;
    *new_pos_col = current_column;
}

