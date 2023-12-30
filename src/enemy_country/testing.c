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


int main(void) {
    mqd_t mq;
    struct mq_attr attr;
    Message msg;

    mq_unlink(QUEUE_NAME); /* Evaluate if this is necessary */

    /* Open the message queue */
    memset(&attr, '\0', sizeof(attr));
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10; /* Maximum number of messages in the queue */
    attr.mq_msgsize = sizeof(Message); /* Maximum message size */
    attr.mq_curmsgs = 0;

    mq = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, 0600, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    while (1) {
        strcpy(msg.sentence, "Hello world");
        //ceasarCipher(msg.sentence, 3);

        if (strcmp(msg.sentence, "exit\n") == 0) {
            break;
        }

        msg.priority = 0;

        /* Send the message to the queue */
        if (mq_send(mq, (const char *)&msg, sizeof(Message), msg.priority) == -1) {
            perror("mq_send");
            continue;
        }
    }

    /* Close and unlink the message queue */
    mq_close(mq);
    mq_unlink(QUEUE_NAME);

    return EXIT_SUCCESS;
}