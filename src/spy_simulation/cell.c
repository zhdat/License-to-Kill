#include "cell.h"
#include <pthread.h>

pthread_mutex_t city_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * \file cell.c
 *
 * Defines a cell within the grid map of the city. The cell may contains:
 * - Nothing (\e WASTELAND);
 * - A residential building (\e RESIDENTIAL_BUILDING);
 * - The City Hall (\e CITY_HALL);
 * - A company (\e COMPANY);
 * - SUPERMARKET (\e SUPERMARKET).
 */

// DEBUG
void print_city(city_t* city) {
    if (city == NULL) {
        printf("Error: city is NULL in print_city\n");
        return;
    }
    for (int i = 0; i < city->height; i++) {
        for (int j = 0; j < city->width; j++) {
            switch (city->cells[j][i].type) {
                case WASTELAND:
                    printf("W");
                    break;
                case RESIDENTIAL_BUILDING:
                    printf("R");
                    break;
                case CITY_HALL:
                    printf("C");
                    break;
                case COMPANY:
                    printf("O");
                    break;
                case SUPERMARKET:
                    printf("S");
                    break;
                default:
                    printf("?");
                    break;
            }
        }
        printf("\n");
    }
}


cell_t* get_cell(city_t* city, int x, int y) {
    if (city == NULL || x < 0 || x >= city->width || y < 0 || y >= city->height) {
#if DEBUG
        printf("Error: invalid parameters in get_cell\n");
#endif
        return NULL;
    }
    return &city->cells[x][y];
}

void define_monitoring(city_t* city, int x, int y, int nb_of_characters) {
    cell_t* cell;
    cell = get_cell(city, x, y);
    if (cell != NULL) {
        cell->nb_of_characters = nb_of_characters;
    }
}

void clear_city(city_t* city) {
    for (int i = 0; i < city->height; i++) {
        for (int j = 0; j < city->width; j++) {
            city->cells[j][i].type = WASTELAND;
            city->cells[j][i].nb_of_characters = 0;
        }
    }
}

void init_city(city_t* city) {
    city->width = 7;
    city->height = 7;

    city->cells[0][0].type = WASTELAND;
    city->cells[0][1].type = RESIDENTIAL_BUILDING;
    city->cells[0][2].type = WASTELAND;
    city->cells[0][3].type = WASTELAND;
    city->cells[0][4].type = WASTELAND;
    city->cells[0][5].type = WASTELAND;
    city->cells[0][6].type = RESIDENTIAL_BUILDING;

    city->cells[1][0].type = COMPANY;
    city->cells[1][1].type = WASTELAND;
    city->cells[1][2].type = RESIDENTIAL_BUILDING;
    city->cells[1][3].type = WASTELAND;
    city->cells[1][4].type = RESIDENTIAL_BUILDING;
    city->cells[1][5].type = WASTELAND;
    city->cells[1][6].type = COMPANY;

    city->cells[2][0].type = WASTELAND;
    city->cells[2][1].type = SUPERMARKET;
    city->cells[2][2].type = WASTELAND;
    city->cells[2][3].type = WASTELAND;
    city->cells[2][4].type = COMPANY;
    city->cells[2][5].type = COMPANY;
    city->cells[2][6].type = WASTELAND;

    city->cells[3][0].type = COMPANY;
    city->cells[3][1].type = WASTELAND;
    city->cells[3][2].type = RESIDENTIAL_BUILDING;
    city->cells[3][3].type = CITY_HALL;
    city->cells[3][4].type = SUPERMARKET;
    city->cells[3][5].type = WASTELAND;
    city->cells[3][6].type = WASTELAND;

    city->cells[4][0].type = WASTELAND;
    city->cells[4][1].type = WASTELAND;
    city->cells[4][2].type = WASTELAND;
    city->cells[4][3].type = COMPANY;
    city->cells[4][4].type = WASTELAND;
    city->cells[4][5].type = RESIDENTIAL_BUILDING;
    city->cells[4][6].type = RESIDENTIAL_BUILDING;

    city->cells[5][0].type = WASTELAND;
    city->cells[5][1].type = RESIDENTIAL_BUILDING;
    city->cells[5][2].type = WASTELAND;
    city->cells[5][3].type = WASTELAND;
    city->cells[5][4].type = WASTELAND;
    city->cells[5][5].type = WASTELAND;
    city->cells[5][6].type = WASTELAND;

    city->cells[6][0].type = COMPANY;
    city->cells[6][1].type = WASTELAND;
    city->cells[6][2].type = RESIDENTIAL_BUILDING;
    city->cells[6][3].type = WASTELAND;
    city->cells[6][4].type = RESIDENTIAL_BUILDING;
    city->cells[6][5].type = COMPANY;
    city->cells[6][6].type = RESIDENTIAL_BUILDING;
}

int should_be_monitored(cell_type_t cell_type) {
    switch (cell_type) {
        case COMPANY:
        case CITY_HALL:
            return 1;
        default:
            return 0;
    }
}

void initialize_surveillance_system(city_t* city) {
    if (city == NULL) {
#if DEBUG
        printf("Error: city is NULL in initialize_surveillance_system\n");
#endif
        return;
    }

    for (int i = 0; i < city->height; i++) {
        for (int j = 0; j < city->width; j++) {
            cell_t* cell = &city->cells[j][i];
            if (should_be_monitored(cell->type)) {
                cell->nb_of_characters = 1;
            }
        }
    }
}


coordinate_t* findTypeOfBuilding(city_t* city, cell_type_t building_type, int number_of_searched_buildings) {
    if (city == NULL || number_of_searched_buildings <= 0) {
        return NULL;
    }

    coordinate_t* coordinates = malloc(number_of_searched_buildings * sizeof(coordinate_t));

    if (coordinates == NULL) {
        return NULL;
    }

    int number_of_found_buildings = 0;
    for (int i = 0; i < city->width; i++) {
        for (int j = 0; j < city->height; j++) {
            if (city->cells[j][i].type == building_type) {
                coordinates[number_of_found_buildings].row = i;
                coordinates[number_of_found_buildings].column = j;
                ++number_of_found_buildings;

                if (number_of_found_buildings >= number_of_searched_buildings) {
                    return coordinates;
                }
            }
        }
    }

    return coordinates;
}

