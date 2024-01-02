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

#include <pthread.h>
#include <semaphore.h>
#ifdef __APPLE__
#include "macosx.h"
#elif __linux__
#include <mqueue.h>
#endif
#include "cell.h"
#include "common.h"
#include "character.h"

#define MAX_CITIZEN_COUNT 127
#define MAX_SOURCE_AGENT_COUNT 3
#define MAX_ATTENDING_OFFICER_COUNT 1
#define MAX_COUNTER_INTELLIGENCE_OFFICER_COUNT 1
#define MAX_MAILBOX_SIZE 100
#define SHARED_MEMORY_NAME "/nothinghere"
#define SEMAPHORE_NAME "/my_semaphore"
#define QUEUE_NAME "/myqueue"


/**
 * \file mem.h
 *
 * Defines structures and functions used to manipulate our shared mem.
 */

typedef struct map_s map_t;
typedef struct memory_s memory_t;
typedef struct timer_s timer_type;

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
    cell_t cells[MAX_ROWS][MAX_COLUMNS]; /*!< Cells that constitute the city map. */
};

typedef struct {
    int row;
    int column;
    int nb_of_employees;
    InformationDistribution cruciality;
}company_t;


typedef struct{
    long type;
    char msg_text[MAX_LENGTH_OF_MESSAGE];
    int is_false_message; /*!< 0 if the message is true, 1 if the message is false */
    int is_encrypted; /*!< 0 if the message is not encrypted, 1 if the message is encrypted */
} message_t;

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

    message_t encrpyted_messages[MAX_MAILBOX_SIZE]; /*!< Boîte aux lettres contenant les messages.*/
    int mailbox_size; /*!< Taille de la boîte aux lettres.*/

    message_t decrypted_messages[MAX_MAILBOX_SIZE]; /*!< Boîte aux lettres contenant les messages déchiffrés.*/
    int decrypted_mailbox_size; /*!< Taille de la boîte aux lettres contenant les messages déchiffrés.*/

    city_t city_map; /*!< Carte de la ville.*/

    company_t companies_priority[NUMBER_OF_COMPANIES]; /*!< Tableau contenant le nombre d'employés des entreprises.*/

    coordinate_t mailbox_coordinate; /*!< Coordonnées de la boîte aux lettres.*/

    timer_type my_timer; /*!< Timer de la simulation.*/

    pthread_mutex_t mutex; /*!< Mutex pour la synchronisation des threads.*/

};

memory_t* open_shared_memory(void);

void end_shared_memory(memory_t* mem);

void destroy_shared_memory(void);

sem_t* create_semaphore(void);

sem_t* open_semaphore(void);

void close_semaphore(sem_t* sem);

void destroy_semaphore(sem_t* sem);

sem_t * create_semaphore_message();

sem_t * open_semaphore_message();

mqd_t create_message_queue();

mqd_t open_message_queue();


#endif /* MEMORY_H */
