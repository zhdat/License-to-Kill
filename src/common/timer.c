#define TOTAL_STEP 2016
#define SECONDS_DURATION 1
#define SHARED_MEMORY_NAME "/nothinghere"

void fin_tour(int signum);
memory_t *initialiser_timer();

int main() {
    struct itimerval timer;
    memory_t *shared_timer = initialiser_timer();

    signal(SIGALRM, fin_tour);

    // Configuration du timer pour envoyer SIGALRM toutes les SECONDS_DURATION
    timer.it_value.tv_sec = SECONDS_DURATION;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = SECONDS_DURATION;
    timer.it_interval.tv_usec = 0;

    // Démarrage du timer
    if (setitimer(ITIMER_REAL, &timer, NULL) == -1) {
        perror("Error while starting the timer");
        return EXIT_FAILURE;
    }

    while (shared_timer->step < TOTAL_STEP) {
        pause();
    }

    printf("End of the simulation.\n");

    munmap(shared_timer, sizeof(memory_t));
    return EXIT_SUCCESS;
}

void fin_tour(int signum) {
    static memory_t *shared_timer = NULL;
    if (shared_timer == NULL) {
        int fd = shm_open(SHARED_MEMORY_NAME, O_RDWR, 0600);
        shared_timer = (memory_t *)mmap(NULL, sizeof(memory_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        close(fd);
    }
    shared_timer->step++;

    shared_timer->minutes += 10;
    if (shared_timer->minutes >= 60) {
        shared_timer->minutes -= 60;
        shared_timer->hours++;
    }
    if (shared_timer->hours >= 24) {
        shared_timer->hours = 0;
        shared_timer->days++;
    }
}

memory_t *initialiser_timer() {
    int fd = shm_open(SHARED_MEMORY_NAME, O_RDWR, 0600);
    ftruncate(fd, sizeof(memory_t));
    memory_t *shared_timer = (memory_t *)mmap(NULL, sizeof(memory_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    shared_timer->total_steps = TOTAL_STEP;
    shared_timer->step = 0;
    return shared_timer;
}