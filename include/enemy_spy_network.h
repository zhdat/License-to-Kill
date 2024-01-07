#ifndef PROJECT_ENEMY_SPY_NETWORK_H
#define PROJECT_ENEMY_SPY_NETWORK_H

#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

#ifdef __APPLE__
#include "macosx.h"
#elif __linux__

#include <mqueue.h>

#endif

#include "memory.h"
#include "tools.h"
#include "common.h"
#include "monitor.h"
#include "logger.h"

/**
 * \file enemy_spy_network.h
 * \brief Manages the activities of the enemy spy network in the simulation.
 */

/**
 * \brief The arguments for the threads.
 */
typedef struct {
    int id;
    memory_t* mem;
} agent_thread_args_t;

/**
 * \brief All the threads in the enemy spy network.
 */
typedef struct {
    pthread_t source_agent_threads[MAX_SOURCE_AGENT_COUNT];
    agent_thread_args_t source_agent_args[MAX_SOURCE_AGENT_COUNT];
    pthread_t attending_officer_threads[MAX_ATTENDING_OFFICER_COUNT];
    agent_thread_args_t attending_officer_args[MAX_ATTENDING_OFFICER_COUNT];
} all_threads_t;

/**
 * \brief Moves a source agent to a specified location.
 *
 * \param arg Pointer to agent_thread_args_t containing agent and memory information.
 * \param row The destination row for the agent.
 * \param column The destination column for the agent.
 */
void move_source_agent(agent_thread_args_t* arg, int row, int column);

/**
 * \brief Moves an attending officer to a specified location.
 *
 * \param arg Pointer to agent_thread_args_t containing officer and memory information.
 * \param row The destination row for the officer.
 * \param column The destination column for the officer.
 */
void move_attending_officer(agent_thread_args_t* arg, int row, int column);

/**
 * \brief Thread function for the morning routine of a source agent.
 *
 * \param arg Pointer to agent_thread_args_t containing agent and memory information.
 * @return NULL.
 */
void* morning_source_agent(void* arg);

/**
 * \brief Thread function for the morning routine of an attending officer.
 *
 * \param arg Pointer to agent_thread_args_t containing officer and memory information.
 * @return NULL.
 */
void* morning_attending_officer(void* arg);

/**
 * \brief Thread function for the evening routine of a source agent.
 *
 * \param arg Pointer to agent_thread_args_t containing agent and memory information.
 * @return NULL.
 */
void* evening_source_agent(void* arg);

/**
 * \brief Thread function for the evening routine of an attending officer.
 *
 * \param arg Pointer to agent_thread_args_t containing officer and memory information.
 * @return NULL.
 */
void* evening_attending_officer(void* arg);

/**
 * \brief Thread function for attempting information theft by a source agent.
 *
 * \param arg Pointer to agent_thread_args_t containing agent and memory information.
 * @return NULL.
 */
void* attempt_information_theft(void* arg);

/**
 * \brief Creates and manages threads for the morning activities of the spy network.
 *
 * \param mem Pointer to the shared memory containing simulation data.
 * \param threads Pointer to all_threads_t structure containing thread information.
 */
void create_network_morning_thread(memory_t* mem, all_threads_t* threads);

/**
 * \brief Creates and manages threads for the evening activities of the spy network.
 *
 * \param mem Pointer to the shared memory containing simulation data.
 * \param threads Pointer to all_threads_t structure containing thread information.
 */
void create_network_evening_thread(memory_t* mem, all_threads_t* threads);

/**
 * \brief Creates and manages threads for the night activities of the spy network.
 *
 * \param mem Pointer to the shared memory containing simulation data.
 * \param threads Pointer to all_threads_t structure containing thread information.
 */
void create_network_night_thread(memory_t* mem, all_threads_t* threads);

/**
 * \brief Handles signal reception for the enemy spy network.
 *
 * \param sig The received signal number.
 */
void handle_signal(int);

/**
 * \brief Sets up signal handling for the enemy spy network.
 */
void set_signals(void);

/**
 * \brief Sets a semaphore for controlling the movement of agents and officers.
 *
 * \param sem Pointer to the semaphore to be used for movement control.
 */
void set_semaphore(sem_t* sem);

/**
 * \brief Creates and manages threads for the enemy spy network's activities.
 *
 * \param mem Pointer to the shared memory containing simulation data.
 */
void create_enemy_spy_thread(memory_t* mem);

/**
 * \brief Maps a source agent to its ID for signal handling.
 *
 * \param agent Pointer to the source_agent_t structure representing the agent.
 * \param id The ID of the agent to be mapped.
 */
void agent_mapping(source_agent_t* agent, int id);


/**
 * \brief Signal handler for a weak bullet attack (SIGUSR1) on an agent.
 *
 * \param sig The signal number received.
 * \param info Signal information structure.
 * \param unused Unused parameter.
 */
void handle_sigusr1(int sig, siginfo_t* info, void* unused);

/**
 * \brief Sets up signal handling for a weak bullet attack (SIGUSR1).
 */
void set_signals_weak_bullet(void);


/**
 * \brief Signal handler for a strong bullet attack (SIGUSR2) on an agent.
 *
 * \param sig The signal number received.
 * \param info Signal information structure.
 * \param unused Unused parameter.
 */
void handle_sigusr2(int sig, siginfo_t* info, void* unused);

/**
 * \brief Sets up signal handling for a strong bullet attack (SIGUSR2).
 */
void set_signals_strong_bullet(void);

/**
 * \brief Accomplishes a mission by an agent and determines the cruciality of information obtained.
 *
 * \param mem Pointer to the shared memory structure.
 * \param company The target company's coordinate.
 * @return The cruciality level of the information obtained.
 */
InformationCruciality accomplish_mission(memory_t* mem, coordinate_t company);

/**
 * \brief Posts a message with a specified priority and type.
 *
 * \param priority The priority level of the message.
 * \param type The type of the message (real or fake).
 */
void post_message(InformationCruciality priority, int type);

/**
 * \brief Picks up messages from the mailbox and decrypts them.
 *
 * \param mem Pointer to the shared memory structure.
 */
void pickup_messages(memory_t* mem);


#endif //PROJECT_ENEMY_SPY_NETWORK_H
