#include "common.h"
#include "memory.h"
#include <pthread.h>

void* citizen_thread(void* arg) {
    // Logique de thread de citoyen
    // TODO: Implémenter la logique spécifique du citoyen

    return NULL;
}

int main() {
    pthread_t threads[NUM_CITIZENS];

    // Création de threads pour les citoyens
    for(int i = 0; i < NUM_CITIZENS; i++) {
        pthread_create(&threads[i], NULL, citizen_thread, NULL);
    }

    // Attente de la fin des threads
    // TODO: Gérer l'attente et la fermeture des threads

    return 0;
}
