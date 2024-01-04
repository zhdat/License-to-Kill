#include "tools.h"
#include "logger.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include "debug.h"

double euclidean_distance(int x1, int y1, int x2, int y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void increments_population_in_cell(memory_t* mem, int col, int row) {
    (mem->city_map.cells[col][row].nb_of_characters)++;
}

void decrements_population_in_cell(memory_t* mem, int col, int row) {
    (mem->city_map.cells[col][row].nb_of_characters)--;
}

enum cell_type_e is_cell_full(cell_t cells[7][7], int row, int col) {
    if (row < 0 || row >= MAX_ROWS || col < 0 || col >= MAX_COLUMNS) {
        return 1;
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
            return 0;
    }
}


void next_move(city_t* city, coordinate_t cell_start, coordinate_t cell_end, int* new_pos_col, int* new_pos_row,
               character_t* character) {
    // Supprimer l'id du personnage de la cellule actuelle
    /*int i;
    for (i = 0; i < city->cells[cell_start.row][cell_start.column].nb_of_characters; i++) {
        if (city->cells[cell_start.row][cell_start.column].ids[i] == character->id) {
            city->cells[cell_start.row][cell_start.column].ids[i] = -1;
            break;
        }
    }*/

    int step_row = (cell_end.row > cell_start.row) ? 1 : ((cell_end.row < cell_start.row) ? -1 : 0);
    int step_col = (cell_end.column > cell_start.column) ? 1 : ((cell_end.column < cell_start.column) ? -1 : 0);

    int current_row = cell_start.row, current_column = cell_start.column;

    // Essayez de bouger dans la direction principale
    if (!is_cell_full(city->cells, current_row + step_row, current_column + step_col)) {
        current_row += step_row;
        current_column += step_col;
    } else {
        // Essayez de bouger verticalement ou horizontalement si directement bloqué
        if (step_row != 0 && !is_cell_full(city->cells, current_row + step_row, current_column)) {
            current_row += step_row;
        } else if (step_col != 0 && !is_cell_full(city->cells, current_row, current_column + step_col)) {
            current_column += step_col;
        } else {
            // Prendre un détour
            bool detour_taken = false;
            for (int d_row = -1; d_row <= 1 && !detour_taken; d_row++) {
                for (int d_col = -1; d_col <= 1; d_col++) {
                    if (d_row != 0 || d_col != 0) { // Éviter la cellule actuelle
                        if (!is_cell_full(city->cells, current_row + d_row, current_column + d_col)) {
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

    // Mettre a jour les nouvelles positions
    *new_pos_row = current_row;
    *new_pos_col = current_column;

    // Stocker l'id du personnage dans la nouvelle cellule
    if (character != NULL) {
        city->cells[*new_pos_row][*new_pos_col].ids[city->cells[*new_pos_row][*new_pos_col].nb_of_characters] = character->id;
    }

    // mettre à jour les caméras
    detect_movement(city, *new_pos_row, *new_pos_col);
}


coordinate_t* findNeighbouringCells(city_t* city, int row, int col, int* neighbouring_cells_count) {
    coordinate_t* neighbouring_cells = (coordinate_t*) malloc(sizeof(coordinate_t) * 8);
    int count = 0;

    for (int d_row = -1; d_row <= 1; d_row++) {
        for (int d_col = -1; d_col <= 1; d_col++) {
            if (d_row != 0 || d_col != 0) { // Éviter la cellule actuelle
                if (!is_cell_full(city->cells, row + d_row, col + d_col)) {
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

// Fonction pour appliquer le chiffrement César
void caesarCipher(char* message, int shift) {
    for (int i = 0; message[i] != '\0'; ++i) {
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
    // Logique de déchiffrement
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
        //log_info("tentative de genération de message avec importance %d", importance);
        //log_info("Message généré: %s", bank->messages[importance][randomIndex]);
        return bank->messages[importance][randomIndex];
    } else {
        return "Aucun message disponible pour ce niveau de crucialité.";
    }
}

int selectRandomNumberUnder(int max) {
    return rand() % max;
}

int selectRandomPercentage(void) {
    return rand() % 101;
}
