/**
 * \file tools.h
 * \brief Header file containing declarations for utility functions used in the simulation.
 *
 * This file contains the declarations of utility functions such as distance calculations,
 * cell population management, and character movements, which are essential for the simulation.
 */

#ifndef PROJECT_TOOLS_H
#define PROJECT_TOOLS_H

#include "memory.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_PATH_LENGTH 100 /**< Maximum length of a path in the simulation */
#define CITY_SIZE 7 /**< Size of the city grid used in the simulation */
#define MAX_QUEUE_SIZE 100 /**< Maximum size of the queue used in the simulation */
#define INT_MAX 2147483647 /**< Maximum value of an integer */

typedef struct thread_s thread_t;

/**
 * @struct thread_s
 * @brief Structure representing a thread in the simulation.
 *
 * This structure holds information about a thread, including pointers to shared memory
 * and an identifier for the character managed by this thread.
 */
struct thread_s {
    memory_t* mem; /**< Pointer to shared memory */
    int id_character; /**< ID of the character managed by this thread */
};

/**
 * @brief Calculates the Euclidean distance between two points.
 *
 * @param x1 X-coordinate of the first point.
 * @param y1 Y-coordinate of the first point.
 * @param x2 X-coordinate of the second point.
 * @param y2 Y-coordinate of the second point.
 * @return double The Euclidean distance between the two points.
 */
double euclidean_distance(int x1, int y1, int x2, int y2);

/**
 * @brief Calculates the Manhattan distance between two points.
 *
 * @param x1 X-coordinate of the first point.
 * @param y1 Y-coordinate of the first point.
 * @param x2 X-coordinate of the second point.
 * @param y2 Y-coordinate of the second point.
 * @return int The Manhattan distance between the two points.
 */
int manhattan_distance(int x1, int y1, int x2, int y2);

/**
 * @brief Calculate the simulation time in minutes based on the turn number.
 *
 * @param turn The current turn number in the simulation.
 * @return int The corresponding number of minutes in the simulation.
 */
int calculate_minutes(int turn);

/**
 * @brief Calculate the simulation time in hours based on the turn number.
 *
 * @param turn The current turn number in the simulation.
 * @return int The corresponding number of hours in the simulation.
 */
int calculate_hour(int turn);

/**
 * @brief Calculate the simulation day based on the turn number.
 *
 * @param turn The current turn number in the simulation.
 * @return int The corresponding day number in the simulation.
 */
int calculate_day(int turn);

/**
 * @brief Increment the population count in a specific cell.
 *
 * @param mem Pointer to the shared memory structure.
 * @param col The column index of the cell.
 * @param row The row index of the cell.
 */
void increments_population_in_cell(memory_t* mem, int col, int row);

/**
 * @brief Decrement the population count in a specific cell.
 *
 * @param mem Pointer to the shared memory structure.
 * @param col The column index of the cell.
 * @param row The row index of the cell.
 */
void decrements_population_in_cell(memory_t* mem, int col, int row);

/**
 * @brief Find neighbouring cells around a given cell in the city grid.
 *
 * @param city Pointer to the city grid structure.
 * @param row The row index of the target cell.
 * @param col The column index of the target cell.
 * @param neighbouring_cells_count Pointer to store the count of neighbouring cells found.
 * @return coordinate_t* An array of neighbouring cell coordinates.
 */
coordinate_t* findNeighbouringCells(city_t* city, int row, int col, int* neighbouring_cells_count);

/**
 * @brief Determines the next move for a character based on the start and end cells.
 *
 * This function calculates the next position for a character to move towards the target
 * based on the current city layout. It ensures that the movement is valid and feasible
 * within the constraints of the city grid.
 *
 * @param city Pointer to the city grid structure.
 * @param cell_start The current position of the character.
 * @param cell_end The target position for the character.
 * @param new_pos_col Pointer to store the new column position.
 * @param new_pos_row Pointer to store the new row position.
 * @param character The character that is moving.
 */
void next_move(city_t* city, coordinate_t cell_start, coordinate_t cell_end, int* new_pos_col, int* new_pos_row,
               character_t character);

/**
 * @brief Checks if a cell is accessible for a given character.
 *
 * Determines whether a particular cell in the grid is accessible based on the character's
 * attributes and the properties of the cell.
 *
 * @param cells The grid of cells in the city.
 * @param row The row index of the cell to check.
 * @param col The column index of the cell to check.
 * @param character The character attempting to access the cell.
 * @return int Returns 1 if the cell is accessible, 0 otherwise.
 */
int is_cell_accessible(cell_t cells[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS], int row, int col, character_t character);

// Additional utility functions...

/**
 * @brief Checks if two cells are the same.
 *
 * This function compares two cell coordinates and determines if they represent the same cell.
 *
 * @param cell1 The first cell to compare.
 * @param cell2 The second cell to compare.
 * @return int Returns 1 if the cells are the same, 0 otherwise.
 */
int is_same_cell(coordinate_t cell1, coordinate_t cell2);

/**
 * @brief Checks if a character is at their place of work.
 *
 * This function determines if the character's current location matches their designated work location.
 *
 * @param character The character to check.
 * @return int Returns 1 if the character is at work, 0 otherwise.
 */
int is_at_work(character_t character);

/**
 * @brief Checks if a character is at their home.
 *
 * This function determines if the character's current location matches their home location.
 *
 * @param character The character to check.
 * @return int Returns 1 if the character is at home, 0 otherwise.
 */
int is_at_home(character_t character);

/**
 * @brief Checks if a character is at a specific cell.
 *
 * This function determines if the character's current location matches the specified cell coordinates.
 *
 * @param character1 The character to check.
 * @param cell2 The cell coordinates to compare against the character's location.
 * @return int Returns 1 if the character is at the specified cell, 0 otherwise.
 */
int character_is_at(character_t character1, coordinate_t cell2);

/**
 * @brief Checks if two characters are at the same cell.
 *
 * This function determines if two characters are located at the same cell coordinates.
 *
 * @param character1 The first character to check.
 * @param character2 The second character to check.
 * @return int Returns 1 if both characters are at the same cell, 0 otherwise.
 */
int characters_are_at_same_cell(character_t character1, character_t character2);

// Additional utility functions...

/**
 * @brief Encrypts a message using the Caesar cipher.
 *
 * @param message The message to be encrypted.
 * @param shift The number of positions each character in the message will be shifted.
 */
void caesarCipher(char* message, int shift);

/**
 * @brief Decrypts a message encrypted with the Caesar cipher.
 *
 * @param message The encrypted message to be decrypted.
 * @param shift The number of positions each character in the message will be shifted back.
 */
void decrpyt_message(char* message, int shift);

/**
 * @brief Sets up a bank of messages for use in the simulation.
 *
 * Initializes and returns a MessageBank structure containing predefined messages
 * categorized by their level of cruciality.
 *
 * @return MessageBank A structure containing arrays of messages.
 */
MessageBank setMessageBank(void);

/**
 * @brief Generates a spy message from the message bank based on the given cruciality.
 *
 * Selects and returns a message from the message bank corresponding to the specified
 * level of cruciality.
 *
 * @param bank Pointer to the message bank.
 * @param importance The level of cruciality of the message.
 * @return char* A randomly selected message of the specified cruciality.
 */
char* generateSpyMessage(MessageBank* bank, InformationCruciality importance);

/**
 * @brief Generates a random number less than the specified maximum value.
 *
 * @param max The maximum value (exclusive).
 * @return int A random number in the range [0, max).
 */
int selectRandomNumberUnder(int max);

/**
 * @brief Generates a random percentage (0 to 100 inclusive).
 *
 * @return int A random percentage value.
 */
int selectRandomPercentage(void);

#endif //PROJECT_TOOLS_H
