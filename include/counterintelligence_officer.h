#ifndef PROJECT_COUNTERINTELLIGENCE_OFFICER_H
#define PROJECT_COUNTERINTELLIGENCE_OFFICER_H

#include <semaphore.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "memory.h"
#include "tools.h"
#include "common.h"
#include "monitor.h"
#include "logger.h"
#include "character.h"

typedef struct {
    int id;
    memory_t* mem;
} officer_thread_args_t;

typedef struct {
    pthread_t counter_intelligence_officer_threads[MAX_COUNTER_INTELLIGENCE_OFFICER_COUNT];
    officer_thread_args_t counter_intelligence_officer_args[MAX_COUNTER_INTELLIGENCE_OFFICER_COUNT];
} all_threads_t;

void set_semaphore(sem_t* sem);

void handle_signal(int sig);

void set_signals(void);

void move_counter_intelligence_officer(officer_thread_args_t* arg, int row, int column, int index);

void* all_day_counter_intelligence_officer(void* args);

void create_counter_intelligence_officer_thread(memory_t* mem, all_threads_t* args);

void create_counter_intelligence_officer_threads(memory_t* mem);

#endif //PROJECT_COUNTERINTELLIGENCE_OFFICER_H
