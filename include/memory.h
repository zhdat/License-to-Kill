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
#ifndef MEMORY_H
#define MEMORY_H

#include "cell.h"
#include "common.h"

#define MAX_SPY_COUNT 3

/**
 * \file memory.h
 *
 * Defines structures and functions used to manipulate our shared memory.
 */

typedef struct map_s map_t;
typedef struct memory_s memory_t;

/**
 * \brief The city map.
 */
struct map_s {
    int columns; /*!< The number of columns of the city map. */
    int rows; /*!< The number of rows of the city map.*/
    cell_t cells[MAX_COLUMNS][MAX_ROWS]; /*!< Cells that constitute the city map. */
};

/**
 * \brief Shared memory used by all processes.
 */
struct memory_s {
    int memory_has_changed; /*!< This flag is set to 1 when the memory has changed. */
    int simulation_has_ended; /*!< This flag is set to the following values:
                                * - 0: has not ended;
                                * - 1: the spy network has fled. It wins!
                                * - 2: the counterintelligence officer has discovered the mailbox. He wins.
                                * - 3: the counterintelligence officer did not discover the mailbox. The spy network
                                *      wins!
                                */
    int step; /*!< Numéro de l'étape de simulation en cours.*/
    double elapsed_time; /*!< Temps écoulé depuis le début de la simulation.*/
    int hour; /*!< Heure actuelle dans la simulation.*/
    int minutes; /*!< Minutes actuelles dans la simulation.*/

    int citizens_at_home; /*!< Nombre de citoyens actuellement à leur domicile.*/
    int citizens_at_work; /*!< Nombre de citoyens actuellement au travail.*/
    int citizens_walking; /*!< Nombre de citoyens actuellement en déplacement.*/

    // spyt_t spies[MAX_SPY_COUNT];  /*!< Tableau contenant les espions.*/
    int spy_count; /*!< Nombre d'espions actuellement dans la simulation.*/

    int officer_health_points; /*!< Points de vie de l'officier.*/
    int officer_location_row; /*!< Ligne actuelle où se trouve l'officier.*/
    int officer_location_column; /*!< Colonne actuelle où se trouve l'officier.*/
    int officer_mailbox_row; /*!< Ligne de la boîte aux lettres de l'officier.*/
    int officer_mailbox_column; /*!< Colonne de la boîte aux lettres de l'officier.*/
    int officer_targeted_character_id; /*!< ID du personnage ciblé par l'officier.*/

    int mailbox_message_count; /*!< Nombre de messages dans la boîte aux lettres.*/

    map_t city_map; /*!< Carte de la ville.*/
};

#endif /* MEMORY_H */
