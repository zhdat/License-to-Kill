#include "enemy_country.h"

sem_t *move_sem;

void set_semaphore(sem_t *sem) {
    move_sem = sem;
}

void writeToSharedMemory(memory_t* mem) {
    int num_message = 1;
    char buffer[MAX_LENGTH_OF_MESSAGE];
    ssize_t msg_size;
    mqd_t mq;
    char *position, *pos, *endptr;
    int priority;
    message_t new_message;

    mq = create_message_queue();

    struct mq_attr attr;
    if (mq_getattr(mq, &attr) == -1) {
        perror("mq_getattr");
        exit(EXIT_FAILURE);
    }

    while(mem->simulation_has_ended == 0) {
        msg_size = mq_receive(mq, buffer, sizeof(buffer), NULL);
        if (msg_size == -1) {
            perror("mq_receive");
            exit(EXIT_FAILURE);
        }

        strcpy(mem->encrpyted_messages[mem->mailbox_size].msg_text, buffer);
        mem->encrpyted_messages[mem->mailbox_size].is_encrypted = 0;
        mem->mailbox_size++;

    }

    mq_close(mq);
}








