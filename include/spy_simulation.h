#ifndef TP_SYS_SPY_SIMULATION_H
#define TP_SYS_SPY_SIMULATION_H

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "memory.h"



/**
 * \file spy_simulation.h
 *
 * \brief Header file for spy simulation, defining functions for initializing and managing the simulation.
 */

/**
 * \brief The main function of the spy simulation.
 *
 * \param mem The shared memory.
 */
void set_memory(memory_t* mem);

/**
 * \brief Creates and initializes a new shared memory area for the simulation.
 *
 * \return Pointer to the newly created shared memory structure.
 */
memory_t* create_shared_memory(void);

/**
 * \brief Sets up spies in the simulation.
 *
 * \param mem Pointer to the shared memory containing simulation data.
 */
void set_spies(memory_t* mem);

/**
 * \brief Sets up citizens in the simulation.
 *
 * \param mem Pointer to the shared memory containing simulation data.
 */
void set_citizens(memory_t* mem);

/**
 * \brief Sets up source agents in the simulation.
 *
 * \param mem Pointer to the shared memory containing simulation data.
 */
void set_attending_officers(memory_t* mem);

/**
 * \brief Sets up counter-intelligence officers in the simulation.
 *
 * \param mem Pointer to the shared memory containing simulation data.
 */
void set_counter_intelligence_officers(memory_t* mem);

/**
 * \brief Sets up the mailbox in the simulation.
 *
 * \param mem Pointer to the shared memory containing simulation data.
 */
void set_mailbox(memory_t* mem);

/**
 * \brief Retrieves the coordinates of the mailbox in the simulation.
 *
 * \param mem Pointer to the shared memory containing simulation data.
 * \return Coordinate of the mailbox.
 */
coordinate_t get_mailbox(memory_t* mem);

/**
 * \brief Finds a residence near the mailbox within a maximum distance.
 *
 * \param mem Pointer to the shared memory containing simulation data.
 * \param max_distance Maximum distance from the mailbox.
 * \return Coordinate of the found residence.
 */
coordinate_t get_residence_near_mailbox(memory_t* mem, int max_distance);

/**
 * \brief Finds a random residence with low population in the simulation.
 *
 * \param mem Pointer to the shared memory containing simulation data.
 * \return Coordinate of the found low populated residence.
 */
coordinate_t find_random_low_populated_residence(memory_t* mem);

/**
 * \brief Sets up the mailbox messages in the simulation.
 *
 * \param mem Pointer to the shared memory containing simulation data.
 */
void set_mailbox_messages(memory_t* mem);

/**
 * \brief Determines the distribution of information based on the number of employees.
 *
 * \param numberOfEmployees The number of employees in a company.
 * \return Information distribution structure based on employee count.
 */
InformationDistribution getInformationDistribution(int numberOfEmployees);

/**
 * \brief Assigns employees to companies in the simulation.
 *
 * \param mem Pointer to the shared memory containing simulation data.
 */
void set_company_employees(memory_t* mem);

/**
 * \brief Sets up all characters in the simulation.
 *
 * \param mem Pointer to the shared memory containing simulation data.
 */
void set_characters(memory_t* mem);

/**
 * \brief Sets up the city map for the simulation.
 *
 * \param mem Pointer to the shared memory containing simulation data.
 */
void set_city_map(memory_t* mem);

/**
 * \brief Initializes the shared memory with necessary content for the simulation.
 *
 * \param mem Pointer to the shared memory structure to be initialized.
 */
void set_content_memory(memory_t* mem);


#endif //TP_SYS_SPY_SIMULATION_H
