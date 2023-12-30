#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

#include "memory.h"

memory_t* open_shared_memory(void) {
    int fd;
    memory_t* mem;

    fd = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0660);
    if (fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    mem = mmap(NULL, sizeof(memory_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mem == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Initialize the mutex
    if (pthread_mutex_init(&mem->mutex, NULL) != 0) {
        perror("pthread_mutex_init");
        exit(EXIT_FAILURE);
    }

    return mem;
}

void end_shared_memory(memory_t* mem) {
    if (munmap(mem, sizeof(memory_t)) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }
}

void destroy_shared_memory(void) {
    if (shm_unlink(SHARED_MEMORY_NAME) == -1) {
        perror("shm_unlink");
        exit(EXIT_FAILURE);
    }
}

sem_t* create_semaphore(void) {
    sem_t* sem;
    sem = sem_open(SEMAPHORE_NAME, O_CREAT | O_RDWR, 0644, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    return sem;
}

sem_t* open_semaphore(void) {
    sem_t* sem;
    sem = sem_open(SEMAPHORE_NAME, O_CREAT | O_RDWR, 0644, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    return sem;
}

void close_semaphore(sem_t* sem) {
    if (sem_close(sem) == -1) {
        perror("sem_close");
        exit(EXIT_FAILURE);
    }
}

void destroy_semaphore(sem_t* sem){
    sem_destroy(sem);
}
