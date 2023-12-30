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
        mem->mailbox_size++;

    }

    mq_close(mq);
}

void decrpyt_message(char* message) {
    // Logique de déchiffrement
    int i = 0;
    while (message[i] != '\0') {
        if (message[i] >= 'a' && message[i] <= 'z') {
            message[i] = message[i] - DECALAGE;
            if (message[i] < 'a') {
                message[i] = message[i] + 'z' - 'a' + 1;
            }
        } else if (message[i] >= 'A' && message[i] <= 'Z') {
            message[i] = message[i] - DECALAGE;
            if (message[i] < 'A') {
                message[i] = message[i] + 'Z' - 'A' + 1;
            }
        }
        i++;
    }
}

int isValidMessage(char* message) {
    message = message;
    // Implémentez votre logique de filtrage ici

    // Retournez 1 si le message est valide, 0 sinon

    return 1; // Exemple : ici, tous les messages sont considérés comme valides
}






