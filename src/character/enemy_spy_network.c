#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "enemy_spy_network.h"

volatile int signal_received_spies[MAX_SOURCE_AGENT_COUNT] = {0, 0, 0};
volatile int signal_received_officer = 0;
source_agent_t* agent_map[MAX_SOURCE_AGENT_COUNT];
int attending_officer_routine[8] = {0, 0, 0, 0, 0, 0, 0, 0 }; // position pair : heures, impair : minutes

sem_t *move_sem;

// @TODO: faire une map de chaques entreprise volée, par quel agent, et quand (P3)



void set_semaphore(sem_t *sem) {
    move_sem = sem;
}


void handle_signal(int sig) {
    sig = sig;
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

void agent_mapping(source_agent_t *agent, int id) {
    //log_debug("Mapping TID %ld to agent %d\n", tid, id);
    agent_map[id] = agent;
}

void handle_sigusr1(int sig, siginfo_t *info, void *unused) {
    sig = sig;
    info = info;
    unused = unused;
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
        if (agent_map[i]->is_attacked == 1) {
            source_agent_t *agent = agent_map[i];
            sem_wait(move_sem);
            agent->character.health--;
            agent->is_attacked = 0;
            sem_post(move_sem);
            break;
        }
    }
}

void set_signals_weak_bullet(void) {
    struct sigaction action;

    action.sa_flags = 0;
    action.sa_sigaction = handle_sigusr1;
    sigemptyset(&action.sa_mask);

    sigaction(SIGUSR1, &action, NULL);
}

void handle_sigusr2(int sig, siginfo_t *info, void *unused) {
    sig = sig;
    info = info;
    unused = unused;
    for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; i++) {
        // log_debug("is agent %d attacked ? %d", i, agent_map[i].agent->is_attacked);
        if (agent_map[i]->is_attacked == 1) {
            source_agent_t *agent = agent_map[i];
            sem_wait(move_sem);
            agent->character.health -= 2;
            agent->is_attacked = 0;
            sem_post(move_sem);
            break;
        }
    }
}

void set_signals_strong_bullet(void) {
    struct sigaction action;

    action.sa_flags = 0;
    action.sa_sigaction = handle_sigusr2;
    sigemptyset(&action.sa_mask);

    sigaction(SIGUSR2, &action, NULL);
}

void move_source_agent(agent_thread_args_t *arg, int row, int column) {
    int start_row, start_column;
    memory_t *mem = arg->mem;
    source_agent_t *spies = &(mem->source_agents[arg->id]);

    if (spies->character.health <= 0) {
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

void move_attending_officer(agent_thread_args_t *arg, int row, int column) {
    int start_row, start_column;
    memory_t *mem = arg->mem;
    attending_officer_t *officer = &(mem->attending_officers[arg->id]);

    if (officer->character.health <= 0) {
        return;
    }

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

    int pid = getpid();

    source_agent_t *current_agent = &(args->mem->source_agents[args->id]);
    current_agent->character.pid = pid;
    //agent_mapping(current_agent, args->id); // Modifiez cette fonction pour utiliser le TID


    int random_activity = rand() % 100;

    if (random_activity < 10) {
        // va au supermarché
        int random_supermarket = rand() % NUMBER_OF_SUPERMARKETS;
        coordinate_t *supermarket_coordinates = findTypeOfBuilding(&args->mem->city_map, SUPERMARKET,
                                                                   NUMBER_OF_SUPERMARKETS);


        while (!character_is_at(current_agent->character,
                                supermarket_coordinates[random_supermarket])) {
            if (signal_received_spies[args->id]) {
                move_source_agent(args, supermarket_coordinates[random_supermarket].row,
                                  supermarket_coordinates[random_supermarket].column);
                signal_received_spies[args->id] = 0;
                if (current_agent->character.health <= 0) {
                    break;
                }
            }
        }
        // il rentre chez lui
        while (!is_at_home(current_agent->character)) {
            if (signal_received_spies[args->id]) {
                move_source_agent(args, current_agent->character.home_row,
                                  current_agent->character.home_column);
                signal_received_spies[args->id] = 0;
                if (current_agent->character.health <= 0) {
                    break;
                }
            }
        }

    } else if (random_activity >= 10 && random_activity < 40) {
        // reste chez lui

    } else {

        int random_company = rand() % NUMBER_OF_COMPANIES;
        coordinate_t *companies_coordinates = findTypeOfBuilding(&args->mem->city_map, COMPANY, NUMBER_OF_COMPANIES);


        while (!character_is_at(current_agent->character, companies_coordinates[random_company])) {
            if (signal_received_spies[args->id]) {
                move_source_agent(args, companies_coordinates[random_company].row,
                                  companies_coordinates[random_company].column);
                signal_received_spies[args->id] = 0;
                if (current_agent->character.health <= 0) {
                    break;
                }
            }
        }

        current_agent->targeted_companies[current_agent->targeted_companies_count] = companies_coordinates[random_company];
        current_agent->targeted_companies_count++;

        // il rentre chez lui

        while (!is_at_home(current_agent->character)) {
            if (signal_received_spies[args->id]) {
                move_source_agent(args, current_agent->character.home_row,
                                  current_agent->character.home_column);
                signal_received_spies[args->id] = 0;
                if (current_agent->character.health <= 0) {
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
    source_agent_t *current_agent = &(args->mem->source_agents[args->id]);
    current_agent->character.pid = pid;
    //agent_mapping(current_agent, args->id); // Modifiez cette fonction pour utiliser le TID


    while (!is_at_home(current_agent->character)) {
        if (signal_received_spies[args->id]) {
            move_source_agent(args, current_agent->character.home_row,
                              current_agent->character.home_column);
            signal_received_spies[args->id] = 0;
            if (current_agent->character.health <= 0) {
                break;
            }
        }
    }

    sleep(1);
    pthread_exit(NULL);
}

void *evening_attending_officer(void *arg) {
    agent_thread_args_t *args = (agent_thread_args_t *) arg;
    attending_officer_t *attendingOfficer = &(args->mem->attending_officers[args->id]);
    // va à un supermarché
    int random_supermarket = rand() % NUMBER_OF_SUPERMARKETS;
    coordinate_t *supermarket_coordinates = findTypeOfBuilding(&args->mem->city_map, SUPERMARKET,
                                                               NUMBER_OF_SUPERMARKETS);


    while (!character_is_at(attendingOfficer->character,
                            supermarket_coordinates[random_supermarket])) {
        if (signal_received_spies[args->id]) {
            move_source_agent(args, supermarket_coordinates[random_supermarket].row,
                              supermarket_coordinates[random_supermarket].column);
            signal_received_spies[args->id] = 0;
            if (attendingOfficer->character.health <= 0) {
                break;
            }
        }
    }



    while (!is_at_home(attendingOfficer->character)) {
        if (signal_received_officer) {
            move_attending_officer(args, attendingOfficer->character.home_row,
                                   attendingOfficer->character.home_column);
            signal_received_officer = 0;
            if (attendingOfficer->character.health <= 0) {
                break;
            }
        }
    }

    sleep(1);
    pthread_exit(NULL);
}

void *morning_attending_officer(void *arg) {
    agent_thread_args_t *args = (agent_thread_args_t *) arg;
    attending_officer_t *attendingOfficer = &(args->mem->attending_officers[args->id]);


    // va à la mailbox
    while (!character_is_at(attendingOfficer->character, args->mem->mailbox_coordinate)) {
        if (signal_received_officer) {
            move_attending_officer(args, args->mem->mailbox_coordinate.row, args->mem->mailbox_coordinate.column);
            signal_received_officer = 0;
            if (attendingOfficer->character.health <= 0) {
                break;
            }
        }

    }

    // récupère les messages
    pickup_messages(args->mem);

    // rentre chez lui
    while (!is_at_home(attendingOfficer->character)) {
        if (signal_received_officer) {
            move_attending_officer(args, attendingOfficer->character.home_row,
                                   attendingOfficer->character.home_column);
            signal_received_officer = 0;
            if (attendingOfficer->character.health <= 0) {
                break;
            }
        }
    }

    sleep(1);
    pthread_exit(NULL);
}


void pickup_messages(memory_t *mem) {
    // parcourir la mailobx, et s'il y a des nouveaux messages les décrypter et les stocker dans decrypted_messages
    for (int i = 0; i < mem->mailbox_size; ++i) {
        if (mem->encrpyted_messages[i].is_encrypted == 0) {
            char *message = malloc(sizeof(char) * MAX_LENGTH_OF_MESSAGE);
            strcpy(message, mem->encrpyted_messages[i].msg_text);
            decrpyt_message(message, 3);
            strcpy(mem->decrypted_messages[mem->decrypted_mailbox_size].msg_text, message);
            mem->decrypted_mailbox_size++;
            mem->encrpyted_messages[i].is_encrypted = 1;
        }
    }
}

void *attempt_information_theft(void *arg) {
    agent_thread_args_t *args = (agent_thread_args_t *) arg;

    int pid = getpid();
    source_agent_t *current_agent = &(args->mem->source_agents[args->id]);
    current_agent->character.pid = pid;
    // agent_mapping(current_agent, args->id); // Modifiez cette fonction pour utiliser le TID


    // go near a company
    int random_company = rand() % current_agent->targeted_companies_count;
    coordinate_t company = current_agent->targeted_companies[random_company];
    int * neighbour_cells_count = malloc(sizeof(int));
    coordinate_t* neighbour_cells = findNeighbouringCells(&args->mem->city_map, company.row, company.column, neighbour_cells_count);
    int random_neighbour_cell = rand() % *neighbour_cells_count;

    int turns = 0;
    int type = 0; // 0 = fake, 1 = real
    InformationCruciality priority = rand() % CRUCIALITY_LEVELS;



    while (!character_is_at(current_agent->character, neighbour_cells[random_neighbour_cell])) {
        if (signal_received_spies[args->id]) {
            move_source_agent(args, neighbour_cells[random_neighbour_cell].row,
                              neighbour_cells[random_neighbour_cell].column);
            signal_received_spies[args->id] = 0;
            if (current_agent->character.health <= 0) {
                break;
            }
        }
    }

    // 12 tours de repérage pour le vol
    while (turns < 12) {
        if (signal_received_spies[args->id]) {
            random_neighbour_cell = rand() % *neighbour_cells_count;
            move_source_agent(args, neighbour_cells[random_neighbour_cell].row,
                              neighbour_cells[random_neighbour_cell].column);
            signal_received_spies[args->id] = 0;
            if (current_agent->character.health <= 0) {
                break;
            }
            turns++;
        }

    }

    int thief_is_possible = rand() % 100;

    if (thief_is_possible < 85) {
        // vole l'information
        while (turns < 18) {
            if (signal_received_spies[args->id]) {
                move_source_agent(args, company.row, company.column);
                if (current_agent->character.health <= 0) {
                    break;
                }
                turns++;
            }
        }
        thief_is_possible = rand() % 100;
        if(thief_is_possible < 90){
            priority = accomplish_mission(args->mem, company);
            type = 1;
            sem_wait(move_sem);
            current_agent->nb_of_stolen_companies++;
            sem_post(move_sem);
        }



    } else {
        // ne vole pas l'information
        // envoie un faux message dans la mailbox
    }

    // @TODO: s'il y a déjà quelqu'un à la mailbox attendre qu'il parte, sinon revenir le lendemain (P1)
    while (!character_is_at(current_agent->character, args->mem->mailbox_coordinate)) {
        if (signal_received_spies[args->id]) {
            move_source_agent(args, args->mem->mailbox_coordinate.row, args->mem->mailbox_coordinate.column);
            if (current_agent->character.health <= 0) {
                break;
            }
        }
    }
    // @TODO: crypter le message (fonction de tools.c) (P0)
    post_message(priority, type);
    current_agent->targeted_companies_count = 0;
    current_agent->has_stolen_a_company_tonight = 1;


    // rentre chez lui
    while (!is_at_home(current_agent->character)) {
        if (signal_received_spies[args->id]) {
            move_source_agent(args, current_agent->character.home_row,
                              current_agent->character.home_column);
            signal_received_spies[args->id] = 0;
            if (current_agent->character.health <= 0) {
                break;
            }
        }
    }

    pthread_exit(NULL);
}


void post_message(InformationCruciality priority, int type) {
    mqd_t mq = open_message_queue();

    sem_t* semaphore_message = open_semaphore_message();

    char *mess = malloc(sizeof(char) * MAX_LENGTH_OF_MESSAGE);

    if (type == 1) {
        MessageBank messageBank = setMessageBank();
        mess = generateSpyMessage(&(messageBank), priority);
    } else {

        strcpy(mess, FAKE_MESSAGE);
    }

    sem_wait(semaphore_message);
    if (mq_send(mq, mess, strlen(mess) + 1, 1) == -1) {
        perror("mq_send");
        exit(EXIT_FAILURE);
    }
    sem_post(semaphore_message);

    free(mess);

}

InformationCruciality accomplish_mission(memory_t *mem, coordinate_t company) {
    InformationCruciality cruciality = select_crucial_information();
    for (int i = 0; i < NUMBER_OF_COMPANIES; ++i) {
        if (mem->companies_priority[i].row == company.row && mem->companies_priority[i].column == company.column) {
            while(mem->companies_priority[i].cruciality.infoCount[cruciality] <= 0){
                cruciality = select_crucial_information();
            }
            mem->companies_priority[i].cruciality.infoCount[cruciality]--;
        }
    }
    return cruciality;

}

int getMessagePriority(InformationCruciality cruciality) {
    switch (cruciality) {
        case Crucial:
            return 10;
        case Strong:
            return 9;
        case Medium:
            return 6;
        case Low:
            return 3;
        case VeryLow:
            return 2;
        default:
            return 0; // En cas de valeur non reconnue
    }
}

InformationCruciality select_crucial_information(void) {
    int random_cruciality = rand() % 100;
    if (random_cruciality < 1) {
        return Crucial;
    } else if (random_cruciality < 6) {
        return Strong;
    } else if (random_cruciality < 20) {
        return Medium;
    } else if (random_cruciality < 50) {
        return Low;
    } else {
        return VeryLow;
    }
}



void initialize_attending_officer_routine(void) {
    int random_hour = rand() % 8;
    int random_minute = rand() % 60;
    for (int i = 0; i < 8; ++i) {
        if (i % 2 == 1) {
            int random_minute = rand() % 60;
            attending_officer_routine[i] = random_minute;
        }
        random_hour = rand() % 9;
        attending_officer_routine[0] = random_hour + 8;
        random_hour = rand() % 9;
        attending_officer_routine[2] = random_hour + 8;
        random_hour = rand() % 2;
        attending_officer_routine[4] = random_hour + 17;
        random_hour = rand() % 2;
        attending_officer_routine[6] = random_hour + 22;
    }
    for(int i = 0; i < 8; i++){
        printf("%d ", attending_officer_routine[i]);
    }

}



void create_network_morning_thread(memory_t *mem, all_threads_t *threads) {
    pthread_attr_t attr;
    agent_thread_args_t *ptr;
    agent_thread_args_t *ptr2;



    if (mem->my_timer.hours >= 8 && mem->my_timer.hours <= 17) {
        if(mem->my_timer.hours == 8 && mem->my_timer.minutes == 0){
            initialize_attending_officer_routine();
        }
        if(mem->my_timer.minutes == 0) {
            for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; ++i) {
                if (mem->source_agents[i].character.health <= 0) {
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
        }

        // @TODO : fix le problème que l'officier va tt le temps à la mailbox et pas uniquement 2 fois par jour (P4)
        if((mem->my_timer.hours == attending_officer_routine[0] && mem->my_timer.minutes == attending_officer_routine[1]) || (mem->my_timer.hours == attending_officer_routine[2] && mem->my_timer.minutes == attending_officer_routine[3])) {
            if (mem->attending_officers[0].character.health <= 0) {
                ptr2 = &threads->attending_officer_args[0];
                pthread_attr_init(&attr);
                if (pthread_create(&threads->attending_officer_threads[0], &attr, morning_attending_officer,
                                   ptr2) == 0) {
                } else {
                    printf("thread not created\n");
                }
            }
        }
        if (mem->attending_officers[0].character.health > 0) {
            ptr2 = &threads->attending_officer_args[0];
            pthread_attr_init(&attr);
            if (pthread_create(&threads->attending_officer_threads[0], &attr, morning_attending_officer,
                               ptr2) == 0) {
            } else {
                printf("thread not created\n");
            }
        }
        // joindre les threads

        if (mem->my_timer.minutes == 0) {
            for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; ++i) {
                if (mem->source_agents[i].character.health <= 0) {
                    continue;
                }
                pthread_join(threads->source_agent_threads[i], NULL);
            }
        }
        if((mem->my_timer.hours == attending_officer_routine[0] && mem->my_timer.minutes == attending_officer_routine[1]) || (mem->my_timer.hours == attending_officer_routine[2] && mem->my_timer.minutes == attending_officer_routine[3])) {
            if (mem->attending_officers[0].character.health > 0) {
                pthread_join(threads->attending_officer_threads[0], NULL);
            }
        }
    }

}

void create_network_evening_thread(memory_t *mem, all_threads_t *threads) {
    pthread_attr_t attr;
    agent_thread_args_t *ptr;
    agent_thread_args_t *ptr2;

    if (mem->my_timer.hours == 17 && mem->my_timer.minutes == 0) {

        for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; ++i) {
            if (mem->source_agents[i].character.health <= 0) {
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

        if(mem->my_timer.hours == attending_officer_routine[4] && mem->my_timer.minutes == attending_officer_routine[5]) {
            if (mem->attending_officers[0].character.health <= 0) {
                ptr2 = &threads->attending_officer_args[0];
                pthread_attr_init(&attr);
                if (pthread_create(&threads->attending_officer_threads[0], &attr, evening_attending_officer,
                                   ptr2) == 0) {
                } else {
                    printf("thread not created\n");
                }
            }
        }
        // joindre les threads

        for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; ++i) {
            if (mem->source_agents[i].character.health <= 0) {
                continue;
            }
            pthread_join(threads->source_agent_threads[i], NULL);
        }
        if(mem->my_timer.hours == attending_officer_routine[4] && mem->my_timer.minutes == attending_officer_routine[5]) {
            if (mem->attending_officers[0].character.health > 0) {
                pthread_join(threads->attending_officer_threads[0], NULL);
            }
        }
    }
}


void create_network_night_thread(memory_t *mem, all_threads_t *threads) {
    pthread_attr_t attr;
    agent_thread_args_t *ptr;
    agent_thread_args_t *ptr2;

    // @TODO : créer les threads avec une probabilité en fonction de l'heure qu'il est (P2)
    if ((mem->my_timer.hours >= 18 || mem->my_timer.hours <= 8) && mem->my_timer.minutes == 0 ) {

        for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; ++i) {
            if (mem->source_agents[i].character.health <= 0) {
                continue;
            }
            if (mem->source_agents[i].targeted_companies_count == 0) {
                continue;
            }
            ptr = &threads->source_agent_args[i];
            pthread_attr_init(&attr);
            if (pthread_create(&threads->source_agent_threads[i], &attr, attempt_information_theft,
                               ptr) == 0) {
            } else {
                printf("thread not created\n");
            }
            sleep(1);

        }

        if(mem->my_timer.hours == attending_officer_routine[6] && mem->my_timer.minutes == attending_officer_routine[7]) {
            if (mem->attending_officers[0].character.health <= 0) {
                ptr2 = &threads->attending_officer_args[0];
                pthread_attr_init(&attr);
                if (pthread_create(&threads->attending_officer_threads[0], &attr, evening_attending_officer,
                                   ptr2) == 0) {
                } else {
                    printf("thread not created\n");
                }
            }
        }
        // joindre les threads

        for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; ++i) {
            if (mem->source_agents[i].character.health <= 0) {
                continue;
            }
            pthread_join(threads->source_agent_threads[i], NULL);
        }
        if(mem->my_timer.hours == attending_officer_routine[6] && mem->my_timer.minutes == attending_officer_routine[7]) {
            if (mem->attending_officers[0].character.health > 0) {
                pthread_join(threads->attending_officer_threads[0], NULL);
            }
        }
    }
}


void create_enemy_spy_thread(memory_t *mem) {
    all_threads_t *threads;
    threads = malloc(sizeof(all_threads_t));
    srand(time(NULL));

    for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; i++) {
        threads->source_agent_args[i].id = i;
        threads->source_agent_args[i].mem = mem;

        agent_mapping(&(mem->source_agents[i]), i);

    }
    for (int i = 0; i < MAX_ATTENDING_OFFICER_COUNT; i++) {
        threads->attending_officer_args[i].id = i;
        threads->attending_officer_args[i].mem = mem;

    }


    while (mem->simulation_has_ended == 0) {
        // @TODO : créer une fonction qui initialise bcp de choses lorsqu'on change de jour (P4)
        // @TODO : voir pour une autre approche dans laquelle on créée une seule fois les threads et on met la logique dedans (grosse optimisation) (P5)
        create_network_morning_thread(mem, threads);
        create_network_evening_thread(mem, threads);
        create_network_night_thread(mem, threads);
    }
}





