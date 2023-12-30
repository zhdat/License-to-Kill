#include "enemy_country.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#include "memory.h"

/*
int main(int argc, char* argv[]) {
    key_t key;
    int msgid;
    message_t msg;
    sem_t *sem = open_semaphore();
    set_semaphore(sem);
    memory_t *mem = open_shared_memory();

    // Boucle infinie

    key = ftok("somefile", 65);

    msgid = msgget(key, 0666 | IPC_CREAT);


    while (1) {

        msgrcv(msgid, &msg, sizeof(msg), 1, 0);
        printf("lblbllb");
        printf("Data Received is : %s \n", msg.msg_text);

        writeToSharedMemory(msg.msg_text, 1);

        // Afficher le message sur le téléscripteur

    }
    //close_semaphore(sem);

    return EXIT_SUCCESS;
}*/

void main() {
    mqd_t mq;
    struct mq_attr attr;
    Message msg;

    sem_t *sem = open_semaphore();
    set_semaphore(sem);

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(Message);
    attr.mq_curmsgs = 0;

    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0600, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    while (1) {
        if (mq_receive(mq, (char *)&msg, sizeof(Message), NULL) == -1) {
            perror("mq_receive");
            continue;
        }

        //printf("message received: %s\n", msg.sentence);
        //log_info("message received: %s\n", msg.sentence);

        writeToSharedMemory(msg.sentence, 1);
    }
}
