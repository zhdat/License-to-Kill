#ifndef PROJECT_TOOLS_H
#define PROJECT_TOOLS_H

#include "memory.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct thread_s thread_t;

struct thread_s {
    memory_t *mem;
    int id_character;
};

double euclidean_distance(int x1, int y1, int x2, int y2);

int calculate_minutes(int turn);

int calculate_hour(int turn);

int calculate_day(int turn);

void increments_population_in_cell(memory_t *mem, int col, int row);

void decrements_population_in_cell(memory_t *mem, int col, int row);

coordinate_t *findNeighbouringCells(city_t *city, int row, int col, int *neighbouring_cells_count);

void find_shortest_path(city_t *city, coordinate_t start, coordinate_t end, coordinate_t *path, int *path_length,
                        character_t character);

void next_move(city_t *city, coordinate_t cell_start, coordinate_t cell_end, int *new_pos_col, int *new_pos_row,
               character_t character);

//

int is_same_cell(coordinate_t cell1, coordinate_t cell2);

int is_at_work(character_t character);

int is_at_home(character_t character);

int character_is_at(character_t character1, coordinate_t cell2);

int characters_are_at_same_cell(character_t character1, character_t character2);

//

void caesarCipher(char *message, int shift);

void decrpyt_message(char *message, int shift);

MessageBank setMessageBank(void);

char *generateSpyMessage(MessageBank *bank, InformationCruciality importance);

int selectRandomNumberUnder(int max);

int selectRandomPercentage(void);

#endif //PROJECT_TOOLS_H
