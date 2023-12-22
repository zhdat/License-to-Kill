#include "common.h"
#include "memory.h"
#include "character_factory.h"
#include <pthread.h>

void* citizen_thread(void* arg) {
    // Logique de thread de citoyen
    // TODO: Implémenter la logique spécifique du citoyen



    return NULL;
}


int main() {
    pthread_t threads[NUM_CITIZENS];


    // TODO : Randomisation de l'habitation des citoyens



    // Création de la factory de citoyens
    character_factory_t *factory;
    factory = new_factory_character(new_citizen(0,0));  
    a_citizen = factory->factory_method(); // création d'un citoyen


    // Création de threads pour les citoyens
    for(int i = 0; i < NUM_CITIZENS; i++) {
        pthread_create(&threads[i], NULL, citizen_thread, NULL);
        factory = new_factory_character(new_citizen(0,0,thread[i]));  
        a_citizen = factory->factory_method();
    }



    // Attente de la fin des threads
    // TODO: Gérer l'attente et la fermeture des threads

    return 0;
}


