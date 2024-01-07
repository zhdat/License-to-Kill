/*
 * The License to Kill Project
 *
 * Copyright (C) 1995-2022 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef CELL_H
#define CELL_H

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "debug.h"

/**
 * \file cell.h
 *
 * \brief Constants and prototypes of the city structure.
 *
 *
 * \details Defines a cell within the grid map of the city. The cell may contains:
 * - Nothing (\e WASTELAND);
 * - A residential building (\e RESIDENTIAL_BUILDING);
 * - The City Hall (\e CITY_HALL);
 * - A company (\e COMPANY);
 * - SUPERMARKET (\e SUPERMARKET).
 */

/**
 * The type of cells within the city map.
 */
enum cell_type_e {
    WASTELAND,              /*!< An empty cell. */
    RESIDENTIAL_BUILDING, /*!< A cell that contains a residential building. */
    CITY_HALL,              /*!< A cell that contains the City Hall. */
    COMPANY,              /*!< A cell that contains a company. */
    SUPERMARKET              /*!< A cell that contains a supermarket. */
};

#define NUMBER_OF_ROWS 7
#define NUMBER_OF_COLUMNS 7

#define MAX_NUMBER_OF_CHARACTERS_ON_WASTELAND 999
#define MAX_NUMBER_OF_CHARACTERS_ON_RESIDENTIAL_BUILDING 15
#define MAX_NUMBER_OF_CHARACTERS_ON_CITY_HALL 20
#define MAX_NUMBER_OF_CHARACTERS_ON_COMPANY 50
#define MAX_NUMBER_OF_CHARACTERS_ON_SUPERMARKET 30

#define NUMBER_OF_RESIDENTIAL_BUILDINGS 11
#define NUMBER_OF_COMPANIES 8
#define NUMBER_OF_SUPERMARKETS 2
#define NUMBER_OF_CITY_HALLS 1

typedef enum cell_type_e cell_type_t;
typedef struct cell_s cell_t;
typedef struct city_s city_t;

/**
 * \brief A cell within the map of the city.
 */
struct cell_s {
    int column;              /*!< Column of the cell. */
    int row;              /*!< Row of the cell. */
    cell_type_t type;      /*!< Type of the cell (\see \enum e_cell_type). */
    int nb_of_characters; /*!< Number of characters on the cell. */
};

/**
 * \brief The map of the city.
 */
struct city_s {
    int width;        /*!< Width of the map. */
    int height;        /*!< Height of the map. */
    cell_t cells[MAX_ROWS][MAX_COLUMNS]; /*!< Cells of the map. */
};

/**
 * \brief coordinates of a cell.
 */
typedef struct {
    int column;
    int row;
} coordinate_t;


/**
 * \brief Creates a new city.
 *
 * \param width The width of the city.
 * \param height The height of the city.
 * @return A pointer to the new city.
 */
city_t* create_city(int width, int height);

/**
 * \brief Prints the layout of the city.
 *
 * Each type of cell is represented by a different character:
 * - Wasteland: 'W'
 * - Residential Building: 'R'
 * - City Hall: 'C'
 * - Company: 'O'
 * - Supermarket: 'S'
 *
 * \param city Pointer to the city to be printed.
 */
void print_city(city_t* city);

/**
 * \brief Retrieves a specific cell from the city.
 *
 * Returns a pointer to the cell at the specified coordinates.
 *
 * \param city Pointer to the city structure.
 * \param x The x-coordinate (column) of the cell.
 * \param y The y-coordinate (row) of the cell.
 * @return A pointer to the cell at the specified coordinates, or NULL if the coordinates are invalid.
 */
cell_t* get_cell(city_t* city, int x, int y);

/**
 * \brief Defines the number of characters in a specific cell for monitoring purposes.
 *
 * \param city Pointer to the city structure.
 * \param x The x-coordinate (column) of the cell.
 * \param y The y-coordinate (row) of the cell.
 * \param nb_of_characters The number of characters to assign to the cell.
 */
void define_monitoring(city_t* city, int x, int y, int nb_of_characters);

/**
 * \brief Clears the city, resetting all cells to WASTELAND and character count to 0.
 *
 * \param city Pointer to the city structure.
 */
void clear_city(city_t* city);

/**
 * \brief Initializes the city with a predefined layout.
 *
 * Sets the types for all cells in the city based on a predefined layout.
 *
 * \param city Pointer to the city structure.
 */
void init_city(city_t* city);

/**
 * \brief Determines if a cell should be under surveillance based on its type.
 *
 * \param cell_type The type of the cell.
 * @return 1 if the cell should be monitored, 0 otherwise.
 */
int should_be_monitored(cell_type_t cell_type);

/**
 * \brief Initializes the surveillance system for the city.
 *
 * Sets up surveillance for cells that require monitoring.
 *
 * \param city Pointer to the city structure.
 */
void initialize_surveillance_system(city_t* city);

/**
 * \brief Finds cells of a specific type and returns their coordinates.
 *
 * \param city Pointer to the city structure.
 * \param building_type The type of building to find.
 * \param number_of_sea The number of buildings to find.
 * @return An array of coordinates for the found buildings, or NULL if not found.
 */
coordinate_t* findTypeOfBuilding(city_t* city, cell_type_t building_type, int number_of_searched_buildings);

#endif /* CELL_H */
