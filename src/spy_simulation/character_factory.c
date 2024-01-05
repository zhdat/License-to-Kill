#include "character_factory.h"

/**
 * @file character_factory.c
 * @brief Defines character factories for creating various types of characters in the spy simulation.
 */

static int last_id = 0;


int generate_unique_id(void) {
    return ++last_id;
}

character_factory_t *new_character_factory(character_t (*factory_method)(int, int)) {
    character_factory_t *factory;

    factory = (character_factory_t *) malloc(sizeof(character_factory_t));
    factory->factory_method = factory_method;

    return factory;
}

character_t new_citizen(int row, int column) {
    character_t citizen;

    citizen.id = generate_unique_id(); // Assuming a function to generate a unique ID
    citizen.row = row; // Assuming variables for initial positions
    citizen.column = column;
    citizen.health = 10;
    citizen.home_row = row;
    citizen.home_column = column;
    citizen.work_row = -1;
    citizen.work_column = -1;


    return citizen;
}

source_agent_factory_t *new_source_agent_factory(source_agent_t (*factory_method)(int, int)) {
    source_agent_factory_t *factory;

    factory = (source_agent_factory_t *) malloc(sizeof(source_agent_factory_t));
    factory->factory_method = factory_method;

    return factory;
}


source_agent_t new_spy_with_licence(int row, int column) {
    source_agent_t spy;

    spy.character.id = generate_unique_id();
    spy.character.row = row;
    spy.character.column = column;
    spy.character.health = 10;
    spy.character.home_row = row;
    spy.character.home_column = column;
    spy.character.work_row = -1;
    spy.character.work_column = -1;
    spy.has_licence_to_kill = 1;
    spy.nb_of_stolen_companies = 0;
    spy.is_attacked = 0;
    spy.targeted_companies_count = 0;

    return spy;

}

source_agent_t new_spy_without_licence(int row, int column) {
    source_agent_t spy;

    spy.character.id = generate_unique_id();
    spy.character.row = row;
    spy.character.column = column;
    spy.character.health = 10;
    spy.character.home_row = row;
    spy.character.home_column = column;
    spy.character.work_row = -1;
    spy.character.work_column = -1;
    spy.has_licence_to_kill = 0;
    spy.nb_of_stolen_companies = 0;
    spy.is_attacked = 0;
    spy.targeted_companies_count = 0;

    return spy;
}

attending_officer_factory_t *new_attending_officer_factory(attending_officer_t (*factory_method)(int, int)) {
    attending_officer_factory_t *factory;

    factory = (attending_officer_factory_t *) malloc(sizeof(attending_officer_factory_t));
    factory->factory_method = factory_method;

    return factory;
}

attending_officer_t new_case_officer(int row, int column) {
    attending_officer_t case_officer;

    case_officer.character.id = generate_unique_id();
    case_officer.character.row = row;
    case_officer.character.column = column;
    case_officer.character.health = 10;
    case_officer.character.home_row = row;
    case_officer.character.home_column = column;
    case_officer.character.work_row = -1;
    case_officer.character.work_column = -1;
    case_officer.is_attacked = 0;

    return case_officer;
}

counter_intelligence_officer_factory_t *
new_counter_intelligence_officer_factory(counter_intelligence_officer_t (*factory_method)(int, int, int)) {
    counter_intelligence_officer_factory_t *factory;

    factory = (counter_intelligence_officer_factory_t *) malloc(sizeof(counter_intelligence_officer_factory_t));
    factory->factory_method = factory_method;

    return factory;
}

counter_intelligence_officer_t new_counter_intelligence_officer(int row, int column, int target_id) {
    counter_intelligence_officer_t counterintelligence_officer;


    counterintelligence_officer.character.id = generate_unique_id();
    counterintelligence_officer.character.row = row;
    counterintelligence_officer.character.column = column;
    counterintelligence_officer.character.health = 10;
    counterintelligence_officer.character.home_row = row;
    counterintelligence_officer.character.home_column = column;
    counterintelligence_officer.character.work_row = -1;
    counterintelligence_officer.character.work_column = -1;
    counterintelligence_officer.targeted_character_id = target_id;

    return counterintelligence_officer;
}
