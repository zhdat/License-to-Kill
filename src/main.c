#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include "logger.h"


#define NUMBER_OF_PROGRAMS 6
#define NUMBER_OF_ARGS_TIMER (NUMBER_OF_PROGRAMS + 2 + 1)

/**
 * \file main.c
 *
 * \brief The main file of the spy simulation.
 *
 * This file is the main file of the spy simulation. It creates the processes
 * for the simulation and waits for them to finish.
 */

void handle_fatal_error_and_exit(const char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

pid_t create_child(void) {

    pid_t pid = fork();

    if (pid == 1) {
        handle_fatal_error_and_exit("Error [fork()]: ");
    }

    return pid;
}

void wait_children(int nb_children) {
    int i = 0;
    while (i < nb_children) {
        wait(NULL);
        i++;
    }
}

char** args_for_timer(pid_t* array_pid) {
    int i, count_program;
    char** array_args = (char**) malloc((NUMBER_OF_ARGS_TIMER) * sizeof(char*));

    for (i = 0; i < NUMBER_OF_ARGS_TIMER; i++) {
        array_args[i] = (char*) malloc(15 * sizeof(char));
    }

    array_args[0] = "timer";
    array_args[1] = "1";
    array_args[NUMBER_OF_ARGS_TIMER - 1] = NULL;
    i = 2;
    for (count_program = 0; count_program < NUMBER_OF_PROGRAMS; count_program++) {
        sprintf(array_args[i], "%d", array_pid[count_program]);
        i++;
    }

    return array_args;
}

void free_args_for_timer(char** array_args) {
    for (int i = 0; i < NUMBER_OF_PROGRAMS; i++) {
        free(array_args[i]);
    }
    free(array_args);
}

int main(void) {
    pid_t* pids = (pid_t*) malloc(sizeof(pid_t) * (NUMBER_OF_PROGRAMS));
    pid_t pid_timer;
    pids[1] = create_child();
    char** array_args_timer = NULL;

    if (pids[1] == 0) {
        execlp("./bin/spy_simulation", "spy_simulation", NULL);
    } else {
        sleep(1);
        pids[0] = create_child();

        if (pids[0] == 0) {
            execlp("./bin/monitor", "monitor", NULL);
        } else {
            pids[2] = create_child();

            if (pids[2] == 0) {
                execlp("./bin/enemy_spy_network", "enemy_spy_network", NULL);
            } else {
                pids[3] = create_child();
                if (pids[3] == 0) {
                    execlp("./bin/enemy_country", "enemy_country", NULL);
                } else {
                    pids[4] = create_child();
                    if (pids[4] == 0) {
                        execlp("./bin/citizen_manager", "citizen_manager", NULL);
                    } else {
                        pids[5] = create_child();
                        if (pids[5] == 0) {
                            execlp("./bin/counterintelligence_officer", "counterintelligence_officer", NULL);
                        } else {
                            pid_timer = create_child();

                            if (pid_timer == 0) {
                                array_args_timer = args_for_timer(pids);
                                execvp("./bin/timer", array_args_timer);
                            } else {
                                wait_children(NUMBER_OF_PROGRAMS + 1);
                                if (array_args_timer != NULL) {
                                    free_args_for_timer(array_args_timer);
                                }

                                free(pids);
                            }
                        }

                    }
                }
            }
        }
    }
}
