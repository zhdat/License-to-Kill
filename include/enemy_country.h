#ifndef TP_SYS2_ENEMY_COUNTRY_H
#define TP_SYS2_ENEMY_COUNTRY_H

#define DECALAGE 3

#include "common.h"

struct message_s {
    long type;
    char msg_text[MAX_LENGTH_OF_MESSAGE];
    int is_false_message; /*!< 0 if the message is true, 1 if the message is false */
};

typedef struct message_s message_t;

void writeToSharedMemory(char* message, int valid_message);

void decrpyt_message(char* message);

int isValidMessage(char* message);

#endif //TP_SYS2_ENEMY_COUNTRY_H
