#include "common.h"
#include "memory.h"
#include <pthread.h>

void* spy_thread(void* arg) {
    // Logique de thread d'espion
    // TODO: Implémenter la logique spécifique de l'espion

    return NULL;
}

int main() {
    pthread_t threads[NUM_SPIES];

    // Création de threads pour les espions
    for(int i = 0; i < NUM_SPIES; i++) {
        pthread_create(&threads[i], NULL, spy_thread, NULL);
    }

    // Attente de la fin des threads
    // TODO: Gérer l'attente et la fermeture des threads

    return 0;
}
