#ifndef TIMER_H
#define TIMER_H

#include <signal.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "debug.h"
#include "memory.h"

/**
 * \file timer.h
 *
 * \brief ensures the simulation runs for a given amount of time.
 * Periodically sends a SIGALRM signal to the main process of a new turn in the simulation.
 */

/**
 * \brief Sets the shared memory for the timer.
 *
 * \param memory_start Pointer to the start of shared memory.
 */
void set_memory(memory_t* memory_start);

/**
 * \brief Sets the process IDs (PIDs) for timer management.
 *
 * \param argc The argument count.
 * \param argv The argument vector containing PIDs.
 */
void set_pids(int argc, char** argv);

/**
 * \brief Checks if the simulation is finished.
 *
 * \param memory Pointer to the shared memory.
 * \return Returns 1 if the simulation is finished, otherwise 0.
 */
int has_simulation_ended(memory_t* memory);


/**
 * \brief The handler function for timer signals.
 *
 * \param signum The signal number.
 */
void timer_handler(int signum);

/**
 * \brief Sets up a timer with a specified interval.
 *
 * \param interval The interval in seconds for the timer.
 */
void my_set_timer(float interval);

/**
 * \brief Frees resources associated with the timer.
 */
void free_timer(void);

#endif //TIMER_H
