#include "counterintelligence_officer.h"

volatile int signal_received_officer = 0;

sem_t* move_sem;

void set_semaphore(sem_t* sem) {
    move_sem = sem;
}

void handle_signal(int sig) {
    signal_received_officer = 1;
}

void set_signals(void) {
    struct sigaction action;

    action.sa_handler = handle_signal;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);

    sigaction(SIGALRM, &action, NULL);
}

void move_counter_intelligence_officer(officer_thread_args_t* arg, int row, int column, int index) {
    int start_row, start_column;
    memory_t* mem = arg->mem;
    counter_intelligence_officer_t* officer = &(mem->counter_intelligence_officers[arg->id]);

    start_row = officer->character.row;
    start_column = officer->character.column;

    coordinate_t start_cell;
    start_cell.row = start_row;
    start_cell.column = start_column;
    coordinate_t end_cell;
    end_cell.row = row;
    end_cell.column = column;

    sem_wait(move_sem);
    decrements_population_in_cell(mem, start_column, start_row);
    next_move(&(mem->city_map), start_cell, end_cell, &officer->character.column, &officer->character.row);
    increments_population_in_cell(mem, officer->character.column, officer->character.row);

    // Vérifier si un l'espion ciblé est sur la même cellule
        source_agent_t* agent = &(mem->source_agents[index]);
        if (agent->character.row == officer->character.row && agent->character.column == officer->character.column && agent->character.id == officer->targeted_character_id) {
            // Un espion est sur la même cellule, envoyez un signal SIGUSR1
            if(agent->character.row != agent->character.home_row && agent->character.column != agent->character.home_column) {
                if (mem->my_timer.hours >= 2) {
                    //log_info("Agent ID: %d, PID: %d, Row: %d, Column: %d", agent->character.id, agent->character.pid, agent->character.row, agent->character.column);
                    //log_info("id of the spy: %d", agent->character.id);
                    if (agent->character.pid != 0) {
                        agent->is_attacked = 1;
                        int injury_type = rand() % 2;
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
        if (signal_received_officer == 1) {
            signal_received_officer = 0;
            for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; i++) {
                if (officer->targeted_character_id == mem->source_agents[i].character.id) {
                    move_counter_intelligence_officer(arg, mem->source_agents[i].character.row,
                                                      mem->source_agents[i].character.column, i);
                    break;
                }
            }
        }
    }
    sleep(1);
    pthread_exit(NULL);
}

void create_counter_intelligence_officer_thread(memory_t* mem, all_threads_t* args) {
    pthread_attr_t attr;
    officer_thread_args_t* ptr;

    ptr = &args->counter_intelligence_officer_args[0];
    pthread_attr_init(&attr);
    if (pthread_create(&args->counter_intelligence_officer_threads[0], &attr, all_day_counter_intelligence_officer,
                       ptr) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    // joindre les threads
    for (int i = 0; i < MAX_COUNTER_INTELLIGENCE_OFFICER_COUNT; i++) {
        pthread_join(args->counter_intelligence_officer_threads[i], NULL);
    }
}

void create_counter_intelligence_officer_threads(memory_t* mem) {
    all_threads_t* threads;
    threads = malloc(sizeof(all_threads_t));
    if (threads == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < MAX_COUNTER_INTELLIGENCE_OFFICER_COUNT; i++) {
        threads->counter_intelligence_officer_args[i].id = i;
        threads->counter_intelligence_officer_args[i].mem = mem;
    }
    while (mem->simulation_has_ended == 0) {
        create_counter_intelligence_officer_thread(mem, threads);
    }
}
