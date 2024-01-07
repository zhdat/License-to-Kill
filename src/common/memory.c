#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

#include "memory.h"
#include "debug.h"

/**
 * \file memory.c
 *
 * Defines functions used to manipulate our shared mem.
 */


memory_t* open_shared_memory(void) {
    int fd;
    memory_t* mem;

    fd = shm_open(SHARED_MEMORY_NAME, O_RDWR, 0660);
    if (fd == -1) {
        perror("shm_open\n");
        exit(EXIT_FAILURE);
    }

    mem = mmap(NULL, sizeof(memory_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mem == MAP_FAILED) {
        perror("mmap\n");
        exit(EXIT_FAILURE);
    }
    return mem;
}

void end_shared_memory(memory_t* mem) {
    if (munmap(mem, sizeof(memory_t)) == -1) {
        perror("munmap\n");
        exit(EXIT_FAILURE);
    }
}

void destroy_shared_memory(void) {
    if (shm_unlink(SHARED_MEMORY_NAME) == -1) {
        perror("shm_unlink\n");
        exit(EXIT_FAILURE);
    }
}

sem_t* create_semaphore(void) {
    sem_t* sem;
    sem = sem_open(SEMAPHORE_NAME, O_CREAT | O_RDWR, 0644, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open\n");
        exit(EXIT_FAILURE);
    }

    return sem;
}

sem_t* open_semaphore(void) {
    sem_t* sem;
    sem = sem_open(SEMAPHORE_NAME, O_RDWR, 0644, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open\n");
        exit(EXIT_FAILURE);
    }

    return sem;
}

void close_semaphore(sem_t* sem) {
    if (sem_close(sem) == -1) {
        perror("sem_close\n");
        exit(EXIT_FAILURE);
    }
}

void destroy_semaphore(sem_t* sem) {
    sem = sem;
    if (sem_unlink(SEMAPHORE_NAME) == -1) {
        perror("sem_unlink\n");
        exit(EXIT_FAILURE);
    }
}

sem_t* create_semaphore_message(void) {
    sem_t* semaphore;
    semaphore = sem_open("/sem_spy_simulation_message", O_CREAT | O_RDWR, 0644, 1);

    if (semaphore == SEM_FAILED) {
#if DEBUG
        printf("Error cannot create semaphore");
#endif
        perror("sem_message_create\n");
        exit(EXIT_FAILURE);
    }

    return semaphore;
}

sem_t* open_semaphore_message(void) {
    sem_t* semaphore;
    semaphore = sem_open("/sem_spy_simulation_message", O_RDWR, 0644, 1);

    if (semaphore == SEM_FAILED) {
#if DEBUG
        printf("Error cannot open semaphore");
#endif
        perror("sem_message_open\n");
        exit(EXIT_FAILURE);
    }

    return semaphore;
}

mqd_t create_message_queue(void) {
    mq_unlink(QUEUE_NAME);

    // Attributs de la file de messages
    struct mq_attr attr;
    attr.mq_flags = 0;        // Flags (utiliser 0 pour aucune option spéciale)
    attr.mq_maxmsg = 10;      // Nombre maximal de messages dans la file
    attr.mq_msgsize = MAX_LENGTH_OF_MESSAGE;   // Taille maximale d'un message
    attr.mq_curmsgs = 0;      // Nombre actuel de messages dans la file

    // Ouvrir ou créer une file de messages avec les attributs spécifiés
    mqd_t mq = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, 0666, &attr);
    if (mq == (mqd_t) -1) {
        perror("mq_create\n");
        exit(EXIT_FAILURE);
    }

    return mq;
}


mqd_t open_message_queue(void) {
    mqd_t mq = mq_open(QUEUE_NAME, O_RDWR);
    if (mq == (mqd_t) -1) {
        perror("mq_open\n");
    }
    return mq;
}

void destroy_message_queue(mqd_t mq) {
    if (mq_close(mq) == -1) {
        perror("mq_close\n");
        exit(EXIT_FAILURE);
    }

    if (mq_unlink(QUEUE_NAME) == -1) {
        perror("mq_unlink\n");
        exit(EXIT_FAILURE);
    }
}
