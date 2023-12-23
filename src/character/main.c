#include <stdio.h>
#include <stdlib.h>

#include "character_factory.h"
#include "logger.h"
#include "memory.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#define MAP_WRITE 0x0002

int main(void)
{
    character_factory_t *factory;
    character_t *a_citizen;
    attending_officer_t *an_attending_officer;
    counter_intelligence_officer_t *a_counterintelligence_officer;

    int memory = shm_open("/nothinghere", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (memory == -1) {
        perror("shm_open error");
    }
    memory_t* p = mmap(NULL, sizeof(memory_t), MAP_WRITE, MAP_SHARED, memory, 0);
    if (p == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    log_info("Create one case officer.");
    an_attending_officer = new_case_officer();

    log_info("Create three source agents, one with a license to kill.");
    for (int i = 0; i < 3; ++i) {
        source_agent_t *source_agent = new_spy();
        if (i == 0) {
            source_agent->has_licence_to_kill = 1; // Assuming this attribute exists
        } else {
            source_agent->has_licence_to_kill = 0;
        }
        printf("Source agent id: %d\n", source_agent->character.id);
        free(source_agent);
    }

    log_info("Create one counterintelligence officer.");
    a_counterintelligence_officer = new_counterintelligence_officer();

    log_info("Create 127 citizens.");
    for (int i = 0; i < 127; ++i) {
        character_t *citizen = new_citizen();
        printf("Citizen id: %d\n", citizen->id);
        free(citizen);
    }


    free(factory);




    log_info("Create a new factory of counterintelligence officers.");
    factory = new_factory(new_counterintelligence_officer);
    log_info("Create a new counterintelligence officer.");
    a_counterintelligence_officer = factory->factory_method();
    log_info("Work with this counterintelligence officer...");

    free(factory);
    free(a_counterintelligence_officer);

    exit(EXIT_SUCCESS);
}
