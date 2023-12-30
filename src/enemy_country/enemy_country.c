#include "enemy_country.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#include "memory.h"




// e programme “enemy_country” reçoit les messages chiffrés en provenance du réseau
//d’espionnage, puis les affichent en continu sur un téléscripteur ﴾exception faite des mes‐
//sages “trompeurs” qu’il aura aussi reçu﴿. La communication entre le réseau d’espions ﴾pro‐
//gramme “enemy_spy_network”﴿ et “enemy_country” est réalisée à l’aide d’un canal de
//transmission unique implémenté avec une file de messages.
#include <stdio.h>

// Définir une structure pour les messages

#include "memory.h"  // Assurez-vous d'inclure le fichier d'en-tête pour la mémoire partagée

sem_t *move_sem;

void set_semaphore(sem_t *sem) {
    move_sem = sem;
}

void writeToSharedMemory(char* message, int valid_message) {
    memory_t* mem;
    char decrpyted_message[MAX_LENGTH_OF_MESSAGE]; // Use a fixed-size buffer instead of malloc
    strcpy(decrpyted_message, message);

    log_info("message bien reçu");
    // Ouvrir la mémoire partagée
    mem = open_shared_memory();

    // Écrire le message dans la mémoire partagée
    sem_wait(move_sem);
    strncpy(mem->encrpyted_messages[mem->mailbox_size].msg_text, message, MAX_LENGTH_OF_MESSAGE - 1);
    mem->encrpyted_messages[mem->mailbox_size].msg_text[MAX_LENGTH_OF_MESSAGE - 1] = '\0';
    mem->mailbox_size++;
    mem->memory_has_changed = 1;
    sem_post(move_sem);




    if (valid_message) {
        decrpyt_message(decrpyted_message);
        sem_wait(move_sem);
        strncpy(mem->decrypted_messages[mem->decrypted_mailbox_size].msg_text, decrpyted_message,
                MAX_LENGTH_OF_MESSAGE - 1);
        mem->decrypted_messages[mem->decrypted_mailbox_size].msg_text[MAX_LENGTH_OF_MESSAGE - 1] = '\0';
        mem->decrypted_mailbox_size++;
        mem->memory_has_changed = 1;
        sem_post(move_sem);
    }


    // Vous pouvez ajouter ici des mécanismes de synchronisation si nécessaire

    // Fermer la mémoire partagée si nécessaire
    // (par exemple, dé-mapper la mémoire)
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
    // Implémentez votre logique de filtrage ici

    // Retournez 1 si le message est valide, 0 sinon

    return 1; // Exemple : ici, tous les messages sont considérés comme valides
}






