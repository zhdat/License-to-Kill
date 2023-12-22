#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "memory.h"

memory_t *open_shared_memory(void){
    int fd;
    memory_t *memory;

    fd = shm_open(SHARED_MEMORY_NAME, O_RDWR, 0660);
    if (fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    memory = mmap(NULL, sizeof(memory_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (memory == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    return memory;
}

void close_shared_memory(memory_t *memory){
    munmap(memory, sizeof(memory_t));
}
