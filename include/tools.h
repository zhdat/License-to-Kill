#ifndef PROJECT_TOOLS_H
#define PROJECT_TOOLS_H

#include "memory.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_PATH_LENGTH 1000
#define CITY_SIZE 7

typedef struct thread_s thread_t;
typedef struct queue_node_s queue_node_t;
typedef struct queue_s queue_t;

struct thread_s {
    memory_t* mem;
    int id_character;
};

// Structure for queue node

#define MAX_QUEUE_SIZE 1000 // Taille maximale de la queue

struct queue_node_s{
    coordinate_t cell;
};

struct queue_s{
    queue_node_t nodes[MAX_QUEUE_SIZE];
    int front; // Indice du premier élément
    int rear;  // Indice du dernier élément
    int size;  // Nombre d'éléments actuels dans la queue
};

void create_queue(queue_t *q);

int en_queue(queue_t *q, coordinate_t cell);

coordinate_t de_queue(queue_t *q);

int is_queue_empty(queue_t *q);

int bfs_find_path(city_t *city, coordinate_t start, coordinate_t end, coordinate_t *path);

//

double euclidean_distance(int x1, int y1, int x2, int y2);

int calculate_minutes(int turn);

int calculate_hour(int turn);

int calculate_day(int turn);

void increments_population_in_cell(memory_t* mem, int col, int row);

void decrements_population_in_cell(memory_t* mem, int col, int row);

coordinate_t* findNeighbouringCells(city_t* city, int row, int col, int* neighbouring_cells_count);

void find_shortest_path(city_t* city, coordinate_t start, coordinate_t end, coordinate_t* path, int* path_length,
                        character_t character);

void next_move(city_t* city, coordinate_t cell_start, coordinate_t cell_end, int* new_pos_col, int* new_pos_row,
               character_t character);

int is_cell_accessible(cell_t cells[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS], int row, int col, character_t character);

//

int is_same_cell(coordinate_t cell1, coordinate_t cell2);

int is_at_work(character_t character);

int is_at_home(character_t character);

int character_is_at(character_t character1, coordinate_t cell2);

int characters_are_at_same_cell(character_t character1, character_t character2);

//

void caesarCipher(char* message, int shift);

void decrpyt_message(char* message, int shift);

MessageBank setMessageBank(void);

char* generateSpyMessage(MessageBank* bank, InformationCruciality importance);

int selectRandomNumberUnder(int max);

int selectRandomPercentage(void);

#endif //PROJECT_TOOLS_H
