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
#include "character.h"

#define MAX_CITIZEN_COUNT 127
#define MAX_SOURCE_AGENT_COUNT 3
#define MAX_ATTENDING_OFFICER_COUNT 1
#define MAX_COUNTER_INTELLIGENCE_OFFICER_COUNT 1
#define MAX_MAILBOX_SIZE 100
#define SHARED_MEMORY_NAME "/nothinghere"


/**
 * \file mem.h
 *
 * Defines structures and functions used to manipulate our shared mem.
 */

typedef struct map_s map_t;
typedef struct memory_s memory_t;
typedef struct timer_s timer_t;

struct timer_s {
    int days;
    int hours;
    int minutes;
    int step;
    int turns;
};

/**
 * \brief The city map.
 */
struct map_s {
    int columns; /*!< The number of columns of the city map. */
    int rows; /*!< The number of rows of the city map.*/
    cell_t cells[MAX_COLUMNS][MAX_ROWS]; /*!< Cells that constitute the city map. */
};

/**
 * \brief Shared mem used by all processes.
 */
struct memory_s {
    int memory_has_changed; /*!< This flag is set to 1 when the mem has changed. */
    int simulation_has_ended; /*!< This flag is set to the following values:
                                * - 0: has not ended;
                                * - 1: the spy network has fled. It wins!
                                * - 2: the counterintelligence officer has discovered the mailbox. He wins.
                                * - 3: the counterintelligence officer did not discover the mailbox. The spy network
                                *      wins!
                                */
    int step; /*!< Numéro de l'étape de simulation en cours.*/
    int total_steps; /*!< Nombre total d'étapes de simulation à effectuer.*/

    int days; /*!< Nombre de jours écoulés depuis le début de la simulation.*/
    int hours; /*!< Heure actuelle dans la simulation.*/
    int minutes; /*!< Minutes actuelles dans la simulation.*/

    int citizens_at_home; /*!< Nombre de citoyens actuellement à leur domicile.*/
    int citizens_at_work; /*!< Nombre de citoyens actuellement au travail.*/
    int citizens_walking; /*!< Nombre de citoyens actuellement en déplacement.*/    

    character_t citizens[MAX_CITIZEN_COUNT]; /*!< Tableau contenant les citoyens.*/

    source_agent_t source_agents[MAX_SOURCE_AGENT_COUNT]; /*!< Tableau contenant les agents sources.*/

    attending_officer_t attending_officers[MAX_ATTENDING_OFFICER_COUNT]; /*!< Tableau contenant les officiers de surveillance.*/

    counter_intelligence_officer_t counter_intelligence_officers[MAX_COUNTER_INTELLIGENCE_OFFICER_COUNT]; /*!<Tableau contenant les officiers de contre-espionnage.*/

    int mailbox[MAX_MAILBOX_SIZE]; /*!< Boîte aux lettres contenant les messages.*/

    city_t city_map; /*!< Carte de la ville.*/

<<<<<<< HEAD
<<<<<<< HEAD
    coordinate_t mailbox_coordinate; /*!< Coordonnées de la boîte aux lettres.*/
    
};

memory_t * open_shared_memory(void);
void end_shared_memory(memory_t * mem);
=======
    timer_t my_timer; /*!< Timer de la simulation.*/
    
};

=======
    timer_t my_timer; /*!< Timer de la simulation.*/
    
};

>>>>>>> a22440242775064688c85465b3e67225034773c7
/* Open the shared mem */
memory_t *open_shared_memory(void);

/* Close the shared mem */
void close_shared_memory(memory_t *memory);
<<<<<<< HEAD
>>>>>>> a224402 (Timer)
=======
>>>>>>> a22440242775064688c85465b3e67225034773c7

#endif /* MEMORY_H */
