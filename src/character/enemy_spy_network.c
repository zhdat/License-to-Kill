#include <stdlib.h>
#include <unistd.h>
#include "enemy_spy_network.h"

volatile int signal_received_spies[MAX_SOURCE_AGENT_COUNT] = {0, 0, 0};
volatile int signal_received_officer = 0;
tid_to_agent_map_t agent_map[MAX_SOURCE_AGENT_COUNT] = {{0, 0},
                                                        {0, 0},
                                                        {0, 0}};

sem_t* move_sem;

void set_semaphore(sem_t* sem) {
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

void map_tid_to_agent(pthread_t tid, source_agent_t *agent, int id) {
    //log_debug("Mapping TID %ld to agent %d\n", tid, id);
    agent_map[id].tid = tid;
    agent_map[id].agent = agent;
}

int get_agent_by_tid(pthread_t tid) {
    for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; i++) {
        // log_info("Comparing TID %ld to TID n:%d %ld", tid,i, agent_map[i].tid );
        if (pthread_equal(agent_map[i].tid, tid)) { // Utilisez pthread_equal pour comparer les TID
            return i;
        }
    }
    return -1; // Thread non trouvé
}

void handle_sigusr1(int sig, siginfo_t *info, void *unused) {
    /*pthread_t tid = pthread_self(); // Utiliser pthread_self() pour obtenir le TID
    int index = get_agent_by_tid(tid);
    log_info("Tid %ld received SIGUSR1", tid);
    //log_info("Agent %d was shot", index);
    if (index != -1) {
        source_agent_t *agent = agent_map[index].agent;
        agent->character.health = 1;
    }*/
    for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; i++) {
        // log_debug("is agent %d attacked ? %d", i, agent_map[i].agent->is_attacked);
        if (agent_map[i].agent->is_attacked == 1) {
            source_agent_t *agent = agent_map[i].agent;
            sem_wait(move_sem);
            agent->character.health--;
            agent->is_attacked = 0;
            sem_post(move_sem);
            break;
        }
    }
}

void set_signals_bullet(void) {
    struct sigaction action;

    action.sa_flags = 0;
    action.sa_sigaction = handle_sigusr1;
    sigemptyset(&action.sa_mask);

    sigaction(SIGUSR1, &action, NULL);
}

void move_source_agent(agent_thread_args_t* arg, int row, int column) {
    int start_row, start_column;
    memory_t* mem = arg->mem;
    source_agent_t* spies = &(mem->source_agents[arg->id]);

    if (spies->character.health == 0) {
        return;
    }

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

void move_attending_officer(agent_thread_args_t* arg, int row, int column) {
    int start_row, start_column;
    memory_t* mem = arg->mem;
    attending_officer_t* officer = &(mem->attending_officers[arg->id]);

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

void* morning_source_agent(void* arg) {

    agent_thread_args_t* args = (agent_thread_args_t*) arg;
    int pid = getpid();

    pthread_t tid = pthread_self(); // Obtenez le TID du thread actuel
    source_agent_t *current_agent = &(args->mem->source_agents[args->id]);
    current_agent->character.pid = pid;
    map_tid_to_agent(tid, current_agent, args->id); // Modifiez cette fonction pour utiliser le TID


    int random_activity = rand() % 100;

    if (random_activity < 10) {
        // va au supermarché
        int random_supermarket = rand() % NUMBER_OF_SUPERMARKETS;
        coordinate_t* supermarket_coordinates = findTypeOfBuilding(&args->mem->city_map, SUPERMARKET,
                                                                   NUMBER_OF_SUPERMARKETS);


        while (!character_is_at(args->mem->source_agents[args->id].character, supermarket_coordinates[random_supermarket])) {
            if (signal_received_spies[args->id]) {
                move_source_agent(args, supermarket_coordinates[random_supermarket].row,
                                  supermarket_coordinates[random_supermarket].column);
                signal_received_spies[args->id] = 0;
                if(args->mem->source_agents[args->id].character.health == 0){
                    break;
                }
            }
        }
        // il rentre chez lui
        while (!is_at_home(args->mem->source_agents[args->id].character)) {
            if (signal_received_spies[args->id]) {
                move_source_agent(args, args->mem->source_agents[args->id].character.home_row,
                                  args->mem->source_agents[args->id].character.home_column);
                signal_received_spies[args->id] = 0;
                if(args->mem->source_agents[args->id].character.health == 0){
                    break;
                }
            }
        }

    } else if (random_activity >= 10 && random_activity < 40) {
        // reste chez lui

    } else {

        int random_company = rand() % NUMBER_OF_COMPANIES;
        coordinate_t* companies_coordinates = findTypeOfBuilding(&args->mem->city_map, COMPANY, NUMBER_OF_COMPANIES);


        while (!character_is_at(args->mem->source_agents[args->id].character, companies_coordinates[random_company])) {
            if (signal_received_spies[args->id]) {
                move_source_agent(args, companies_coordinates[random_company].row,
                                  companies_coordinates[random_company].column);
                signal_received_spies[args->id] = 0;
                if (args->mem->source_agents[args->id].character.health == 0) {
                    break;
                }
            }
        }

        // il rentre chez lui

        while (!is_at_home(args->mem->source_agents[args->id].character)) {
            if (signal_received_spies[args->id]) {
                move_source_agent(args, args->mem->source_agents[args->id].character.home_row,
                                  args->mem->source_agents[args->id].character.home_column);
                signal_received_spies[args->id] = 0;
                if (args->mem->source_agents[args->id].character.health == 0) {
                    break;
                }
            }
        }
    }

    sleep(1);
    pthread_exit(NULL);
}

void *evening_source_agent(void *arg) {
    agent_thread_args_t *args = (agent_thread_args_t *) arg;

    int pid = getpid();
    pthread_t tid = pthread_self(); // Obtenez le TID du thread actuel
    source_agent_t *current_agent = &(args->mem->source_agents[args->id]);
    map_tid_to_agent(tid, current_agent, args->id); // Modifiez cette fonction pour utiliser le TID
    current_agent->character.pid = pid;


    while (!is_at_home(args->mem->source_agents[args->id].character)) {
        if (signal_received_spies[args->id]) {
            move_source_agent(args, args->mem->source_agents[args->id].character.home_row,
                              args->mem->source_agents[args->id].character.home_column);
            signal_received_spies[args->id] = 0;
            if (args->mem->source_agents[args->id].character.health == 0) {
                break;
            }
        }
    }

    sleep(1);
    pthread_exit(NULL);
}

void *evening_attending_officer(void *arg) {
    agent_thread_args_t *args = (agent_thread_args_t *) arg;
    while (!is_at_home(args->mem->attending_officers[args->id].character)) {
        if (signal_received_officer) {
            move_attending_officer(args, args->mem->attending_officers[args->id].character.home_row,
                                   args->mem->attending_officers[args->id].character.home_column);
            signal_received_officer = 0;
            if(args->mem->attending_officers[args->id].character.health == 0){
                break;
            }
        }
    }

    sleep(1);
    pthread_exit(NULL);
}

void* morning_attending_officer(void* arg) {
    agent_thread_args_t* args = (agent_thread_args_t*) arg;

    coordinate_t next = {rand() % 7, rand() % 7};
    
    while (!character_is_at(args->mem->attending_officers[args->id].character, next)) {
        if (signal_received_officer) {
            move_attending_officer(args, next.row, next.column);
            signal_received_officer = 0;
            if(args->mem->attending_officers[args->id].character.health == 0){
                break;
            }
        }

    }

    sleep(1);
    pthread_exit(NULL);
}


void create_network_morning_thread(memory_t* mem, all_threads_t* threads) {
    pthread_attr_t attr;
    agent_thread_args_t* ptr;
    agent_thread_args_t* ptr2;

    if (mem->my_timer.hours >= 2 && mem->my_timer.minutes == 0 && mem->my_timer.hours <= 17) {
        for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; ++i) {
            if(mem->source_agents[i].character.health == 0){
                continue;
            }
            ptr = &threads->source_agent_args[i];
            pthread_attr_init(&attr);
            if (pthread_create(&threads->source_agent_threads[i], &attr, morning_source_agent,
                               ptr) == 0) {
            } else {
                printf("thread not created\n");
            }
            sleep(1);
        }

        if (mem->attending_officers[0].character.health == 0) {
            ptr2 = &threads->attending_officer_args[0];
            pthread_attr_init(&attr);
            if (pthread_create(&threads->attending_officer_threads[0], &attr, morning_attending_officer,
                               ptr2) == 0) {
            } else {
                printf("thread not created\n");
            }
        }
        // joindre les threads

        for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; ++i) {
            if(mem->source_agents[i].character.health == 0){
                continue;
            }
            pthread_join(threads->source_agent_threads[i], NULL);
        }
        if(mem->attending_officers[0].character.health == 0) {
            pthread_join(threads->attending_officer_threads[0], NULL);
        }
    }

}

void create_network_evening_thread(memory_t* mem, all_threads_t* threads) {
    pthread_attr_t attr;
    agent_thread_args_t* ptr;
    agent_thread_args_t* ptr2;

    if (mem->my_timer.hours == 17 && mem->my_timer.minutes == 0) {

        for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; ++i) {
            if(mem->source_agents[i].character.health == 0){
                continue;
            }
            ptr = &threads->source_agent_args[i];
            pthread_attr_init(&attr);
            if (pthread_create(&threads->source_agent_threads[i], &attr, evening_source_agent,
                               ptr) == 0) {
            } else {
                printf("thread not created\n");
            }
            sleep(1);

        }

        if(mem->attending_officers[0].character.health == 0) {
            ptr2 = &threads->attending_officer_args[0];
            pthread_attr_init(&attr);
            if (pthread_create(&threads->attending_officer_threads[0], &attr, evening_attending_officer,
                               ptr2) == 0) {
            } else {
                printf("thread not created\n");
            }
        }
        // joindre les threads

        for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; ++i) {
            if(mem->source_agents[i].character.health == 0){
                continue;
            }
            pthread_join(threads->source_agent_threads[i], NULL);
        }
        if(mem->attending_officers[0].character.health == 0) {
            pthread_join(threads->attending_officer_threads[0], NULL);
        }
    }
}


void create_enemy_spy_thread(memory_t* mem) {
    all_threads_t* threads;
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





