#ifndef TP_SYS2_ENEMY_COUNTRY_H
#define TP_SYS2_ENEMY_COUNTRY_H

#include "common.h"
#include <semaphore.h>
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "memory.h"

#ifdef __APPLE__
#include "macosx/mqueue.h"
#elif __linux__

#include <mqueue.h>

#endif


/**
 * \file enemy_country.h
 * \brief Handles operations related to the enemy country, particularly in managing shared memory for message communication.
 */


/**
 * \brief Writes messages received from a message queue to shared memory.
 *
 * This function continuously reads messages from a message queue and writes them into the shared memory.
 * It extracts the priority from each message and stores it along with the message text.
 *
 * \param mem Pointer to the shared memory structure where messages are stored.
 */
void writeToSharedMemory(memory_t* mem);


#endif //TP_SYS2_ENEMY_COUNTRY_H
