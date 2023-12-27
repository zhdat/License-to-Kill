#ifndef PROJECT_ENEMY_SPY_NETWORK_H
#define PROJECT_ENEMY_SPY_NETWORK_H
#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include "memory.h"
#include "tools.h"
#include "common.h"
#include "monitor.h"

typedef struct {
    int id;
    memory_t* mem;
} agent_thread_args_t;


typedef struct {
    pthread_t source_agent_threads[MAX_SOURCE_AGENT_COUNT];
    agent_thread_args_t source_agent_args[MAX_SOURCE_AGENT_COUNT];
    pthread_t attending_officer_threads[MAX_ATTENDING_OFFICER_COUNT];
    agent_thread_args_t attending_officer_args[MAX_ATTENDING_OFFICER_COUNT];
} all_threads_t;

void move_source_agent(memory_t* mem, int row, int column, int id);
void move_attending_officer(memory_t* mem, int row, int column);
int is_valid_move(int column_end, int row_end, memory_t* mem);
void source_agent_thread_func(void* arg, memory_t* mem);
void create_and_run_source_agent_threads(memory_t* mem);
void create_and_run_attending_officer_threads(memory_t* mem);

#endif //PROJECT_ENEMY_SPY_NETWORK_H
