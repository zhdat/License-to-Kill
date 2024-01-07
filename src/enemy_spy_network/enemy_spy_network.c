#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "enemy_spy_network.h"
#include "debug.h"

/**
 * \file enemy_spy_network.c
 * \brief Defines functions for initializing and managing the enemy spy network.
 */

volatile int signal_received_spies[MAX_SOURCE_AGENT_COUNT] = {0, 0, 0};
volatile int signal_received_officer = 0;
source_agent_t* agent_map[MAX_SOURCE_AGENT_COUNT];
int attending_officer_routine[4] = {0, 0, 0, 0};

sem_t* move_sem;

// @TODO: faire une map de chaques entreprise volée, par quel agent, et quand (P3)

void set_semaphore(sem_t* sem) {
    move_sem = sem;
}

void handle_signal(int sig) {
    sig = sig;
    for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; i++) {
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

static int getMessagePriority(InformationCruciality cruciality) {
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
            return 0;
    }
}

static InformationCruciality select_crucial_information(void) {
    int random_cruciality = selectRandomNumberUnder(100);
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

void agent_mapping(source_agent_t* agent, int id) {
    agent_map[id] = agent;
}

void handle_sigusr1(int sig, siginfo_t* info, void* unused) {
    sig = sig;
    info = info;
    unused = unused;
    for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; i++) {
        if (agent_map[i]->is_attacked == 1) {
            source_agent_t* agent = agent_map[i];
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

void handle_sigusr2(int sig, siginfo_t* info, void* unused) {
    sig = sig;
    info = info;
    unused = unused;
    for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; i++) {
        if (agent_map[i]->is_attacked == 1) {
            source_agent_t* agent = agent_map[i];
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

void move_source_agent(agent_thread_args_t* arg, int row, int column) {
    if (arg == NULL) {
        return;
    }
    int start_row, start_column;
    memory_t* mem = arg->mem;
    source_agent_t* spies = &(mem->source_agents[arg->id]);

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

    if (sem_wait(move_sem) == -1) {
        perror("sem_wait");
    }
    decrements_population_in_cell(mem, start_column, start_row);
    next_move(&(mem->city_map), start_cell, end_cell, &spies->character.column, &spies->character.row,
              spies->character);
    increments_population_in_cell(mem, spies->character.column, spies->character.row);
    if (sem_post(move_sem) == -1) {
        perror("sem_post");
    }
}

void move_attending_officer(agent_thread_args_t* arg, int row, int column) {
    if (arg == NULL) {
        return;
    }
    int start_row, start_column;
    memory_t* mem = arg->mem;
    attending_officer_t* officer = &(mem->attending_officers[arg->id]);

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

    if (sem_wait(move_sem) == -1) {
        perror("sem_wait");
    }
    decrements_population_in_cell(mem, start_column, start_row);
    next_move(&(mem->city_map), start_cell, end_cell, &officer->character.column, &officer->character.row,
              officer->character);
    increments_population_in_cell(mem, officer->character.column, officer->character.row);
    if (sem_post(move_sem) == -1) {
        perror("sem_post");
    }
}

void* morning_source_agent(void* arg) {
    agent_thread_args_t* args = (agent_thread_args_t*) arg;
    int pid = getpid();
    source_agent_t* current_agent = &(args->mem->source_agents[args->id]);
    current_agent->character.pid = pid;

    int random_activity = selectRandomNumberUnder(100);

    if (random_activity < 10) {
        /*!< go to a supermarket */
        int random_supermarket = selectRandomNumberUnder(NUMBER_OF_SUPERMARKETS);
        coordinate_t* supermarket_coordinates = findTypeOfBuilding(&args->mem->city_map, SUPERMARKET,
                                                                   NUMBER_OF_SUPERMARKETS);

        while (!character_is_at(current_agent->character,
                                supermarket_coordinates[random_supermarket])) {
            if (signal_received_spies[args->id]) {
                signal_received_spies[args->id] = 0;
                move_source_agent(args, supermarket_coordinates[random_supermarket].row,
                                  supermarket_coordinates[random_supermarket].column);
                if (current_agent->character.health <= 0) {
                    break;
                }
            }
        }

        /*!< go back home */
        while (!is_at_home(current_agent->character)) {
            if (signal_received_spies[args->id]) {
                signal_received_spies[args->id] = 0;
                move_source_agent(args, current_agent->character.home_row,
                                  current_agent->character.home_column);
                if (current_agent->character.health <= 0) {
                    break;
                }
            }
        }

    } else if (random_activity >= 10 && random_activity < 40) {
        /*!< stay at home */
    } else {

        int random_company = selectRandomNumberUnder(NUMBER_OF_COMPANIES);
        coordinate_t* companies_coordinates = findTypeOfBuilding(&args->mem->city_map, COMPANY, NUMBER_OF_COMPANIES);


        while (!character_is_at(current_agent->character, companies_coordinates[random_company])) {
            if (signal_received_spies[args->id]) {
                signal_received_spies[args->id] = 0;
                move_source_agent(args, companies_coordinates[random_company].row,
                                  companies_coordinates[random_company].column);
                if (current_agent->character.health <= 0) {
                    break;
                }
            }
        }

        current_agent->targeted_companies[current_agent->targeted_companies_count] = companies_coordinates[random_company];
        current_agent->targeted_companies_count++;

        /*!< go back home */

        while (!is_at_home(current_agent->character)) {
            if (signal_received_spies[args->id]) {
                signal_received_spies[args->id] = 0;
                move_source_agent(args, current_agent->character.home_row,
                                  current_agent->character.home_column);
                if (current_agent->character.health <= 0) {
                    break;
                }
            }
        }
    }

    sleep(1);
    pthread_exit(NULL);
}

void* evening_source_agent(void* arg) {
    agent_thread_args_t* args = (agent_thread_args_t*) arg;

    int pid = getpid();
    source_agent_t* current_agent = &(args->mem->source_agents[args->id]);
    current_agent->character.pid = pid;

    while (!is_at_home(current_agent->character)) {
        if (signal_received_spies[args->id]) {
            signal_received_spies[args->id] = 0;
            move_source_agent(args, current_agent->character.home_row,
                              current_agent->character.home_column);
            if (current_agent->character.health <= 0) {
                break;
            }
        }
    }

    sleep(1);
    pthread_exit(NULL);
}

void* evening_attending_officer(void* arg) {
    agent_thread_args_t* args = (agent_thread_args_t*) arg;
    attending_officer_t* attendingOfficer = &(args->mem->attending_officers[args->id]);

    int random_supermarket = selectRandomNumberUnder(NUMBER_OF_SUPERMARKETS);
    coordinate_t* supermarket_coordinates = findTypeOfBuilding(&args->mem->city_map, SUPERMARKET,
                                                               NUMBER_OF_SUPERMARKETS);


    while (!character_is_at(attendingOfficer->character, supermarket_coordinates[random_supermarket])) {
        if (signal_received_officer) {
            signal_received_officer = 0;
            move_attending_officer(args, supermarket_coordinates[random_supermarket].row,
                                   supermarket_coordinates[random_supermarket].column);
            if (attendingOfficer->character.health <= 0) {
                break;
            }
        }
    }

    while (!is_at_home(attendingOfficer->character)) {
        if (signal_received_officer) {
            signal_received_officer = 0;
            move_attending_officer(args, attendingOfficer->character.home_row,
                                   attendingOfficer->character.home_column);
            if (attendingOfficer->character.health <= 0) {
                break;
            }
        }
    }

    sleep(1);
    pthread_exit(NULL);
}

void* morning_attending_officer(void* arg) {
    agent_thread_args_t* args = (agent_thread_args_t*) arg;
    attending_officer_t* attendingOfficer = &(args->mem->attending_officers[args->id]);


    /*!< go to mailbox */
    while (!character_is_at(attendingOfficer->character, args->mem->mailbox_coordinate)) {
        if (signal_received_officer) {
            signal_received_officer = 0;
            move_attending_officer(args, args->mem->mailbox_coordinate.row, args->mem->mailbox_coordinate.column);
            if (attendingOfficer->character.health <= 0) {
                break;
            }
        }
    }

    /*!< get messages */
    attendingOfficer->have_messages = 1;

    /*!< go back home */
    while (!is_at_home(attendingOfficer->character)) {
        if (signal_received_officer) {
            signal_received_officer = 0;
            move_attending_officer(args, attendingOfficer->character.home_row,
                                   attendingOfficer->character.home_column);
            if (attendingOfficer->character.health <= 0) {
                break;
            }
        }
    }

    sleep(1);
    pthread_exit(NULL);
}

void* night_attending_officer(void* arg) {
    agent_thread_args_t* args = (agent_thread_args_t*) arg;
    attending_officer_t* attendingOfficer = &(args->mem->attending_officers[args->id]);

    if (attendingOfficer->have_messages == 0) {
        pthread_exit(NULL);
    }
    /*!< send message */
    pickup_messages(args->mem);

    attendingOfficer->have_messages = 0;

    sleep(1);
    pthread_exit(NULL);
}


void pickup_messages(memory_t* mem) {
    for (int i = 0; i < mem->mailbox_size; i++) {
        if (mem->encrpyted_messages[i].is_encrypted == 0) {
            char* message = malloc(sizeof(char) * MAX_LENGTH_OF_MESSAGE);
            strcpy(message, mem->encrpyted_messages[i].msg_text);
            decrpyt_message(message, 3);
            strcpy(mem->decrypted_messages[mem->decrypted_mailbox_size].msg_text, message);
            mem->decrypted_mailbox_size++;
            mem->encrpyted_messages[i].is_encrypted = 1;
        }
    }
}

void* attempt_information_theft(void* arg) {
    agent_thread_args_t* args = (agent_thread_args_t*) arg;

    int pid = getpid();
    source_agent_t* current_agent = &(args->mem->source_agents[args->id]);
    current_agent->character.pid = pid;


    /*!< go near company */
    int random_company = selectRandomNumberUnder(current_agent->targeted_companies_count);
    coordinate_t company = current_agent->targeted_companies[random_company];
    int* neighbour_cells_count = malloc(sizeof(int));
    coordinate_t* neighbour_cells = findNeighbouringCells(&args->mem->city_map, company.row, company.column,
                                                          neighbour_cells_count);
    int random_neighbour_cell = selectRandomNumberUnder(*neighbour_cells_count);

    int turns = 0;
    int type_of_message = 0; // 0 = fake, 1 = real
    InformationCruciality priority = selectRandomNumberUnder(CRUCIALITY_LEVELS);

    while (!character_is_at(current_agent->character, neighbour_cells[random_neighbour_cell])) {
        if (signal_received_spies[args->id]) {
            signal_received_spies[args->id] = 0;
            move_source_agent(args, neighbour_cells[random_neighbour_cell].row,
                              neighbour_cells[random_neighbour_cell].column);
            if (current_agent->character.health <= 0) {
                break;
            }
        }
    }

    while (turns < 12) {
        if (signal_received_spies[args->id]) {
            signal_received_spies[args->id] = 0;
            random_neighbour_cell = selectRandomNumberUnder(*neighbour_cells_count);
            move_source_agent(args, neighbour_cells[random_neighbour_cell].row,
                              neighbour_cells[random_neighbour_cell].column);
            if (current_agent->character.health <= 0) {
                break;
            }
            turns++;
        }
    }

    int thief_is_possible = selectRandomNumberUnder(100);

    if (thief_is_possible < 85) {
        while (turns < 18) {
            if (signal_received_spies[args->id]) {
                signal_received_spies[args->id] = 0;
                move_source_agent(args, company.row, company.column);
                if (current_agent->character.health <= 0) {
                    break;
                }
                turns++;
            }
        }
        thief_is_possible = selectRandomNumberUnder(100);
        if (thief_is_possible < 90) {
            if (current_agent->character.health <= 0) {
                pthread_exit(NULL);
            }
            priority = accomplish_mission(args->mem, company);
            if (current_agent->character.health <= 0) {
                pthread_exit(NULL);
            }
            type_of_message = 1; // Real message
            current_agent->nb_of_stolen_companies++;
            strcpy(current_agent->stolen_message, "YES");
        }


    } else {
        // ne vole pas l'information
        // envoie un faux message dans la mailbox
    }

    // @TODO: s'il y a déjà quelqu'un à la mailbox attendre qu'il parte, sinon revenir le lendemain (P1)
    while (!character_is_at(current_agent->character, args->mem->mailbox_coordinate)) {
        if (signal_received_spies[args->id]) {
            signal_received_spies[args->id] = 0;
            move_source_agent(args, args->mem->mailbox_coordinate.row, args->mem->mailbox_coordinate.column);
            if (current_agent->character.health <= 0) {
                break;
            }
        }
    }

    if (current_agent->character.health <= 0) {
        pthread_exit(NULL);
    }

    if (current_agent->character.health > 0) {
        post_message(priority, type_of_message);
    }
    //post_message(priority, type);
    current_agent->targeted_companies_count = 0;
    strcpy(current_agent->stolen_message, EMPTY);

    while (!is_at_home(current_agent->character)) {
        if (signal_received_spies[args->id]) {
            signal_received_spies[args->id] = 0;
            move_source_agent(args, current_agent
                                      ->character.home_row,
                              current_agent->character.home_column);
            if (current_agent->character.health <= 0) {
                break;
            }
        }
    }

    free(neighbour_cells_count);
    pthread_exit(NULL);
}


void post_message(InformationCruciality priority, int type) {
    mqd_t mq = open_message_queue();

    sem_t* semaphore_message = open_semaphore_message();

    char mess[MAX_LENGTH_OF_MESSAGE];

    if (type == 1) {
        MessageBank messageBank = setMessageBank();
        strcpy(mess, generateSpyMessage(&(messageBank), priority));
    } else {

        strcpy(mess, FAKE_MESSAGE);
    }

    caesarCipher(mess, 3);
    char priority_char[10];
    sprintf(priority_char, " - %d", getMessagePriority(priority));
    strcat(mess, priority_char);

    sem_wait(semaphore_message);
    if (mq_send(mq, mess, strlen(mess) + 1, 1) == -1) {
        perror("mq_send\n");
    }

    sem_post(semaphore_message);

    mq_close(mq);
}

InformationCruciality accomplish_mission(memory_t* mem, coordinate_t company) {
    InformationCruciality cruciality = select_crucial_information();
    for (int i = 0; i < NUMBER_OF_COMPANIES; i++) {
        coordinate_t company_priority = {mem->companies_priority[i].row,
                                         mem->companies_priority[i].column};
        if (is_same_cell(company, company_priority)) {
            while (mem->companies_priority[i].cruciality.infoCount[cruciality] <= 0) {
                cruciality = select_crucial_information();
            }
            mem->companies_priority[i].cruciality.infoCount[cruciality]--;
        }
    }
    return cruciality;
}

void initialize_attending_officer_routine(void) {
    int random_hour;
    random_hour = rand() % 9;
    attending_officer_routine[0] = random_hour + 8;
    random_hour = rand() % 9;
    attending_officer_routine[1] = random_hour + 8;
    random_hour = rand() % 2;
    attending_officer_routine[2] = random_hour + 17;
    random_hour = rand() % 2;
    attending_officer_routine[3] = random_hour + 22;
    for (int i = 0; i < 4; i++) {
#if DEBUG
        printf("%d ", attending_officer_routine[i]);
#endif
    }
}


void create_network_morning_thread(memory_t* mem, all_threads_t* threads) {
    pthread_attr_t attr;
    agent_thread_args_t* ptr;
    agent_thread_args_t* ptr2;


    if (mem->timer.hours >= 8 && mem->timer.hours <= 17 && mem->timer.minutes == 0) {
        if (mem->timer.hours == 8) {
            initialize_attending_officer_routine();
        }
        for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; i++) {
            if (mem->source_agents[i].character.health <= 0) {
                continue;
            }
            ptr = &threads->source_agent_args[i];
            pthread_attr_init(&attr);
            if (pthread_create(&threads->source_agent_threads[i], &attr, morning_source_agent,
                               ptr) == 0) {
            } else {
#if DEBUG
                printf("thread not created\n");
#endif
            }
            sleep(1);
        }

        if ((mem->timer.hours == attending_officer_routine[0]) ||
            (mem->timer.hours == attending_officer_routine[1])) {
            if (mem->attending_officers[0].character.health > 0) {
                ptr2 = &threads->attending_officer_args[0];
                pthread_attr_init(&attr);
                if (pthread_create(&threads->attending_officer_threads[0], &attr, morning_attending_officer,
                                   ptr2) == 0) {
                } else {
#if DEBUG
                    printf("thread not created\n");
#endif
                }
            }
        }
        // joindre les threads

        for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; i++) {
            if (mem->source_agents[i].character.health <= 0) {
                continue;
            }
            pthread_join(threads->source_agent_threads[i], NULL);
        }

        if ((mem->timer.hours == attending_officer_routine[0]) ||
            (mem->timer.hours == attending_officer_routine[1])) {
            if (mem->attending_officers[0].character.health > 0) {
                pthread_join(threads->attending_officer_threads[0], NULL);
            }
        }
    }

}

void create_network_evening_thread(memory_t* mem, all_threads_t* threads) {
    pthread_attr_t attr;
    agent_thread_args_t* ptr;
    agent_thread_args_t* ptr2;

    if (mem->timer.hours == 17 && mem->timer.minutes == 0) {

        for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; i++) {
            if (mem->source_agents[i].character.health <= 0) {
                continue;
            }
            ptr = &threads->source_agent_args[i];
            pthread_attr_init(&attr);
            if (pthread_create(&threads->source_agent_threads[i], &attr, evening_source_agent,
                               ptr) == 0) {
            } else {
#if DEBUG
                printf("thread not created\n");
#endif
            }
            sleep(1);

        }

        if (mem->timer.hours == attending_officer_routine[2]) {
            if (mem->attending_officers[0].character.health > 0) {
                ptr2 = &threads->attending_officer_args[0];
                pthread_attr_init(&attr);
                if (pthread_create(&threads->attending_officer_threads[0], &attr, evening_attending_officer,
                                   ptr2) == 0) {
                } else {
#if DEBUG
                    printf("thread not created\n");
#endif
                }
            }
        }
        // joindre les threads

        for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; i++) {
            if (mem->source_agents[i].character.health <= 0) {
                continue;
            }
            pthread_join(threads->source_agent_threads[i], NULL);
        }

        if (mem->timer.hours == attending_officer_routine[2]) {
            if (mem->attending_officers[0].character.health > 0) {
                pthread_join(threads->attending_officer_threads[0], NULL);
            }
        }
    }
}


void create_network_night_thread(memory_t* mem, all_threads_t* threads) {
    pthread_attr_t attr;
    agent_thread_args_t* ptr;
    agent_thread_args_t* ptr2;

    // @TODO : créer les threads avec une probabilité en fonction de l'heure qu'il est (P2)
    if ((mem->timer.hours >= 18 || mem->timer.hours <= 8) && mem->timer.minutes == 0) {
        if (mem->timer.turns <= 20) {
            return;
        }

        for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; i++) {
#if DEBUG
            //log_info("creation des threads...");
#endif
            if (mem->source_agents[i].character.health <= 0) {
                continue;
            }
            if (mem->source_agents[i].targeted_companies_count == 0) {
#if DEBUG
                //log_info("pas de cible");
#endif
                continue;
            }
#if DEBUG
            //log_info("creation du thread %d...", i);
#endif
            ptr = &threads->source_agent_args[i];
            pthread_attr_init(&attr);
            if (pthread_create(&threads->source_agent_threads[i], &attr, attempt_information_theft,
                               ptr) == 0) {
            } else {
#if DEBUG
                printf("thread not created\n");
#endif
            }
            sleep(1);

        }

        if (mem->attending_officers[0].character.health > 0) {
            ptr2 = &threads->attending_officer_args[0];
            pthread_attr_init(&attr);
            if (pthread_create(&threads->attending_officer_threads[0], &attr, night_attending_officer,
                               ptr2) == 0) {
            } else {
#if DEBUG
                printf("thread not created\n");
#endif
            }
        }

        for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; i++) {
            if (mem->source_agents[i].character.health <= 0) {
                continue;
            }
            if (mem->source_agents[i].targeted_companies_count == 0) {
                continue;
            }
            pthread_join(threads->source_agent_threads[i], NULL);
        }

        if (mem->attending_officers[0].character.health > 0) {
            pthread_join(threads->attending_officer_threads[0], NULL);
        }
    }
}


void create_enemy_spy_thread(memory_t* mem) {
    all_threads_t* threads;
    threads = malloc(sizeof(all_threads_t));

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
        create_network_morning_thread(mem, threads);
        create_network_evening_thread(mem, threads);
        create_network_night_thread(mem, threads);
    }
}




