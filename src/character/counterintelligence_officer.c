#include "counterintelligence_officer.h"
#include "debug.h"

volatile int signal_received_officer = 0;

sem_t* move_sem;

void set_semaphore(sem_t* sem) {
    move_sem = sem;
}

void handle_signal(int sig) {
    sig = sig;
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
              &(officer->character));
    increments_population_in_cell(mem, officer->character.column, officer->character.row);

    // Vérifier si un l'espion ciblé est sur la même cellule
    source_agent_t* agent = &(mem->source_agents[index]);
    if (agent->character.row == officer->character.row && agent->character.column == officer->character.column &&
        agent->character.id == officer->targeted_character_id) {
        // Un espion est sur la même cellule, envoyez un signal SIGUSR1
        if (agent->character.row != agent->character.home_row &&
            agent->character.column != agent->character.home_column) {
            if (mem->timer.hours >= 2) {
                //log_info("Agent ID: %d, PID: %d, Row: %d, Column: %d", agent->character.id, agent->character.pid, agent->character.row, agent->character.column);
                //log_info("id of the spy: %d", agent->character.id);
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

    int target_id[4];
    for (int k = 0; k < 4; k++) {
        for (int i = 0; i < MAX_ROWS; i++) {
            for (int j = 0; i < MAX_COLUMNS; i++) {
                detect_movement(&(mem->city_map), i, j, mem);
                target_id[k] = detect_movement_to_id(mem, i, j);
            }
        }
    }
    log_info("target id: %d", target_id[0]);
    officer->targeted_character_id = target_id[0];

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
}

int detect_movement_to_id(memory_t* mem, int row, int column) {
    if (mem->city_map.cells[row][column].sensor_data.has_motion == 1) {
        for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; i++) {
            if (mem->city_map.cells[row][column].ids[i] != -1) {
                return mem->city_map.cells[row][column].ids[i];
            }
        }
    }
    return -1;
}

void detect_movement(city_t* city, int x, int y, memory_t* mem) {
    cell_t* cell = get_cell(city, x, y);
    if (cell == NULL || !cell->sensor_data.camera_active) {
        return;
    }

    // Conditions pour déterminer un mouvement suspect
    // Exemple: Un personnage reste trop longtemps dans une entreprise ou l'hôtel de ville
    if ((cell->type == COMPANY || cell->type == CITY_HALL) && cell->nb_of_characters > 0 && (mem->timer.hours >= 20 || mem->timer.hours < 8)){
        // Supposons que chaque appel à cette fonction représente une unité de temps
        cell->sensor_data.detected_time++;

        if (cell->sensor_data.detected_time > SOME_SUSPICIOUS_TIME_THRESHOLD) {
            cell->sensor_data.has_motion = 1;
        }
    } else {
        // Réinitialiser le compteur de temps si les conditions ne sont pas remplies
        cell->sensor_data.detected_time = 0;
        cell->sensor_data.has_motion = 0;
    }
}
