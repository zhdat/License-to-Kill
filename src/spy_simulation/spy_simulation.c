#include "spy_simulation.h"

#include "tools.h"
#include "logger.h"
#include "character_factory.h"
#include "cell.h"
#include "common.h"
#include "debug.h"

/**
 * \file spy_simulation.c
 * \brief Defines functions for initializing and managing the spy simulation.
 */

static int fd = -1;

static memory_t* _mem;

void set_memory(memory_t* mem) {
    _mem = mem;
}

memory_t* create_shared_memory(void) {
    memory_t* mem;

    fd = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0644);

    if (fd == -1) {
#if DEBUG
        perror("Error cannot open shared memory \n");
#endif
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fd, sizeof(memory_t)) == -1) {
#if DEBUG
        perror("Error ftruncate \n");
#endif
        exit(EXIT_FAILURE);
    }

    mem = (memory_t*) mmap(NULL, sizeof(memory_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    return mem;
}

void set_spies(memory_t* mem) {
    source_agent_factory_t* spy_factory_has_license, * spy_factory_has_not_license;
    source_agent_t spy;

    spy_factory_has_license = new_source_agent_factory(new_spy_with_licence);
    spy_factory_has_not_license = new_source_agent_factory(new_spy_without_licence);

    coordinate_t choosen_residence;

    for (int i = 0; i < MAX_SOURCE_AGENT_COUNT; i++) {
        if (i == 0) {
            choosen_residence = get_residence_near_mailbox(mem, 4);
            spy = spy_factory_has_license->factory_method(choosen_residence.row, choosen_residence.column);
            increments_population_in_cell(mem, choosen_residence.column, choosen_residence.row);
        } else {
            choosen_residence = get_residence_near_mailbox(mem, 4);
            spy = spy_factory_has_not_license->factory_method(choosen_residence.row, choosen_residence.column);
            increments_population_in_cell(mem, choosen_residence.column, choosen_residence.row);
        }
        mem->source_agents[i] = spy;
    }


}

void set_citizens(memory_t* mem) {
    character_factory_t* citizen_factory;
    character_t citizen;

    citizen_factory = new_character_factory(new_citizen);

    coordinate_t choosen_residence;

    for (int i = 0; i < MAX_CITIZEN_COUNT; i++) {
        choosen_residence = find_random_low_populated_residence(mem);
        citizen = citizen_factory->factory_method(choosen_residence.row, choosen_residence.column);
        increments_population_in_cell(mem, choosen_residence.column, choosen_residence.row);

        mem->citizens[i] = citizen;
    }
}

void set_attending_officers(memory_t* mem) {
    attending_officer_factory_t* attending_officer_factory;
    attending_officer_t attending_officer;

    attending_officer_factory = new_attending_officer_factory(new_case_officer);

    coordinate_t choosen_residence;

    for (int i = 0; i < MAX_ATTENDING_OFFICER_COUNT; i++) {
        choosen_residence = find_random_low_populated_residence(mem);
        attending_officer = attending_officer_factory->factory_method(choosen_residence.row, choosen_residence.column);
        attending_officer.mailbox_column = mem->mailbox_coordinate.column;
        attending_officer.mailbox_row = mem->mailbox_coordinate.row;

        increments_population_in_cell(mem, choosen_residence.column, choosen_residence.row);

        mem->attending_officers[i] = attending_officer;

    }
}

void set_counter_intelligence_officers(memory_t* mem) {
    counter_intelligence_officer_factory_t* counterintelligence_officer_factory;
    counter_intelligence_officer_t counter_intelligence_officer;

    counterintelligence_officer_factory = new_counter_intelligence_officer_factory(new_counter_intelligence_officer);

    coordinate_t city_hall;
    city_hall = findTypeOfBuilding(&(mem->city_map), CITY_HALL, 1)[0];

    for (int i = 0; i < MAX_COUNTER_INTELLIGENCE_OFFICER_COUNT; i++) {
        counter_intelligence_officer = counterintelligence_officer_factory->factory_method(city_hall.row,
                                                                                           city_hall.column, 2);
        counter_intelligence_officer.city_hall_row = city_hall.row;
        counter_intelligence_officer.city_hall_column = city_hall.column;
        counter_intelligence_officer.mailbox_column = mem->mailbox_coordinate.column;
        counter_intelligence_officer.mailbox_row = mem->mailbox_coordinate.row;


        increments_population_in_cell(mem, counter_intelligence_officer.city_hall_column,
                                      counter_intelligence_officer.city_hall_row);

        mem->counter_intelligence_officers[i] = counter_intelligence_officer;
    }
}


coordinate_t* get_residential_buildings(memory_t* mem) {

    coordinate_t* residential_buildings;
    residential_buildings = findTypeOfBuilding(&(mem->city_map), RESIDENTIAL_BUILDING, NUMBER_OF_RESIDENTIAL_BUILDINGS);
    return residential_buildings;
}

void set_mailbox(memory_t* mem) {
    coordinate_t* residential_buildings;
    residential_buildings = get_residential_buildings(mem);
    int randomResidentialBuilding = selectRandomNumberUnder(NUMBER_OF_RESIDENTIAL_BUILDINGS);
    mem->mailbox_coordinate.row = residential_buildings[randomResidentialBuilding].row;
    mem->mailbox_coordinate.column = residential_buildings[randomResidentialBuilding].column;
}

coordinate_t get_mailbox(memory_t* mem) {
    return mem->mailbox_coordinate;
}

coordinate_t get_residence_near_mailbox(memory_t* mem, int max_distance) {
    coordinate_t* residential_buildings;
    residential_buildings = get_residential_buildings(mem);
    int randomResidentialBuilding = selectRandomNumberUnder(NUMBER_OF_RESIDENTIAL_BUILDINGS);
    coordinate_t residence = residential_buildings[randomResidentialBuilding];

    // we don't want the residence to be the same as the mailbox
    while ((residence.row == mem->mailbox_coordinate.row) || (residence.column == mem->mailbox_coordinate.column) ||
           (euclidean_distance(residence.column, residence.row, mem->mailbox_coordinate.column,
                               mem->mailbox_coordinate.row) > max_distance)) {
        randomResidentialBuilding = selectRandomNumberUnder(NUMBER_OF_RESIDENTIAL_BUILDINGS);
        residence = residential_buildings[randomResidentialBuilding];
    }
    return residence;

}


coordinate_t find_random_low_populated_residence(memory_t* mem) {
    coordinate_t* residential_buildings;
    residential_buildings = get_residential_buildings(mem);
    coordinate_t mailbox = get_mailbox(mem);
    coordinate_t residence = residential_buildings[0];

    int min_population = 999;
    int selected_index = -1;

    for (int i = 0; i < NUMBER_OF_RESIDENTIAL_BUILDINGS; i++) {
        if (is_same_cell(residential_buildings[i], mailbox)) {
            continue;
        }

        int current_population = mem->city_map.cells[residential_buildings[i].row][residential_buildings[i].column].nb_of_characters;
        if (current_population < min_population) {
            min_population = current_population;
            selected_index = i;
        }

    }

    if (selected_index != -1) {
        residence = residential_buildings[selected_index];
    }

    return residence;
}


void affect_work_to_citizens(memory_t* mem) {
    coordinate_t* companies;
    companies = findTypeOfBuilding(&(mem->city_map), COMPANY, NUMBER_OF_COMPANIES);
    coordinate_t* supermarkets;
    supermarkets = findTypeOfBuilding(&(mem->city_map), SUPERMARKET, NUMBER_OF_SUPERMARKETS);
    coordinate_t* city_halls;
    city_halls = findTypeOfBuilding(&(mem->city_map), CITY_HALL, NUMBER_OF_CITY_HALLS);

    int randomSupermarketID;
    int randomCompanyID;
    int randomCitizen = selectRandomNumberUnder(MAX_CITIZEN_COUNT);
    coordinate_t city_hall = city_halls[0];

#if DEBUG
    printf("Affectation des citoyens...\n");
#endif
    for (int j = 0; j < 10; j++) {
        while (mem->citizens[randomCitizen].work_row != -1 || mem->citizens[randomCitizen].work_column != -1) {
            randomCitizen = selectRandomNumberUnder(MAX_CITIZEN_COUNT);
        }
        mem->citizens[randomCitizen].work_row = city_hall.row;
        mem->citizens[randomCitizen].work_column = city_hall.column;
    }

    for (int i = 0; i < NUMBER_OF_SUPERMARKETS; i++) {
        for (int j = 0; j < 3; j++) {
            randomSupermarketID = selectRandomNumberUnder(NUMBER_OF_SUPERMARKETS);
            coordinate_t supermarket = supermarkets[randomSupermarketID];
            while (mem->citizens[randomCitizen].work_row != -1 || mem->citizens[randomCitizen].work_column != -1) {
                randomCitizen = selectRandomNumberUnder(MAX_CITIZEN_COUNT);
            }
            mem->citizens[randomCitizen].work_row = supermarket.row;
            mem->citizens[randomCitizen].work_column = supermarket.column;
        }
    }

    for (int i = 0; i < NUMBER_OF_COMPANIES; i++) {
        for (int j = 0; j < 5; j++) {
            randomCompanyID = selectRandomNumberUnder(NUMBER_OF_COMPANIES);
            coordinate_t company = companies[randomCompanyID];
            while (mem->citizens[randomCitizen].work_row != -1 || mem->citizens[randomCitizen].work_column != -1) {
                randomCitizen = selectRandomNumberUnder(MAX_CITIZEN_COUNT);
            }
            mem->citizens[randomCitizen].work_row = company.row;
            mem->citizens[randomCitizen].work_column = company.column;
        }
    }

    // affectations de tous les citoyens restants dans des companies randomSupermarketId (max MAX_NUMBER_OF_CHARACTERS_ON_COMPANY)
    for (int i = 0; i < MAX_CITIZEN_COUNT; i++) {
        if (mem->citizens[i].work_row == -1 || mem->citizens[i].work_column == -1) {
            randomCompanyID = selectRandomNumberUnder(NUMBER_OF_COMPANIES);
            coordinate_t company = companies[randomCompanyID];
            while (mem->city_map.cells[company.column][company.row].nb_of_characters >=
                   MAX_NUMBER_OF_CHARACTERS_ON_COMPANY) {
                randomCompanyID = selectRandomNumberUnder(NUMBER_OF_COMPANIES);
                company = companies[randomCompanyID];
            }
            mem->citizens[i].work_row = company.row;
            mem->citizens[i].work_column = company.column;
        }
    }
#if DEBUG
    printf("Affectation des citoyens terminée\n");
#endif
}


void set_company_employees(memory_t* mem) {
    coordinate_t* companies_coordinates = findTypeOfBuilding(&(mem->city_map), COMPANY, NUMBER_OF_COMPANIES);
    for (int i = 0; i < NUMBER_OF_COMPANIES; i++) {
        coordinate_t current_company = companies_coordinates[i];
        mem->companies_priority[i].column = current_company.column;
        mem->companies_priority[i].row = current_company.row;
        for (int j = 0; j < MAX_CITIZEN_COUNT; j++) {
            coordinate_t current_citizen_coordinates = {mem->citizens[j].work_column, mem->citizens[j].work_row};
            if (is_same_cell(current_citizen_coordinates, current_company)) {
                mem->companies_priority[i].nb_of_employees++;
            }
        }
        mem->companies_priority[i].cruciality = getInformationDistribution(mem->companies_priority[i].nb_of_employees);
    }
}

InformationDistribution getInformationDistribution(int numberOfEmployees) {
    InformationDistribution distribution = {0};

    if (numberOfEmployees > 30) {
        distribution.infoCount[Crucial] = 2;
        distribution.infoCount[Strong] = 5;
        distribution.infoCount[Medium] = 12;
        distribution.infoCount[Low] = 20;
        distribution.infoCount[VeryLow] = 30;
    } else if (numberOfEmployees > 20) {
        distribution.infoCount[Crucial] = 0;
        distribution.infoCount[Strong] = 1;
        distribution.infoCount[Medium] = 12;
        distribution.infoCount[Low] = 20;
        distribution.infoCount[VeryLow] = 30;
    } else if (numberOfEmployees > 10) {
        distribution.infoCount[Crucial] = 0;
        distribution.infoCount[Strong] = 1;
        distribution.infoCount[Medium] = 7;
        distribution.infoCount[Low] = 20;
        distribution.infoCount[VeryLow] = 30;
    } else {
        distribution.infoCount[Crucial] = 0;
        distribution.infoCount[Strong] = 1;
        distribution.infoCount[Medium] = 7;
        distribution.infoCount[Low] = 11;
        distribution.infoCount[VeryLow] = 17;
    }

    return distribution;
}

void set_city_map(memory_t* mem) {
    clear_city(&(mem->city_map));
    init_city(&(mem->city_map));
    set_mailbox(mem);
}

void set_characters(memory_t* mem) {
    set_spies(mem);
    set_citizens(mem);
    set_attending_officers(mem);
    set_counter_intelligence_officers(mem);
    affect_work_to_citizens(mem);
    set_company_employees(mem);
}

void set_mailbox_messages(memory_t* mem) {
    mem->mailbox_size = 0;
    mem->decrypted_mailbox_size = 0;
}

void set_content_memory(memory_t* mem) {
    srand(time(NULL));
    mem->memory_has_changed = 1;
    mem->simulation_has_ended = 0;
    set_mailbox_messages(mem);
    set_city_map(mem);
    set_characters(mem);

}
