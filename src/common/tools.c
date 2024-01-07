#include "tools.h"
#include "logger.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "debug.h"

/**
 * \file tools.c
 * \brief Provides utility functions for the simulation, including distance calculations, cell population management, and character movements.
 */

double euclidean_distance(int x1, int y1, int x2, int y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

int manhattan_distance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

void increments_population_in_cell(memory_t* mem, int col, int row) {
    (mem->city_map.cells[col][row].nb_of_characters)++;
}

void decrements_population_in_cell(memory_t* mem, int col, int row) {
    (mem->city_map.cells[col][row].nb_of_characters)--;
}

static int is_cell_valid(int row, int col) {
    return row >= 0 && row < MAX_ROWS && col >= 0 && col < MAX_COLUMNS;
}

static int is_cell_full(cell_t cells[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS], int row, int col) {
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
            return 0;
    }
}

static int
is_cell_authorised(cell_t cells[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS], int row, int col, character_t character) {
    coordinate_t home_cell = {character.home_row, character.home_column};
    coordinate_t work_cell = {character.work_row, character.work_column};
    coordinate_t moveto_cell = {row, col};

    if (cells[row][col].type == RESIDENTIAL_BUILDING && !is_same_cell(home_cell, moveto_cell)) {
        return 0;
    }
    if (cells[row][col].type == COMPANY && !is_same_cell(work_cell, moveto_cell)) {
        return 0;
    }
    return 1;
}

int is_cell_accessible(cell_t cells[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS], int row, int col, character_t character) {
    if (!is_cell_valid(row, col)) {
        return 0;
    }

    if (is_cell_full(cells, row, col)) {
        return 0;
    }

    if (!is_cell_authorised(cells, row, col, character)) {
        return 0;
    }

    return 1;
}

void next_move(city_t* city, coordinate_t cell_start, coordinate_t cell_end, int* new_pos_col, int* new_pos_row,
               character_t character) {
    int step_row = (cell_end.row > cell_start.row) ? 1 : ((cell_end.row < cell_start.row) ? -1 : 0);
    int step_col = (cell_end.column > cell_start.column) ? 1 : ((cell_end.column < cell_start.column) ? -1 : 0);

    int current_row = cell_start.row, current_column = cell_start.column;

    // Try to move diagonally
    if (is_cell_accessible(city->cells, current_row + step_row, current_column + step_col, character)) {
        current_row += step_row;
        current_column += step_col;
    } else {
        // Try to move vertically or horizontally
        if (step_row != 0 && is_cell_accessible(city->cells, current_row + step_row, current_column, character)) {
            current_row += step_row;
        } else if (step_col != 0 &&
                   is_cell_accessible(city->cells, current_row, current_column + step_col, character)) {
            current_column += step_col;
        } else {
            // Take a deviation
            int neighbouring_cells_count = 0;
            coordinate_t* neighbouring_cells = findNeighbouringCells(city, current_row, current_column,
                                                                     &neighbouring_cells_count);

            // Find the neighbouring cell that is the closest to the end cell
            int min_distance = INT_MAX;
            int min_distance_index = -1;

            for (int i = 0; i < neighbouring_cells_count; i++) {
                int distance = manhattan_distance(neighbouring_cells[i].row, neighbouring_cells[i].column,
                                                  cell_end.row, cell_end.column);
                if (distance < min_distance) {
                    min_distance = distance;
                    min_distance_index = i;
                }
            }

            if (min_distance_index != -1) {
                current_row = neighbouring_cells[min_distance_index].row;
                current_column = neighbouring_cells[min_distance_index].column;
            }

            free(neighbouring_cells);
        }
    }

    // Update the new position
    *new_pos_row = current_row;
    *new_pos_col = current_column;
}

coordinate_t* findNeighbouringCells(city_t* city, int row, int col, int* neighbouring_cells_count) {
    coordinate_t* neighbouring_cells = (coordinate_t*) malloc(sizeof(coordinate_t) * 8);
    int count = 0;

    for (int d_row = -1; d_row <= 1; d_row++) {
        for (int d_col = -1; d_col <= 1; d_col++) {
            if (d_row != 0 || d_col != 0) { // Avoid the current cell
                if (!is_cell_full(city->cells, row + d_row, col + d_col) && is_cell_valid(row + d_row, col + d_col)) {
                    neighbouring_cells[count].row = row + d_row;
                    neighbouring_cells[count].column = col + d_col;
                    count++;
                }
            }
        }
    }

    *neighbouring_cells_count = count;
    return neighbouring_cells;
}


int is_same_cell(coordinate_t cell1, coordinate_t cell2) {
    return cell1.row == cell2.row && cell1.column == cell2.column;
}

int is_at_work(character_t character) {
    return character.row == character.work_row && character.column == character.work_column;
}

int is_at_home(character_t character) {
    return character.row == character.home_row && character.column == character.home_column;
}

int character_is_at(character_t character1, coordinate_t cell2) {
    return character1.row == cell2.row && character1.column == cell2.column;
}

int characters_are_at_same_cell(character_t character1, character_t character2) {
    return character1.row == character2.row && character1.column == character2.column;
}

void caesarCipher(char* message, int shift) {
    for (int i = 0; message[i] != '\0'; i++) {
        char ch = message[i];

        if (ch >= 'a' && ch <= 'z') {
            ch = ch + shift;
            if (ch > 'z') {
                ch = ch - 'z' + 'a' - 1;
            }
            message[i] = ch;
        } else if (ch >= 'A' && ch <= 'Z') {
            ch = ch + shift;
            if (ch > 'Z') {
                ch = ch - 'Z' + 'A' - 1;
            }
            message[i] = ch;
        }
    }
}

void decrpyt_message(char* message, int shift) {
    int i = 0;
    while (message[i] != '\0') {
        if (message[i] >= 'a' && message[i] <= 'z') {
            message[i] = message[i] - shift;
            if (message[i] < 'a') {
                message[i] = message[i] + 'z' - 'a' + 1;
            }
        } else if (message[i] >= 'A' && message[i] <= 'Z') {
            message[i] = message[i] - shift;
            if (message[i] < 'A') {
                message[i] = message[i] + 'Z' - 'A' + 1;
            }
        }
        i++;
    }
}

MessageBank setMessageBank(void) {
    MessageBank bank = {
            .messages = {
                    [Crucial] = {"Plans secrets de tech. avancee obtenus. Urgence extreme.",
                                 "Fusion majeure detectee entre geants tech. Impact enorme.",
                                 "Echantillon materiel militaire revolutionnaire acquis.",
                                 "Acces serveurs financiers cles. Donnees critiques en main.",
                                 "Partenariat secret gouvernemental decouvert. Tres sensible."},
                    [Strong] = {"Failles financieres de la cible exposees. Grande valeur.",
                                "Plans d'expansion globale de la cible reveles. Important.",
                                "Strategie de contournement de brevet decouverte. Utile.",
                                "Liste de chercheurs influençables compilee. Opportunite.",
                                "Faiblesses de securite IT trouvees. Potentiel d'exploitation."},
                    [Medium] = {"Tendances de marche et previsions pour l'annee reçues.",
                                "Nouvelles politiques RH et effets notes. À surveiller.",
                                "Mises a niveau de production planifiees decouvertes.",
                                "Changements de gestion internes observes. À etudier.",
                                "Strategies a moyen terme de la direction saisies."},
                    [Low] = {"Procedures operationnelles standards collectees. Basique.",
                             "Contrats mineurs et partenariats etablis. Peu critique.",
                             "Moral des employes analyse. Potentiel d'influence mineur.",
                             "Gestion energie et dechets etudiee. Peu pertinent.",
                             "Performances mensuelles standard saisies. Info generale."},
                    [VeryLow] = {"Activites quotidiennes regulieres notees. Rien d'urgent.",
                                 "Programmes de formation et bien-etre releves. Basique.",
                                 "Details sur evenements sociaux de l'entreprise. Standard.",
                                 "Routines et horaires de travail observes. Rien de special.",
                                 "Activites de reseautage saisies. Valeur faible."}
            }
    };
    return bank;
}

char* generateSpyMessage(MessageBank* bank, InformationCruciality importance) {
    int messageCount = 0;
    while (bank->messages[importance][messageCount] != NULL && messageCount < MAX_MESSAGES) {
        messageCount++;
    }

    if (messageCount > 0) {
        int randomIndex = selectRandomNumberUnder(messageCount);
        return bank->messages[importance][randomIndex];
    } else {
        return "Aucun message disponible !";
    }
}

int selectRandomNumberUnder(int max) {
    return rand() % max;
}

int selectRandomPercentage(void) {
    return rand() % 101;
}
