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

struct character_factory_s {
    character_t *(*factory_method)(void);
};

character_factory_t *new_factory(character_t *(*factory_method)(void));
character_t *new_citizen(void);
character_t *new_spy(void);
character_t *new_case_officer(void);
character_t *new_counterintelligence_officer(void);
void operation_citizen(character_t *citizen);
void operation_spy(character_t *spy);
void operation_case_officer(character_t *case_officer);
void operation_counterintelligence_officer(character_t *counterintelligence_officer);


#endif //TP_SYS_CHARACTER_FACTORY_H
