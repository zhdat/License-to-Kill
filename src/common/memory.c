#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "memory.h"

memory_t *open_shared_memory(void){
    int fd;
<<<<<<< HEAD
    memory_t *mem;
=======
    memory_t *memory;
>>>>>>> a22440242775064688c85465b3e67225034773c7

    fd = shm_open(SHARED_MEMORY_NAME, O_RDWR, 0660);
    if (fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
<<<<<<< HEAD
    mem = mmap(NULL, sizeof(memory_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mem == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    return mem;
}

void end_shared_memory(memory_t* mem) {
    if (munmap(mem, sizeof(memory_t)) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }
=======

    memory = mmap(NULL, sizeof(memory_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (memory == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    return memory;
}

void close_shared_memory(memory_t *memory){
    munmap(memory, sizeof(memory_t));
>>>>>>> a22440242775064688c85465b3e67225034773c7
}
