#include <stdlib.h>
#include <string.h>

int id_characters = 0;

character_factory_t *new_factory_character(character_t (*factory_method)(int, int, int))
{
    character_factory_t *factory;

    factory = (character_factory_t *) malloc(sizeof(character_factory_t));
    factory->factory_method = factory_method;

    return factory;
}

character_t new_citizen(int home_x, int home_y, int thread_id)
{
    character_t citizen;

    citizen.id = id_characters++;
    citizen.x = home_x;
    citizen.y = home_y;
    citizen.health_points = 10;
    citizen.home_x = home_x;
    citizen.home_y = home_y;
    citizen.work_x = -1;
    citizen.work_y = -1;
    citizen.pid = thread_id;

    return citizen;
}