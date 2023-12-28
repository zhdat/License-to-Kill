#ifndef TP_SYS_CITIZEN_MANGER_H
#define TP_SYS_CITIZEN_MANGER_H

#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include "memory.h"
#include "tools.h"
#include "common.h"
#include "monitor.h"
#include "logger.h"
#include "cell.h"


typedef struct {
    int id;
    memory_t* mem;
} citizen_thread_args_t;

typedef struct {
    int index;
    memory_t* mem;
} citizen_monitor_args_t;

void set_semaphore(sem_t* sem);

void timer_handler(int signum);

void set_timer_citizens(void);

void set_signal_handler(void);

void move_citizen(citizen_monitor_args_t* arg, int row, int column);

void* citizen_to_work(void* args);

void* citizen_to_home(void* args);

void* citizen_to_home_supermarket(void* args);

int work_in_supermarket(memory_t mem, character_t citizen);

void
create_morning_thread(memory_t* mem, pthread_t ids[MAX_CITIZEN_COUNT], citizen_monitor_args_t* args[MAX_CITIZEN_COUNT]);

void create_evening_company_thread(memory_t* mem, pthread_t ids[MAX_CITIZEN_COUNT],
                                   citizen_monitor_args_t* args[MAX_CITIZEN_COUNT]);

void create_evening_supermarket_thread(memory_t* mem, pthread_t ids[MAX_CITIZEN_COUNT],
                                       citizen_monitor_args_t* args[MAX_CITIZEN_COUNT]);

void create_citizens_thread(memory_t* mem);

#endif //TP_SYS_CITIZEN_MANGER_H
