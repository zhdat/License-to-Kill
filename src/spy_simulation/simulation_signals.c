#include "simulation_signals.h"
#include "debug.h"

static memory_t *_mem;

void set_memory(memory_t *mem) {
    _mem = mem;
}
