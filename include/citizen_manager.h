#ifndef TP_SYS_CITIZEN_MANGER_H
#define TP_SYS_CITIZEN_MANGER_H

#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include "memory.h"
#include "tools.h"
#include "common.h"
#include "monitor.h"
#include "logger.h"

typedef struct {
    int id;
    memory_t* mem;
} citizen_thread_args_t;

typedef struct {
    pthread_t citizen_threads[MAX_CITIZEN_COUNT];
    citizen_thread_args_t citizen_args[MAX_CITIZEN_COUNT];
} all_threads_t;

void signal_handler(int signum);

void move_citizen(memory_t* mem, int row, int column, int id);

int is_valid_move(int column_end, int row_end, memory_t* mem);

void* citizen_thread_func(void* arg);

void create_and_run_citizen_threads(memory_t* mem, all_threads_t* threads);

#endif //TP_SYS_CITIZEN_MANGER_H
