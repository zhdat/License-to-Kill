//
// Created by clement on 22/12/23.
//

#ifndef TP_SYS_CHARACTER_FACTORY_H
#define TP_SYS_CHARACTER_FACTORY_H

#include <stdio.h>
#include <stdlib.h>
#include "character.h"

/*
 * "character_t" defines the interface to "objects" that will be created by the factory.
 */

/*
 * "character_factory_t" declares the production function that should return
 * "character_t" objects.
 */
typedef struct character_factory_s character_factory_t;
typedef struct source_agent_factory_s source_agent_factory_t;
typedef struct attending_officer_factory_s attending_officer_factory_t;
typedef struct counter_intelligence_officer_factory_s counter_intelligence_officer_factory_t;

struct character_factory_s {
    character_t (*factory_method)(int, int);
};

struct source_agent_factory_s {
    source_agent_t (*factory_method)(int,int);
};

struct attending_officer_factory_s {
    attending_officer_t (*factory_method)(int, int);
};

struct counter_intelligence_officer_factory_s {
    counter_intelligence_officer_t (*factory_method)(int, int, int);
};

character_factory_t *new_character_factory(character_t (*factory_method)(int, int));
source_agent_factory_t *new_source_agent_factory(source_agent_t (*factory_method)(int,int));
attending_officer_factory_t *new_attending_officer_factory(attending_officer_t (*factory_method)(int, int));
counter_intelligence_officer_factory_t *new_counter_intelligence_officer_factory(counter_intelligence_officer_t (*factory_method)(int, int, int));
character_t new_citizen(int, int);
source_agent_t new_spy_without_licence(int,int);
source_agent_t new_spy_with_licence(int,int);
attending_officer_t new_case_officer(int, int);
counter_intelligence_officer_t new_counter_intelligence_officer(int, int, int);
int generate_unique_id(void);


#endif //TP_SYS_CHARACTER_FACTORY_H
