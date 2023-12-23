#include <stdio.h>
#include <stdlib.h>

#include "character_factory.h"
#include "logger.h"
#include "memory.h"


static int last_id = 0;

int generate_unique_id() {
    return ++last_id;
}

character_factory_t *new_factory(character_t *(*factory_method)(void))
{
    character_factory_t *factory;

    factory = (character_factory_t *) malloc(sizeof(character_factory_t));
    factory->factory_method = factory_method;

    return factory;
}

character_t *new_citizen(void)
{
    character_t *citizen;

    citizen = (character_t *) malloc(sizeof(character_t));
    if (citizen == NULL) {
        log_error("Failed to allocate memory for citizen");
        return NULL;
    }

    citizen->id = generate_unique_id(); // Assuming a function to generate a unique ID
    citizen->x = 1; // Assuming variables for initial positions
    citizen->y = 1;
    citizen->health = 10;
    citizen->home_x = 1;
    citizen->home_y = 1;
    citizen->work_x = -1;
    citizen->work_y = -1;



    return citizen;
}

character_t *new_spy(void)
{
    character_t *spy;

    spy = (character_t *) malloc(sizeof(character_t));
    if (spy == NULL) {
        log_error("Failed to allocate memory for spy");
        return NULL;
    }


    spy->id = generate_unique_id(); // Assuming a function to generate a unique ID
    spy->x = 1; // Assuming variables for initial positions
    spy->y = 1;
    spy->health = 10;
    spy->home_x = 1;
    spy->home_y = 1;
    spy->work_x = -1;
    spy->work_y = -1;
    return spy;
}

character_t *new_case_officer(void)
{
    character_t *case_officer;

    case_officer = (character_t *) malloc(sizeof(character_t));
    if (case_officer == NULL) {
        log_error("Failed to allocate memory for case officer");
        return NULL;
    }

    case_officer->id = generate_unique_id(); // Assuming a function to generate a unique ID
    case_officer->x = 1; // Assuming variables for initial positions
    case_officer->y = 1;
    case_officer->health = 10;
    case_officer->home_x = 1;
    case_officer->home_y = 1;
    case_officer->work_x = -1;
    case_officer->work_y = -1;



    return case_officer;
}

character_t *new_counterintelligence_officer(void)
{
    character_t *counterintelligence_officer;

    counterintelligence_officer = (character_t *) malloc(sizeof(character_t));

    return counterintelligence_officer;
}



void operation_citizen(character_t *citizen)
{
    printf(">> Operation of a citizen...\n");
}

void operation_spy(character_t *spy)
{
    printf(">> Operation of a spy...\n");
}

void operation_case_officer(character_t *case_officer)
{
    printf(">> Operation of a case officer...\n");
}

void operation_counterintelligence_officer(character_t *counterintelligence_officer)
{
    printf(">> Operation of a counterintelligence_officer...\n");
}