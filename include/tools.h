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

/**
 * \file tools.h
 * \brief Provides utility functions for the simulation, including distance calculations, cell population management, and character movements.
 */

/**
 * \brief Calculates the Euclidean distance between two points.
 *
 * \param x1 X-coordinate of the first point.
 * \param y1 Y-coordinate of the first point.
 * \param x2 X-coordinate of the second point.
 * \param y2 Y-coordinate of the second point.
 * @return The Euclidean distance between the two points.
 */
double euclidean_distance(int x1, int y1, int x2, int y2);

/**
 * \brief Increments the population count in a specified cell.
 *
 * \param mem Pointer to the shared memory structure.
 * \param col Column of the cell.
 * \param row Row of the cell.
 */
void increments_population_in_cell(memory_t *mem, int col, int row);

/**
 * \brief Decrements the population count in a specified cell.
 *
 * \param mem Pointer to the shared memory structure.
 * \param col Column of the cell.
 * \param row Row of the cell.
 */
void decrements_population_in_cell(memory_t *mem, int col, int row);

/**
 * \brief Checks if a cell is full based on its type and current population.
 *
 * \param cells The grid of cells.
 * \param row Row of the cell.
 * \param col Column of the cell.
 * @return 1 if the cell is full, 0 otherwise.
 */
enum cell_type_e is_cell_full(cell_t cells[7][7], int row, int col);

/**
 * \brief Determines the next move for a character towards a target cell.
 *
 * \param city Pointer to the city structure.
 * \param cell_start Starting coordinate.
 * \param cell_end Destination coordinate.
 * \param new_pos_col Pointer to store the new column position.
 * \param new_pos_row Pointer to store the new row position.
 */
void next_move(city_t *cells, coordinate_t cell_start, coordinate_t cell_end, int *new_pos_col, int *new_pos_row);

/**
 * \brief Finds neighbouring cells of a specific cell within the city.
 *
 * \param city Pointer to the city structure.
 * \param row Row of the target cell.
 * \param col Column of the target cell.
 * \param neighbouring_cells_count Pointer to store the count of neighbouring cells.
 * @return Pointer to an array of coordinates of neighbouring cells.
 */
coordinate_t *findNeighbouringCells(city_t *city, int row, int col, int *neighbouring_cells_count);


//

/**
 * \brief Determines if a cell is at the same position as another cell.
 *
 * \param cell1 First cell.
 * \param cell2 Second cell.
 * @return 1 if the cells are at the same position, 0 otherwise.
 */
int is_same_cell(coordinate_t cell1, coordinate_t cell2);

/**
 * \brief Determines if a character is at work.
 *
 * \param character The character.
 * @return 1 if the character is at work, 0 otherwise.
 */
int is_at_work(character_t character);

/**
 * \brief Determines if a character is at home.
 *
 * \param character The character.
 * @return 1 if the character is at home, 0 otherwise.
 */
int is_at_home(character_t character);

/**
 * \brief Determines if a character is at a specific cell.
 *
 * \param character1 The character.
 * \param cell2 The cell.
 * @return 1 if the character is at the cell, 0 otherwise.
 */
int character_is_at(character_t character1, coordinate_t cell2);

/**
 * \brief Determines if two characters are at the same cell.
 *
 * \param character1 The first character.
 * \param character2 The second character.
 * @return 1 if the characters are at the same cell, 0 otherwise.
 */
int characters_are_at_same_cell(character_t character1, character_t character2);

//

/**
 * \brief Encrypts a message using the Caesar cipher.
 *
 * \param message The message to be encrypted.
 * \param shift The shift value for the Caesar cipher.
 */
void caesarCipher(char *message, int shift);

/**
 * \brief Decrypts a message encrypted with the Caesar cipher.
 *
 * \param message The encrypted message to be decrypted.
 * \param shift The shift value used in the Caesar cipher.
 */
void decrpyt_message(char *message, int shift);

/**
 * \brief Initializes and returns a message bank for spy messages.
 *
 * @return The initialized MessageBank structure.
 */
MessageBank setMessageBank(void);

/**
 * \brief Generates a random spy message based on importance level.
 *
 * \param bank Pointer to the MessageBank structure.
 * \param importance The importance level of the message.
 * @return A randomly selected spy message of the given importance.
 */
char *generateSpyMessage(MessageBank *bank, InformationCruciality importance);

/**
 * \brief Selects a random number less than the specified maximum.
 *
 * \param max The maximum value for the random number.
 * @return A random number less than max.
 */
int selectRandomNumberUnder(int max);

/**
 * \brief Selects a random percentage (0 to 100).
 *
 * @return A random percentage.
 */
int selectRandomPercentage(void);

#endif //PROJECT_TOOLS_H
