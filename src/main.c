#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

#define NUMBER_OF_PROGRAMS 7

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

int main() {
    pid_t child_pids[NUMBER_OF_PROGRAMS];

    // Example of launching each program with their specific arguments
    char *spy_simulation_args[] = {"spy_simulation", NULL, NULL, NULL};
    child_pids[0] = create_child("./bin/spy_simulation", spy_simulation_args);

    char *monitor_args[] = {"monitor", NULL, NULL};
    child_pids[1] = create_child("./bin/monitor", monitor_args);


    /*char *citizen_manager_args[] = {"citizen_manager", "arg1", NULL};
    child_pids[1] = create_child("citizen_manager", citizen_manager_args);*/

    // ... Similar code for other components ...

    wait_children(NUMBER_OF_PROGRAMS);

    printf("All child processes have completed.\n");
    return 0;
}

