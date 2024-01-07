#ifndef TP_SYS_CITIZEN_MANGER_H
#define TP_SYS_CITIZEN_MANGER_H

#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "memory.h"
#include "tools.h"
#include "common.h"
#include "monitor.h"
#include "logger.h"
#include "cell.h"

/**
 * \file citizen_manager.h
 *
 * \brief Manages the activities of the citizens in the simulation.
 */

/**
 * \brief Arguments for the citizen threads.
 */
typedef struct {
    int index;
    memory_t* mem;
} citizen_monitor_args_t;

/**
 * \brief Sets a semaphore for controlling citizen movements.
 *
 * \param sem Pointer to the semaphore to be used for movement control.
 */
void set_semaphore(sem_t* sem);

/**
 * \brief Handler for timer signals, used to manage citizen movements.
 *
 * This function updates a global array `timer_citizens` to signal that citizens should move.
 *
 * \param signum The signal number (not used in the function).
 */
void timer_handler(int signum);

/**
 * \brief Initializes the citizen timer array.
 *
 * Sets all elements of the global array `timer_citizens` to 0.
 */
void set_timer_citizens(void);

/**
 * \brief Sets up the signal handler for managing citizens.
 *
 * Configures the system to handle `SIGALRM` with `timer_handler`.
 */
void set_signal_handler(void);

/**
 * \brief Moves a citizen to a new location.
 *
 * This function changes the citizen's position and updates the population in the cells.
 *
 * \param arg Pointer to `citizen_monitor_args_t` containing citizen and memory information.
 * \param row The destination row for the citizen.
 * \param column The destination column for the citizen.
 */
void move_citizen(citizen_monitor_args_t* arg, int row, int column);

/**
 * \brief Thread function for moving citizens to work.
 *
 * Moves the citizen to their workplace when signaled by the timer.
 *
 * \param args Pointer to `citizen_monitor_args_t` containing citizen and memory information.
 * @return NULL.
 */
void* citizen_to_work(void* args);

/**
 * \brief Thread function for moving citizens to home.
 *
 * Moves the citizen to their home when signaled by the timer.
 *
 * \param args Pointer to `citizen_monitor_args_t` containing citizen and memory information.
 * @return NULL.
 */
void* citizen_to_home(void* args);

/**
 * \brief Thread function for moving citizens to home via supermarket.
 *
 * Moves the citizen to the supermarket and then to home when signaled by the timer.
 *
 * \param args Pointer to `citizen_monitor_args_t` containing citizen and memory information.
 * @return NULL.
 */
void* citizen_to_home_supermarket(void* args);

/**
 * \brief Checks if a citizen works in a supermarket.
 *
 * \param mem The shared memory containing the city map and citizens.
 * \param citizen The citizen to be checked.
 * @return 1 if the citizen works in a supermarket, 0 otherwise.
 */
int work_in_supermarket(memory_t mem, character_t citizen);

/**
 * \brief Creates threads for citizens to move to work in the morning.
 *
 * \param mem Pointer to the shared memory containing simulation data.
 * \param ids Array of thread IDs for the citizens.
 * \param args Array of arguments for the citizen threads.
 */
void
create_morning_thread(memory_t* mem, pthread_t ids[MAX_CITIZEN_COUNT], citizen_monitor_args_t* args[MAX_CITIZEN_COUNT]);

/**
 * \brief Creates threads for citizens to move home from companies in the evening.
 *
 * \param mem Pointer to the shared memory containing simulation data.
 * \param ids Array of thread IDs for the citizens.
 * \param args Array of arguments for the citizen threads.
 */
void create_evening_company_thread(memory_t* mem, pthread_t ids[MAX_CITIZEN_COUNT],
                                   citizen_monitor_args_t* args[MAX_CITIZEN_COUNT]);

/**
 * \brief Creates threads for citizens to move home from supermarkets in the evening.
 *
 * \param mem Pointer to the shared memory containing simulation data.
 * \param ids Array of thread IDs for the citizens.
 * \param args Array of arguments for the citizen threads.
 */
void create_evening_supermarket_thread(memory_t* mem, pthread_t ids[MAX_CITIZEN_COUNT],
                                       citizen_monitor_args_t* args[MAX_CITIZEN_COUNT]);

/**
 * \brief Creates and manages threads for all citizens' movements.
 *
 * Handles the creation of threads for citizens' movements based on the time of day.
 *
 * \param mem Pointer to the shared memory containing simulation data.
 */
void create_citizens_thread(memory_t* mem);

#endif //TP_SYS_CITIZEN_MANGER_H
