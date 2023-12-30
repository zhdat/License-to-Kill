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

#ifdef __APPLE__
#include "macosx/mqueue.h"
#elif __linux__
#include <mqueue.h>
#endif

#define QUEUE_NAME "/myqueue"
#define MAX_MSG_SIZE 1024

typedef struct {
    char sentence[MAX_MSG_SIZE];
    int priority;
} Message;

struct message_s {
    long type;
    char msg_text[MAX_LENGTH_OF_MESSAGE];
    int is_false_message; /*!< 0 if the message is true, 1 if the message is false */
};

void set_semaphore(sem_t *sem);

typedef struct message_s message_t;

void writeToSharedMemory(char* message, int valid_message);

void decrpyt_message(char* message);

int isValidMessage(char* message);

#endif //TP_SYS2_ENEMY_COUNTRY_H
