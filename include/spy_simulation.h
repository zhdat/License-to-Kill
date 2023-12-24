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
void set_city_map(memory_t* mem);
void set_mailbox(memory_t* mem);
coordinate_t get_mailbox(memory_t* mem);
coordinate_t get_residence_near_mailbox(memory_t* mem, int max_distance);
void increments_population_in_cell(memory_t* mem, int col, int row);
coordinate_t find_random_low_populated_residence(memory_t* mem);
int* print_where_citizens_work(memory_t* mem);




#endif //TP_SYS_SPY_SIMULATION_H
