#include "enemy_country.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#include "memory.h"

int main(void) {
    key_t key;
    int msgid;
    message_t msg;

    // Boucle infinie

    key = ftok("somefile", 65);

    msgid = msgget(key, 0666 | IPC_CREAT);


    while (1) {

        msgrcv(msgid, &msg, sizeof(msg), 1, 0);

        writeToSharedMemory(msg.msg_text, 1);

        // Afficher le message sur le téléscripteur

    }

    return EXIT_SUCCESS;
}
