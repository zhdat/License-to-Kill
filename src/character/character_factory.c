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

source_agent_t *new_spy(void)
{
    source_agent_t *spy;

    spy = (source_agent_t *) malloc(sizeof(source_agent_t));
    if (spy == NULL) {
        log_error("Failed to allocate memory for spy");
        return NULL;
    }


    spy->character.id = generate_unique_id(); // Assuming a function to generate a unique ID
    spy->character.x = 1; // Assuming variables for initial positions
    spy->character.y = 1;
    spy->character.health = 10;
    spy->character.home_x = 1;
    spy->character.home_y = 1;
    spy->character.work_x = -1;
    spy->character.work_y = -1;
    spy->has_licence_to_kill = 0;
    spy->level_agent = 0;

    return spy;
}

attending_officer_t *new_case_officer(void)
{
    attending_officer_t *case_officer;

    case_officer = (attending_officer_t *) malloc(sizeof(attending_officer_t));
    if (case_officer == NULL) {
        log_error("Failed to allocate memory for case officer");
        return NULL;
    }

    case_officer->character.id = generate_unique_id(); // Assuming a function to generate a unique ID
    case_officer->character.x = 1; // Assuming variables for initial positions
    case_officer->character.y = 1;
    case_officer->character.health = 10;
    case_officer->character.home_x = 1;
    case_officer->character.home_y = 1;
    case_officer->character.work_x = -1;
    case_officer->character.work_y = -1;
    case_officer->nb_agent = 0;


    return case_officer;
}

counter_intelligence_officer_t *new_counterintelligence_officer(void)
{
    counter_intelligence_officer_t *counterintelligence_officer;

    counterintelligence_officer = (counter_intelligence_officer_t *) malloc(sizeof(counter_intelligence_officer_t));
    if (counterintelligence_officer == NULL) {
        log_error("Failed to allocate memory for counterintelligence officer");
        return NULL;
    }

    counterintelligence_officer->character.id = generate_unique_id(); // Assuming a function to generate a unique ID
    counterintelligence_officer->character.x = 1; // Assuming variables for initial positions
    counterintelligence_officer->character.y = 1;
    counterintelligence_officer->character.health = 10;
    counterintelligence_officer->character.home_x = 1;
    counterintelligence_officer->character.home_y = 1;
    counterintelligence_officer->character.work_x = -1;
    counterintelligence_officer->character.work_y = -1;
    counterintelligence_officer->radius_action = 0;


    return counterintelligence_officer;
}