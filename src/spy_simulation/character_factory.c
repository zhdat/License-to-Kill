#include "character_factory.h"

/**
 * @file character_factory.c
 * @brief Defines character factories for creating various types of characters in the spy simulation.
 */

static int current_id = 0;

static int generate_unique_id(void) {
    return ++current_id;
}

character_factory_t *new_character_factory(character_t (*factory_method)(int, int)) {
    character_factory_t *factory;

    factory = (character_factory_t *) malloc(sizeof(character_factory_t));
    factory->factory_method = factory_method;

    return factory;
}

static void set_basic_character_attributes(character_t *character, int row, int column) {
    character->id = generate_unique_id();
    character->row = row;
    character->column = column;
    character->health = 10;
    character->home_row = row;
    character->home_column = column;
    character->work_row = -1;
    character->work_column = -1;
}

character_t new_citizen(int row, int column) {
    character_t citizen;

    set_basic_character_attributes(&citizen, row, column);

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

    set_basic_character_attributes(&spy.character, row, column);
    spy.has_licence_to_kill = 1;
    spy.nb_of_stolen_companies = 0;
    spy.is_attacked = 0;
    spy.targeted_companies_count = 0;
    strcpy(spy.stolen_message, EMPTY);

    return spy;

}

source_agent_t new_spy_without_licence(int row, int column) {
    source_agent_t spy = new_spy_with_licence(row, column);

    spy.has_licence_to_kill = 0;
    spy.nb_of_stolen_companies = 0;
    spy.is_attacked = 0;
    spy.targeted_companies_count = 0;
    strcpy(spy.stolen_message, EMPTY);

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

    set_basic_character_attributes(&case_officer.character, row, column);
    case_officer.is_attacked = 0;
    case_officer.have_messages = 0;

    return case_officer;
}

counter_intelligence_officer_factory_t * new_counter_intelligence_officer_factory(counter_intelligence_officer_t (*factory_method)(int, int, int)) {
    counter_intelligence_officer_factory_t *factory;

    factory = (counter_intelligence_officer_factory_t *) malloc(sizeof(counter_intelligence_officer_factory_t));
    factory->factory_method = factory_method;

    return factory;
}

counter_intelligence_officer_t new_counter_intelligence_officer(int row, int column, int target_id) {
    counter_intelligence_officer_t counterintelligence_officer;


    set_basic_character_attributes(&counterintelligence_officer.character, row, column);
    counterintelligence_officer.targeted_character_id = target_id;

    return counterintelligence_officer;
}
