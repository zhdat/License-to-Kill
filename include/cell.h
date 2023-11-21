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

#include "common.h"

/**
 * \file cell.h
 *
 * Defines a cell within the grid map of the city. The cell may contains:
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
	WASTELAND,			  /*!< An empty cell. */
	RESIDENTIAL_BUILDING, /*!< A cell that contains a residential building. */
	CITY_HALL,			  /*!< A cell that contains the City Hall. */
	COMPANY,			  /*!< A cell that contains a company. */
	SUPERMARKET			  /*!< A cell that contains a supermarket. */
};

typedef enum cell_type_e cell_type_t;
typedef struct cell_s cell_t;

/**
 * \brief A cell within the map of the city.
 */
struct cell_s {
	int column;			  /*!< Column of the cell. */
	int row;			  /*!< Row of the cell. */
	cell_type_t type;	  /*!< Type of the cell (@see \enum e_cell_type). */
	int nb_of_characters; /*!< Max. number of characters on the cell. */
};

typedef struct city_s city_t;

/**
 * \brief The map of the city.
 */
struct city_s {
	int width;		/*!< Width of the map. */
	int height;		/*!< Height of the map. */
	cell_t cells[MAX_COLUMNS][MAX_ROWS]; /*!< Cells of the map. */
};

/* Function to create a city */
city_t* create_city(int width, int height);
city_t* delete_city(city_t* city);
city_t* print_city(city_t* city);
cell_t* get_cell(city_t* city, int x, int y);
void define_monitoring(city_t* city, int x, int y, int nb_of_characters);
void clear_city(city_t* city);
void init_city(city_t* city);
int should_be_monitored(cell_type_t cell_type);
void initialize_surveillance_system(city_t* city);

#endif /* CELL_H */
