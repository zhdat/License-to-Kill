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
#include "logger.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

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

#define SOME_SUSPICIOUS_TIME_THRESHOLD 5

typedef enum cell_type_e cell_type_t;
typedef struct cell_s cell_t;
typedef struct city_s city_t;
typedef struct sensor_data_s sensor_data_t;

struct sensor_data_s {
    int has_motion; // 1 si un mouvement est détecté, 0 sinon
    int camera_active; // 1 si la caméra est active, 0 sinon
    int lidar_active; // 1 si le lidar est actif, 0 sinon
    int detected_time;
};

/**
 * \brief A cell within the map of the city.
 */
struct cell_s {
	int column;			  /*!< Column of the cell. */
	int row;			  /*!< Row of the cell. */
	cell_type_t type;	  /*!< Type of the cell (@see \enum e_cell_type). */
	int nb_of_characters; /*!< Number of characters on the cell. */
	int is_under_surveillance; /*!< Is the cell under surveillance? */
	sensor_data_t sensor_data; /*!< Sensor data. */
};

/**
 * \brief The map of the city.
 */
struct city_s {
	int width;		/*!< Width of the map. */
	int height;		/*!< Height of the map. */
	cell_t cells[MAX_ROWS][MAX_COLUMNS]; /*!< Cells of the map. */
};


typedef struct {
    int column;
    int row;
} coordinate_t;


/* Function to create a city */
city_t* create_city(int width, int height);
void delete_city(city_t* city);
void print_city(city_t* city);
cell_t* get_cell(city_t* city, int x, int y);
void define_monitoring(city_t* city, int x, int y, int nb_of_characters);
void clear_city(city_t* city);
void init_city(city_t* city);
int should_be_monitored(cell_type_t cell_type);
coordinate_t* findTypeOfBuilding(city_t* city, cell_type_t building_type, int count);
void print_city_with_characters(city_t * city);
void activate_camera(city_t* city, int x, int y);
void activate_lidar(city_t* city, int x, int y);
void detect_movement(city_t* city, int x, int y);
void initialize_cameras(city_t *city);
#endif /* CELL_H */
