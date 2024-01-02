#ifndef TIMER_H
#define TIMER_H

#include <signal.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "memory.h"

void set_memory(memory_t* memory_start);
void set_pids(int argc, char** argv);
int simulationFinished(memory_t* memory);
void timer_handler(int signum);
void my_set_timer(float interval);
void free_timer();

#endif //TIMER_H
