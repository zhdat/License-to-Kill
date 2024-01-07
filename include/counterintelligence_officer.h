#ifndef PROJECT_COUNTERINTELLIGENCE_OFFICER_H
#define PROJECT_COUNTERINTELLIGENCE_OFFICER_H

#include <semaphore.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "memory.h"
#include "tools.h"
#include "common.h"
#include "monitor.h"
#include "logger.h"
#include "character.h"

//

/**
 * \file counterintelligence_officer.h
 * \brief Manages the activities of counter-intelligence officers in the simulation.
 */

/**
 * \brief The maximum number of counter-intelligence officers in the simulation.
 */
typedef struct {
    int id;
    memory_t* mem;
} officer_thread_args_t;

/**
 * \brief Structure for managing all threads for counter-intelligence officers.
 */
typedef struct {
    pthread_t counter_intelligence_officer_threads[MAX_COUNTER_INTELLIGENCE_OFFICER_COUNT];
    officer_thread_args_t counter_intelligence_officer_args[MAX_COUNTER_INTELLIGENCE_OFFICER_COUNT];
} all_threads_t;

/**
 * \brief Sets a semaphore for controlling the movement of counter-intelligence officers.
 *
 * \param sem Pointer to the semaphore to be used for movement control.
 */
void set_semaphore(sem_t* sem);

/**
 * \brief Handles the signal for counter-intelligence officer activities.
 *
 * This function updates a global flag to signal that counter-intelligence officers should perform their activities.
 *
 * \param sig The signal number.
 */
void handle_signal(int sig);

/**
 * \brief Sets the signal handler for managing counter-intelligence officers.
 *
 * Configures the system to handle specific signals with `handle_signal`.
 */
void set_signals(void);

/**
 * \brief Moves a counter-intelligence officer to a new location.
 *
 * This function changes the officer's position and updates the population in the cells.
 *
 * \param arg Pointer to `officer_thread_args_t` containing officer and memory information.
 * \param row The destination row for the officer.
 * \param column The destination column for the officer.
 * \param index Index of the source agent being targeted by the officer.
 */
void move_counter_intelligence_officer(officer_thread_args_t* arg, int row, int column, int index);

/**
 * \brief Thread function for the all-day activity of a counter-intelligence officer.
 *
 * Moves the officer based on the signals received and the location of the targeted source agent.
 *
 * \param args Pointer to `officer_thread_args_t` containing officer and memory information.
 * @return NULL.
 */
void* all_day_counter_intelligence_officer(void* args);

/**
 * \brief Creates a thread for a counter-intelligence officer.
 *
 * \param args Pointer to `all_threads_t` containing thread and argument information for officers.
 */
void create_counter_intelligence_officer_thread(all_threads_t* args);

/**
 * \brief Creates and manages threads for counter-intelligence officers' activities.
 *
 * Handles the creation of threads for counter-intelligence officers based on the simulation status.
 *
 * \param mem Pointer to the shared memory containing simulation data.
 */
void create_counter_intelligence_officer_threads(memory_t* mem);

#endif //PROJECT_COUNTERINTELLIGENCE_OFFICER_H
