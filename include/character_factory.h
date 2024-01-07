#ifndef TP_SYS_CHARACTER_FACTORY_H
#define TP_SYS_CHARACTER_FACTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "character.h"
#include "debug.h"
#include "logger.h"
#include "memory.h"
#include "spy_simulation.h"

/**
 * \file character_factory.h
 *
 * \brief Defines the character factory.
 */

/**
 * "character_factory_t" declares the production function that should return
 * "character_t" objects.
 */

typedef struct character_factory_s character_factory_t;
typedef struct source_agent_factory_s source_agent_factory_t;
typedef struct attending_officer_factory_s attending_officer_factory_t;
typedef struct counter_intelligence_officer_factory_s counter_intelligence_officer_factory_t;

/**
 * @struct character_factory_s
 * \brief Represents a factory for creating generic character objects.
 *
 * This structure encapsulates a factory method for creating new characters,
 * allowing for the creation of various types of characters with different
 * properties and behaviors.
 */
struct character_factory_s {
    character_t (* factory_method)(int, int);
};

/**
 * @struct source_agent_factory_s
 * \brief Represents a factory for creating source agent characters.
 *
 * This structure encapsulates a factory method specifically for creating
 * source agent characters, a specialized type of character in the spy simulation.
 */
struct source_agent_factory_s {
    source_agent_t (* factory_method)(int, int);
};

/**
 * @struct attending_officer_factory_s
 * \brief Represents a factory for creating attending officer characters.
 *
 * This structure encapsulates a factory method for creating attending officer
 * characters. Attending officers have specific roles and attributes in the spy
 * simulation context.
 */
struct attending_officer_factory_s {
    attending_officer_t (* factory_method)(int, int);
};

/**
 * @struct counter_intelligence_officer_factory_s
 * \brief Represents a factory for creating counter-intelligence officer characters.
 *
 * This structure encapsulates a factory method for creating counter-intelligence
 * officer characters. These officers play a crucial role in the spy simulation,
 * having unique abilities and goals.
 */
struct counter_intelligence_officer_factory_s {
    counter_intelligence_officer_t (* factory_method)(int, int, int);
};

/**
 * \brief Creates a new character factory.
 *
 * \param factory_method The function that will be used to create new characters.
 * @return A pointer to the new character factory.
 */
character_factory_t* new_character_factory(character_t (* factory_method)(int, int));

/**
 * \brief Creates a new source agent factory.
 *
 * \param factory_method The function that will be used to create new source agents.
 * @return A pointer to the new source agent factory.
 */
source_agent_factory_t* new_source_agent_factory(source_agent_t (* factory_method)(int, int));

/**
 * \brief Creates a new attending officer factory.
 *
 * \param factory_method The function that will be used to create new attending officers.
 * @return A pointer to the new attending officer factory.
 */
attending_officer_factory_t* new_attending_officer_factory(attending_officer_t (* factory_method)(int, int));

/**
 * \brief Creates a new counter intelligence officer factory.
 *
 * \param factory_method The function that will be used to create new counter intelligence officers.
 * @return A pointer to the new counter intelligence officer factory.
 */
counter_intelligence_officer_factory_t*
new_counter_intelligence_officer_factory(counter_intelligence_officer_t (* factory_method)(int, int, int));

/**
 * \brief Creates a new character.
 *
 * \param row The row where the character will be created.
 * \param column The column where the character will be created.
 * @return The new character.
 */
character_t new_citizen(int, int);

/**
 * \brief Creates a new source agent without licence.
 *
 * \param row The row where the source agent will be created.
 * \param column The column where the source agent will be created.
 * @return The new source agent without licence.
 */
source_agent_t new_spy_without_licence(int, int);

/**
 * \brief Creates a new source agent with licence.
 *
 * \param row The row where the source agent will be created.
 * \param column The column where the source agent will be created.
 * @return The new source agent with licence.
 */
source_agent_t new_spy_with_licence(int, int);

/**
 * \brief Creates a new attending officer.
 *
 * \param row The row where the attending officer will be created.
 * \param column The column where the attending officer will be created.
 * @return The new attending officer.
 */
attending_officer_t new_case_officer(int, int);

/**
 * \brief Creates a new counter intelligence officer.
 *
 * \param row The row where the counter intelligence officer will be created.
 * \param column The column where the counter intelligence officer will be created.
 * \param targeted_character_id The ID of the character that the counter intelligence officer will target.
 * @return The new counter intelligence officer.
 */
counter_intelligence_officer_t new_counter_intelligence_officer(int, int, int);


#endif //TP_SYS_CHARACTER_FACTORY_H
