#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

#define NUMBER_OF_PROGRAMS 5
#define NUMBER_OF_ARGS_TIMER (NUMBER_OF_PROGRAMS + 2 + 1)

void handle_fatal_error_and_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

pid_t create_child(const char *program_name, char *const argv[]) {
    pid_t pid = fork();

    if (pid == -1) {
        handle_fatal_error_and_exit("Error [fork()]: ");
    } else if (pid == 0) {
        // Child process
        if (execvp(program_name, argv) == -1) {
            handle_fatal_error_and_exit("Error [execvp()]: ");
        }
        exit(EXIT_SUCCESS);
    }

    return pid;
}

void wait_children(int nb_children) {
    int status, i = 0;
    while (i < nb_children) {
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            printf("Child process exited with error status.\n");
        }
        i++;
    }
}

char** args_for_timer(pid_t* array_pid) {
    int i, count_program;
    char** array_args = (char **) malloc((NUMBER_OF_ARGS_TIMER) * sizeof(char *));

    for(i = 0; i < NUMBER_OF_ARGS_TIMER; i++) {
        array_args[i] = (char *) malloc(15 * sizeof(char));
    }

    array_args[0] = "timer";
    array_args[1] = "1";
    array_args[NUMBER_OF_ARGS_TIMER - 1] = NULL;
    i = 2;
    for(count_program = 0; count_program < NUMBER_OF_PROGRAMS; count_program++) {
        sprintf(array_args[i], "%d", array_pid[count_program]);
        i++;
    }

    return array_args;
}

int main() {
    pid_t child_pids[NUMBER_OF_PROGRAMS];

    // Example of launching each program with their specific arguments
    char *spy_simulation_args[] = {"spy_simulation", NULL};
    child_pids[0] = create_child("./bin/spy_simulation", spy_simulation_args);

    char *monitor_args[] = {"monitor", NULL};
    child_pids[1] = create_child("./bin/monitor", monitor_args);


    // char *timer_args[] = {"timer", child_pids, NULL};
    // child_pids[2] = create_child("./bin/timer", args_for_timer(child_pids));

    char *enemy_country_args[] = {"enemy_country", NULL};
    child_pids[3] = create_child("./bin/enemy_country", enemy_country_args);

    char *testing_args[] = {"testing", NULL};
    child_pids[4] = create_child("./bin/testing", testing_args);

    /*char *citizen_manager_args[] = {"citizen_manager", "arg1", NULL};
    child_pids[1] = create_child("citizen_manager", citizen_manager_args);*/

    // ... Similar code for other components ...

    wait_children(NUMBER_OF_PROGRAMS);

    printf("All child processes have completed.\n");
    return 0;
}

