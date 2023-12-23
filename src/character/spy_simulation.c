#include "spy_simulation.h"
#include "character_factory.h"
#include "cell.h"
#include "common.h"
#include "memory.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>


static int _fd = -1;

memory_t* create_shared_memory(void) {
    memory_t *mem;

    _fd = shm_open ( SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0644) ;

    if (_fd == -1) {
        printf("Error cannot open shared memory \n");
        exit(EXIT_FAILURE);
    }

    if ( ftruncate ( _fd , sizeof( memory_t )) == -1) {
        printf("Error ftruncate \n");
        exit(EXIT_FAILURE);
    }

    mem = (memory_t *) mmap (NULL, sizeof(memory_t), PROT_READ | PROT_WRITE, MAP_SHARED , _fd , 0);

    return mem;
}

void set_spies(memory_t* mem){
    source_agent_factory_t *spy_factory_has_license, *spy_factory_has_not_license;
    source_agent_t spy;

    spy_factory_has_license = new_source_agent_factory(new_spy_with_licence);
    spy_factory_has_not_license = new_source_agent_factory(new_spy_without_licence);

    cell_t choosen_residence;

    for(int i = 0; i < MAX_SOURCE_AGENT_COUNT ; i++){
        if(i == 0){

            spy = spy_factory_has_license->factory_method(1, 1);
        }else{
            spy = spy_factory_has_not_license->factory_method(1, 1);
        }
        mem->source_agents[i] = spy;
    }


}

void set_citizens(memory_t* mem){
    character_factory_t *citizen_factory;
    character_t citizen;

    citizen_factory = new_character_factory(new_citizen);

    cell_t choosen_residence;

    for(int i = 0; i < MAX_CITIZEN_COUNT ; i++){
        citizen = citizen_factory->factory_method(1, 1);
        mem->citizens[i] = citizen;
    }
}

void set_attending_officers(memory_t* mem){
    attending_officer_factory_t *attending_officer_factory;
    attending_officer_t attending_officer;

    attending_officer_factory = new_attending_officer_factory(new_case_officer);

    cell_t choosen_residence;

    for(int i = 0; i < MAX_ATTENDING_OFFICER_COUNT ; i++){
        attending_officer = attending_officer_factory->factory_method(1, 1);
        mem->attending_officers[i] = attending_officer;
    }
}

void set_counter_intelligence_officers(memory_t* mem){
    counter_intelligence_officer_factory_t *counterintelligence_officer_factory;
    counter_intelligence_officer_t counter_intelligence_officer;

    counterintelligence_officer_factory = new_counter_intelligence_officer_factory(new_counter_intelligence_officer);

    cell_t choosen_residence;

    for(int i = 0; i < MAX_COUNTER_INTELLIGENCE_OFFICER_COUNT ; i++){
        counter_intelligence_officer = counterintelligence_officer_factory->factory_method(1,1, 10);
        mem->counter_intelligence_officers[i] = counter_intelligence_officer;
    }
}

void set_characters(memory_t* mem){
    set_spies(mem);
    set_citizens(mem);
    set_attending_officers(mem);
    set_counter_intelligence_officers(mem);
}

void set_content_memory(memory_t* mem) {
    mem->memory_has_changed = 1;
    mem->simulation_has_ended = 0;
    set_characters(mem);
    clear_city(&(mem->city_map));
    print_city(&(mem->city_map));
    init_city(&(mem->city_map));
    print_city(&(mem->city_map));

    mem->step = 100;
}
