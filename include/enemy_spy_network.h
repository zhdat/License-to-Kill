#ifndef PROJECT_ENEMY_SPY_NETWORK_H
#define PROJECT_ENEMY_SPY_NETWORK_H
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

typedef struct pid_to_agent_map {
    int pid;
    source_agent_t *agent;
} pid_to_agent_map_t;

void move_source_agent(agent_thread_args_t* arg, int row, int column);
void move_attending_officer(agent_thread_args_t* arg, int row, int column);
void * morning_source_agent(void* arg);
void * morning_attending_officer(void* arg);
void * evening_source_agent(void* arg);
void * evening_attending_officer(void* arg);
void create_network_morning_thread(memory_t* mem, all_threads_t* threads);
void create_network_evening_thread(memory_t *mem, all_threads_t *threads);
void handle_signal(int);
void set_signals(void);
void set_semaphore(sem_t* sem);
void create_enemy_spy_thread(memory_t* mem);
void map_pid_to_agent(int pid, source_agent_t *agent, int id);
source_agent_t* get_agent_by_pid(int pid);
void handle_sigusr1(int sig, siginfo_t *info, void *unused);
void set_signals_bullet(void);

#endif //PROJECT_ENEMY_SPY_NETWORK_H
