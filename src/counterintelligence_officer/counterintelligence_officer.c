#include "counterintelligence_officer.h"
#include "debug.h"

/**
 * \file main.c
 * \brief Defines functions for initializing and managing the counterintelligence officer.
 */

volatile int signal_received_officer = 0;

sem_t* move_sem;

pthread_mutex_t signal_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t signal_cond = PTHREAD_COND_INITIALIZER;


void set_semaphore(sem_t* sem) {
    move_sem = sem;
}

void handle_signal(int sig) {
    sig = sig;
    pthread_mutex_lock(&signal_mutex);
    signal_received_officer = 1;
    pthread_cond_signal(&signal_cond);
    pthread_mutex_unlock(&signal_mutex);
}

void set_signals(void) {
    struct sigaction action;

    action.sa_handler = handle_signal;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);

    sigaction(SIGALRM, &action, NULL);
}

static int is_right_target(counter_intelligence_officer_t* officer, source_agent_t* agent) {
    return officer->targeted_character_id == agent->character.id ? 1 : 0;
}

void move_counter_intelligence_officer(officer_thread_args_t* arg, int row, int column, int index) {
    int start_row, start_column;
    memory_t* mem = arg->mem;
    counter_intelligence_officer_t* officer = &(mem->counter_intelligence_officers[arg->id]);
    coordinate_t start_cell;
    coordinate_t end_cell;

    start_row = officer->character.row;
    start_column = officer->character.column;

    start_cell.row = start_row;
    start_cell.column = start_column;
    end_cell.row = row;
    end_cell.column = column;

    sem_wait(move_sem);
    decrements_population_in_cell(mem, start_column, start_row);
    next_move(&(mem->city_map), start_cell, end_cell, &officer->character.column, &officer->character.row,
              officer->character);
    increments_population_in_cell(mem, officer->character.column, officer->character.row);

    /* Check if there is a spy on the same cell */
    source_agent_t* agent = &(mem->source_agents[index]);
    if (characters_are_at_same_cell(agent->character, officer->character) && is_right_target(officer, agent)) {
        /* Check if the spy is not at home */
        if (!is_at_home(agent->character)) {
            if (mem->timer.hours >= 2) {
                if (agent->character.pid != 0) {
                    agent->is_attacked = 1;
                    int injury_type = selectRandomNumberUnder(2);
                    if (injury_type == 0) {
                        kill(agent->character.pid, SIGUSR1);
                    } else {
                        kill(agent->character.pid, SIGUSR2);
                    }
                }
            }
        }
    }

    sem_post(move_sem);
}

void* all_day_counter_intelligence_officer(void* args) {
    officer_thread_args_t* arg = (officer_thread_args_t*) args;
    memory_t* mem = arg->mem;
    counter_intelligence_officer_t* officer = &(mem->counter_intelligence_officers[arg->id]);

    while (officer->targeted_character_id != -1) {
        pthread_mutex_lock(&signal_mutex);
        while (signal_received_officer == 0) {
            pthread_cond_wait(&signal_cond, &signal_mutex);
        }
        signal_received_officer = 0;
        pthread_mutex_unlock(&signal_mutex);
        for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; i++) {
            if (is_right_target(officer, &(mem->source_agents[i]))) {
                move_counter_intelligence_officer(arg, mem->source_agents[i].character.row,
                                                  mem->source_agents[i].character.column, i);
                break;
            }
        }
    }

    sleep(1);
    pthread_exit(NULL);
}

void create_counter_intelligence_officer_thread(all_threads_t* args) {
    pthread_attr_t attr;
    officer_thread_args_t* ptr;

    ptr = &args->counter_intelligence_officer_args[0];
    pthread_attr_init(&attr);
    if (pthread_create(&args->counter_intelligence_officer_threads[0], &attr, all_day_counter_intelligence_officer,
                       ptr) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < MAX_COUNTER_INTELLIGENCE_OFFICER_COUNT; i++) {
        pthread_join(args->counter_intelligence_officer_threads[i], NULL);
    }
}

void create_counter_intelligence_officer_threads(memory_t* mem) {
    all_threads_t* threads;
    threads = (all_threads_t*) malloc(sizeof(all_threads_t));
    if (threads == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < MAX_COUNTER_INTELLIGENCE_OFFICER_COUNT; i++) {
        threads->counter_intelligence_officer_args[i].id = i;
        threads->counter_intelligence_officer_args[i].mem = mem;
    }
    while (mem->simulation_has_ended == 0) {
        create_counter_intelligence_officer_thread(threads);
    }
    free(threads);
}
