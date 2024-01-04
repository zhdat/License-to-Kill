#ifndef TP_SYS2_ENEMY_COUNTRY_H
#define TP_SYS2_ENEMY_COUNTRY_H

#define DECALAGE 3

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


#define MAX_MSG_SIZE 1024

typedef struct {
    char sentence[MAX_MSG_SIZE];
    int priority;
} Message;


void set_semaphore(sem_t *sem);

void writeToSharedMemory(memory_t *mem);

void decrpyt_message(char *message);

int isValidMessage(char *message);

#endif //TP_SYS2_ENEMY_COUNTRY_H
