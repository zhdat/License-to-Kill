#include "../../include/simulation_signals.h"
#include <stdio.h>
#include <stdlib.h>

static memory_t * _mem;

void set_memory(memory_t *mem){
    _mem = mem;
}