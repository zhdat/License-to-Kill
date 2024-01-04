#include "cell.h"
#include <stdio.h>
#include <stdlib.h>
#include "debug.h"

void delete_city(city_t* city) {
    int i;
    if (city != NULL) {
        for (i = 0; i < city->height; i++) {
            free(city->cells[i]);
        }
        //free(city->cells);
        free(city);
    }
}

void print_city(city_t* city) {
    int i, j;
    if (city == NULL) {
        printf("Error: city is NULL in print_city\n");
        return;
    }
    for (i = 0; i < city->height; i++) {
        for (j = 0; j < city->width; j++) {
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

void print_city_with_characters(city_t* city) {
    int i, j;
    if (city == NULL) {
        printf("Error: city is NULL in print_city\n");
        exit(0);
    }
    for (i = 0; i < city->width; i++) {
        for (j = 0; j < city->height; j++) {
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
            if (city->cells[i][j].nb_of_characters > 0) {
                printf("[%d]", city->cells[i][j].nb_of_characters);
            } else {
                printf(" ");
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

#if DEBUG
    printf("Init city...\n");
#endif

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

    // Initialize camera and lidar
    initialize_cameras(city);
}

int should_be_monitored(cell_type_t cell_type) {
    switch (cell_type) {
        case COMPANY:
        case CITY_HALL:
            return 1; // Surveillance accrue pour les bâtiments importants
        default:
            return 0; // Pas de surveillance ou surveillance minimale
    }
}


// Function to find cells of a specific type and return their coordinates
coordinate_t* findTypeOfBuilding(city_t* city, cell_type_t building_type, int count) {
    if (city == NULL || count <= 0) {
        return NULL; // Null check for city and check for non-positive count
    }

    coordinate_t* coordinates = malloc(count * sizeof(coordinate_t));
    if (coordinates == NULL) {
        return NULL; // Memory allocation check
    }

    int found = 0; // Counter for found buildings
    for (int i = 0; i < city->width; ++i) {
        for (int j = 0; j < city->height; ++j) {
            if (city->cells[j][i].type == building_type) {
                coordinates[found].row = i;
                coordinates[found].column = j;
                found++;

                if (found >= count) {
                    return coordinates; // Return early if all buildings are found
                }
            }
        }
    }

    return coordinates; // Return the found coordinates (may be fewer than count if not all are found)
}

void activate_camera(city_t* city, int x, int y) {
    cell_t* cell = get_cell(city, x, y);
    if (cell != NULL) {
        cell->sensor_data.camera_active = 1;
    }
}

void activate_lidar(city_t* city, int x, int y) {
    cell_t* cell = get_cell(city, x, y);
    if (cell != NULL) {
        cell->sensor_data.lidar_active = 1;
    }
}

void detect_movement(city_t* city, int x, int y) {
    cell_t* cell = get_cell(city, x, y);
    if (cell == NULL || !cell->sensor_data.camera_active) {
        return;
    }

    // Conditions pour déterminer un mouvement suspect
    // Exemple: Un personnage reste trop longtemps dans une entreprise ou l'hôtel de ville
    if ((cell->type == COMPANY || cell->type == CITY_HALL) && cell->nb_of_characters > 0) {
        // Supposons que chaque appel à cette fonction représente une unité de temps
        cell->sensor_data.detected_time++;

        if (cell->sensor_data.detected_time > SOME_SUSPICIOUS_TIME_THRESHOLD) {
            cell->sensor_data.has_motion = 1;
        }
    } else {
        // Réinitialiser le compteur de temps si les conditions ne sont pas remplies
        cell->sensor_data.detected_time = 0;
        cell->sensor_data.has_motion = 0;
    }
}

void initialize_cameras(city_t* city) {
    if (city == NULL) {
        return;
    }
    for (int i = 0; i < city->height; i++) {
        for (int j = 0; j < city->width; j++) {
            cell_t* cell = &city->cells[j][i];
            if (should_be_monitored(cell->type)) {
                activate_camera(city, j, i);
                log_info("Camera activated at (%d, %d)", j, i);
            }
        }
    }
}


