#ifndef TP_SYS_SPY_SIMULATION_H
#define TP_SYS_SPY_SIMULATION_H

#include "memory.h"

memory_t* create_shared_memory(void);
void set_content_memory(memory_t* mem);
void set_spies(memory_t* mem);
void set_citizens(memory_t* mem);
void set_attending_officers(memory_t* mem);
void set_counter_intelligence_officers(memory_t* mem);
void set_characters(memory_t* mem);




#endif //TP_SYS_SPY_SIMULATION_H
