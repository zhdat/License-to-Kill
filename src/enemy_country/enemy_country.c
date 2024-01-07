#include "enemy_country.h"
#include "debug.h"

/**
 * \file main.c
 * \brief Defines function for initializing and managing the enemy country.
 */

void writeToSharedMemory(memory_t* mem) {
    char buffer[MAX_LENGTH_OF_MESSAGE];
    ssize_t msg_size;
    mqd_t mq;
    char* pos, * endptr;
    int priority;

    mq = create_message_queue();

    struct mq_attr attr;
    if (mq_getattr(mq, &attr) == -1) {
        perror("mq_getattr");
        exit(EXIT_FAILURE);
    }

    while (mem->simulation_has_ended == 0) {
        msg_size = mq_receive(mq, buffer, sizeof(buffer), NULL);
        if (msg_size == -1) {
            perror("mq_receive");
            exit(EXIT_FAILURE);
        }

        pos = strchr(buffer, '-');
        if (pos == NULL) {
            log_info("- not find !\n");
            continue;
        }
        *pos = '\0';
        pos++;
        priority = strtol(pos, &endptr, 10);

        if (mem->mailbox_size == MAX_MAILBOX_SIZE) {
            log_info("Mailbox is full. Message is not added to the mailbox.\n");
            continue;
        }
        strcpy(mem->encrpyted_messages[mem->mailbox_size].msg_text, buffer);
        mem->encrpyted_messages[mem->mailbox_size].is_encrypted = 0;
        mem->encrpyted_messages[mem->mailbox_size].priority = priority;
        mem->mailbox_size++;
    }
    mq_close(mq);
    destroy_message_queue(mq);
}
