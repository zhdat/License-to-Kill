#include <stdlib.h>
#include <unistd.h>
#include "enemy_spy_network.h"

volatile int signal_received_spies[MAX_SOURCE_AGENT_COUNT] = {0, 0, 0};
volatile int signal_received_officer = 0;
pid_to_agent_map_t agent_map[MAX_SOURCE_AGENT_COUNT] = {{0, 0}, {0, 0}, {0, 0}};

sem_t *move_sem;

void set_semaphore(sem_t *sem) {
    move_sem = sem;
}

void handle_signal(int sig) {
    for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; ++i) {
        signal_received_spies[i] = 1;
    }
    signal_received_officer = 1;
}

void set_signals(void) {
    struct sigaction action;

    action.sa_handler = handle_signal;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);

    sigaction(SIGALRM, &action, NULL);
}

// Fonction pour initialiser la correspondance PID -> Espion
void map_pid_to_agent(int pid, source_agent_t *agent) {
    for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; i++) {
        if (agent_map[i].pid == 0) { // Trouver un emplacement vide
            agent_map[i].pid = pid;
            agent_map[i].agent = agent;
            break;
        }
    }
}

source_agent_t* get_agent_by_pid(int pid) {
    for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; i++) {
        if (agent_map[i].pid == pid) {
            return agent_map[i].agent;
        }
    }
    return NULL; // Espion non trouvé
}

// Gestionnaire de signal SIGUSR1
void handle_sigusr1(int sig, siginfo_t *info, void *unused) {
    int pid = getpid(); // Obtenir le PID du thread actuel
    source_agent_t *agent = get_agent_by_pid(pid);

    if (agent != NULL) {
        // Mettre à jour la santé de l'espion
        agent->character.health -= 1;
    }
}

void set_signals_bullet(void) {
    struct sigaction action;

    action.sa_flags = SA_SIGINFO;
    action.sa_sigaction = handle_sigusr1;
    sigemptyset(&action.sa_mask);

    sigaction(SIGUSR1, &action, NULL);
}

void move_source_agent(agent_thread_args_t *arg, int row, int column) {
    int start_row, start_column;
    memory_t *mem = arg->mem;
    source_agent_t *spies = &(mem->source_agents[arg->id]);

    start_row = spies->character.row;
    start_column = spies->character.column;


    coordinate_t start_cell;
    start_cell.row = start_row;
    start_cell.column = start_column;
    coordinate_t end_cell;
    end_cell.row = row;
    end_cell.column = column;


    sem_wait(move_sem);
    decrements_population_in_cell(mem, start_column, start_row);
    next_move(&(mem->city_map), start_cell, end_cell, &spies->character.column, &spies->character.row);
    increments_population_in_cell(mem, spies->character.column, spies->character.row);
    sem_post(move_sem);

}

void move_attending_officer(agent_thread_args_t *arg, int row, int column) {
    int start_row, start_column;
    memory_t *mem = arg->mem;
    attending_officer_t *officer = &(mem->attending_officers[arg->id]);

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
    sem_post(move_sem);
}

void *morning_source_agent(void *arg) {
    agent_thread_args_t *args = (agent_thread_args_t *) arg;
    /*int pid = getpid();
    // Obtenez la référence à l'objet source_agent pour le thread courant
    source_agent_t *current_agent = &(args->mem->source_agents[args->id]);

    // Mappez le PID du thread à l'objet source_agent correspondant
    map_pid_to_agent(pid, current_agent);*/

    int random_activity = rand() % 100;

    if (random_activity < 10){
        // va au supermarché
        int random_supermarket = rand() % NUMBER_OF_SUPERMARKETS;
        coordinate_t *supermarket_coordinates = findTypeOfBuilding(&args->mem->city_map, SUPERMARKET, NUMBER_OF_SUPERMARKETS);

        while ((args->mem->source_agents[args->id].character.row != supermarket_coordinates[random_supermarket].row)
               ||
               (args->mem->source_agents[args->id].character.column != supermarket_coordinates[random_supermarket].column)) {
            if (signal_received_spies[args->id]) {
                move_source_agent(args, supermarket_coordinates[random_supermarket].row,
                                  supermarket_coordinates[random_supermarket].column);
                signal_received_spies[args->id] = 0;
            }
        }
        // il rentre chez lui
        while ((args->mem->source_agents[args->id].character.row !=
                args->mem->source_agents[args->id].character.home_row)
               || (args->mem->source_agents[args->id].character.column !=
                   args->mem->source_agents[args->id].character.home_column)) {
            if (signal_received_spies[args->id]) {
                move_source_agent(args, args->mem->source_agents[args->id].character.home_row,
                                  args->mem->source_agents[args->id].character.home_column);
                signal_received_spies[args->id] = 0;
            }
        }

    } else if (random_activity >= 10 && random_activity < 40) {
        // reste chez lui

    } else {

        int random_company = rand() % NUMBER_OF_COMPANIES;
        coordinate_t *companies_coordinates = findTypeOfBuilding(&args->mem->city_map, COMPANY, NUMBER_OF_COMPANIES);


        while ((args->mem->source_agents[args->id].character.row != companies_coordinates[random_company].row)
               ||
               (args->mem->source_agents[args->id].character.column != companies_coordinates[random_company].column)) {
            if (signal_received_spies[args->id]) {
                move_source_agent(args, companies_coordinates[random_company].row,
                                  companies_coordinates[random_company].column);
                signal_received_spies[args->id] = 0;
            }
        }

        // il rentre chez lui

        while ((args->mem->source_agents[args->id].character.row !=
                args->mem->source_agents[args->id].character.home_row)
               || (args->mem->source_agents[args->id].character.column !=
                   args->mem->source_agents[args->id].character.home_column)) {
            if (signal_received_spies[args->id]) {
                move_source_agent(args, args->mem->source_agents[args->id].character.home_row,
                                  args->mem->source_agents[args->id].character.home_column);
                signal_received_spies[args->id] = 0;
            }
        }
    }

    sleep(1);
    pthread_exit(NULL);
}

void *evening_source_agent(void *arg) {
    agent_thread_args_t *args = (agent_thread_args_t *) arg;
    while ((args->mem->source_agents[args->id].character.row !=
            args->mem->source_agents[args->id].character.home_row)
           || (args->mem->source_agents[args->id].character.column !=
               args->mem->source_agents[args->id].character.home_column)) {
        if (signal_received_spies[args->id]) {
            move_source_agent(args, args->mem->source_agents[args->id].character.home_row,
                              args->mem->source_agents[args->id].character.home_column);
            signal_received_spies[args->id] = 0;
        }
    }

    sleep(1);
    pthread_exit(NULL);
}

void *evening_attending_officer(void *arg) {
    agent_thread_args_t *args = (agent_thread_args_t *) arg;
    while ((args->mem->attending_officers[args->id].character.row !=
            args->mem->attending_officers[args->id].character.home_row)
           || (args->mem->attending_officers[args->id].character.column !=
               args->mem->attending_officers[args->id].character.home_column)) {
        if (signal_received_officer) {
            move_attending_officer(args, args->mem->attending_officers[args->id].character.home_row,
                                   args->mem->attending_officers[args->id].character.home_column);
            signal_received_officer = 0;
        }
    }

    sleep(1);
    pthread_exit(NULL);
}

void *morning_attending_officer(void *arg) {
    agent_thread_args_t *args = (agent_thread_args_t *) arg;

    int next_row = rand() % 7;
    int next_column = rand() % 7;
    while ((args->mem->attending_officers[args->id].character.row != next_row)
           || (args->mem->attending_officers[args->id].character.column != next_column)) {
        if (signal_received_officer) {
            move_attending_officer(args, next_row, next_column);
            signal_received_officer = 0;
        }

    }

    sleep(1);
    pthread_exit(NULL);
}


void create_network_morning_thread(memory_t *mem, all_threads_t *threads) {
    pthread_attr_t attr;
    agent_thread_args_t *ptr;
    agent_thread_args_t *ptr2;

    if (mem->my_timer.hours >= 8 && mem->my_timer.minutes == 0 && mem->my_timer.hours <= 17) {
        for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; ++i) {
            ptr = &threads->source_agent_args[i];
            pthread_attr_init(&attr);
            if (pthread_create(&threads->source_agent_threads[i], &attr, morning_source_agent,
                               ptr) == 0) {
            } else {
                printf("thread not created\n");
            }
            sleep(1);

        }

        ptr2 = &threads->attending_officer_args[0];
        pthread_attr_init(&attr);
        if (pthread_create(&threads->attending_officer_threads[0], &attr, morning_attending_officer,
                           ptr2) == 0) {
        } else {
            printf("thread not created\n");
        }

        // joindre les threads

        for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; ++i) {
            pthread_join(threads->source_agent_threads[i], NULL);
        }
        pthread_join(threads->attending_officer_threads[0], NULL);

    }

}

void create_network_evening_thread(memory_t *mem, all_threads_t *threads) {
    pthread_attr_t attr;
    agent_thread_args_t *ptr;
    agent_thread_args_t *ptr2;

    if (mem->my_timer.hours == 17 && mem->my_timer.minutes == 0) {

        for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; ++i) {
            ptr = &threads->source_agent_args[i];
            pthread_attr_init(&attr);
            if (pthread_create(&threads->source_agent_threads[i], &attr, evening_source_agent,
                               ptr) == 0) {
            } else {
                printf("thread not created\n");
            }
            sleep(1);

        }

        ptr2 = &threads->attending_officer_args[0];
        pthread_attr_init(&attr);
        if (pthread_create(&threads->attending_officer_threads[0], &attr, evening_attending_officer,
                           ptr2) == 0) {
        } else {
            printf("thread not created\n");
        }

        // joindre les threads

        for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; ++i) {
            pthread_join(threads->source_agent_threads[i], NULL);
        }
        pthread_join(threads->attending_officer_threads[0], NULL);
    }
}


void create_enemy_spy_thread(memory_t *mem) {
    all_threads_t *threads;
    threads = malloc(sizeof(all_threads_t));
    srand(time(NULL));

    for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; i++) {
        threads->source_agent_args[i].id = i;
        threads->source_agent_args[i].mem = mem;

    }
    for (int i = 0; i < MAX_ATTENDING_OFFICER_COUNT; i++) {
        threads->attending_officer_args[i].id = i;
        threads->attending_officer_args[i].mem = mem;

    }


    while (mem->simulation_has_ended == 0) {
        create_network_morning_thread(mem, threads);
        create_network_evening_thread(mem, threads);
        //create_network_night_thread(mem, threads);
    }
}





