#include "character_factory.h"
#include <stdio.h>
#include <stdlib.h>


#include "logger.h"
#include "memory.h"
#include "spy_simulation.h"


static int last_id = 0;

int generate_unique_id(void ) {
    return ++last_id;
}

character_factory_t *new_character_factory(character_t (*factory_method)(int, int))
{
    character_factory_t *factory;

    factory = (character_factory_t *) malloc(sizeof(character_factory_t));
    factory->factory_method = factory_method;

    return factory;
}

character_t new_citizen(int x, int y)
{
    character_t citizen;

    citizen.id = generate_unique_id(); // Assuming a function to generate a unique ID
    citizen.row = x; // Assuming variables for initial positions
    citizen.column = y;
    citizen.health = 10;
    citizen.home_row = x;
    citizen.home_column = y;
    citizen.work_row = -1;
    citizen.work_column = -1;



    return citizen;
}

source_agent_factory_t *new_source_agent_factory(source_agent_t (*factory_method)(int, int))
{
    source_agent_factory_t *factory;

    factory = (source_agent_factory_t *) malloc(sizeof(source_agent_factory_t));
    factory->factory_method = factory_method;

    return factory;
}


source_agent_t new_spy_with_licence(int x, int y)
{
    source_agent_t spy;

    spy.character.id = generate_unique_id(); // Assuming a function to generate a unique ID
    spy.character.row = x; // Assuming variables for initial positions
    spy.character.column = y;
    spy.character.health = 10;
    spy.character.home_row = x;
    spy.character.home_column = y;
    spy.character.work_row = -1;
    spy.character.work_column = -1;
    spy.has_licence_to_kill = 1;
    spy.level_agent = 0;

    return spy;

}

source_agent_t new_spy_without_licence(int x, int y)
{
    source_agent_t spy;

    spy.character.id = generate_unique_id(); // Assuming a function to generate a unique ID
    spy.character.row = x; // Assuming variables for initial positions
    spy.character.column = y;
    spy.character.health = 10;
    spy.character.home_row = x;
    spy.character.home_column = y;
    spy.character.work_row = -1;
    spy.character.work_column = -1;
    spy.has_licence_to_kill = 0;
    spy.level_agent = 0;

    return spy;
}

attending_officer_factory_t *new_attending_officer_factory(attending_officer_t (*factory_method)(int, int))
{
    attending_officer_factory_t *factory;

    factory = (attending_officer_factory_t *) malloc(sizeof(attending_officer_factory_t));
    factory->factory_method = factory_method;

    return factory;
}

attending_officer_t new_case_officer(int x, int y)
{
    attending_officer_t case_officer;

    case_officer.character.id = generate_unique_id(); // Assuming a function to generate a unique ID
    case_officer.character.row = x; // Assuming variables for initial positions
    case_officer.character.column = y;
    case_officer.character.health = 10;
    case_officer.character.home_row = x;
    case_officer.character.home_column = y;
    case_officer.character.work_row = -1;
    case_officer.character.work_column = -1;

    return case_officer;
}

counter_intelligence_officer_factory_t *new_counter_intelligence_officer_factory(counter_intelligence_officer_t (*factory_method)(int, int, int))
{
    counter_intelligence_officer_factory_t *factory;

    factory = (counter_intelligence_officer_factory_t *) malloc(sizeof(counter_intelligence_officer_factory_t));
    factory->factory_method = factory_method;

    return factory;
}

counter_intelligence_officer_t new_counter_intelligence_officer(int x, int y, int target_id)
{
    counter_intelligence_officer_t counterintelligence_officer;


    counterintelligence_officer.character.id = generate_unique_id(); // Assuming a function to generate a unique ID
    counterintelligence_officer.character.row = x; // Assuming variables for initial positions
    counterintelligence_officer.character.column = y;
    counterintelligence_officer.character.health = 10;
    counterintelligence_officer.character.home_row = x;
    counterintelligence_officer.character.home_column = y;
    counterintelligence_officer.character.work_row = -1;
    counterintelligence_officer.character.work_column = -1;
    counterintelligence_officer.city_hall_column = 4;
    counterintelligence_officer.city_hall_row = 4;
    counterintelligence_officer.targeted_character_id = target_id;



    return counterintelligence_officer;
}