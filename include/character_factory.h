#ifndef CHARACTER_FACTORY_H
#define CHARACTER_FACTORY_H

#include <stdlib.h>
#include <string.h>
#include "character.h"

typedef struct character_factory_s character_factory_t;

character_factory_t *new_factory_character(character_t (*factory_method)(int, int));

character_t new_citizen(int home_x, int home_y);

#endif /* CHARACTER_FACTORY_H */