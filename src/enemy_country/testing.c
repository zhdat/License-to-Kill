#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "enemy_country.h"

// Structure pour les messages IPC

// Fonction pour appliquer le chiffrement César
void caesarCipher(char *message, int shift) {
    for (int i = 0; message[i] != '\0'; ++i) {
        char ch = message[i];

        if (ch >= 'a' && ch <= 'z') {
            ch = ch + shift;
            if (ch > 'z') {
                ch = ch - 'z' + 'a' - 1;
            }
            message[i] = ch;
        } else if (ch >= 'A' && ch <= 'Z') {
            ch = ch + shift;
            if (ch > 'Z') {
                ch = ch - 'Z' + 'A' - 1;
            }
            message[i] = ch;
        }
    }
}

int main() {
    message_t msg;
    int msgid;
    key_t key;
    int shift = 3;

    // Créer une clé unique pour la file de messages
    key = ftok("somefile", 65);

    // Connecter à la file de messages
    msgid = msgget(key, 0666 | IPC_CREAT);

    // Messages exemples
    char *messages[] = {"Hello World", "Enemy Spotted", "Mission Accomplished", NULL};

    for (int i = 0; messages[i] != NULL; i++) {
        strcpy(msg.msg_text, messages[i]);
        caesarCipher(msg.msg_text, shift);  // Chiffrer le message


        msg.type = 1;

        msgsnd(msgid, &msg, sizeof(msg), 0);
    }

    return 0;
}
