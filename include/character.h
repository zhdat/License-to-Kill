#ifndef CHARACTER_H
#define CHARACTER_H

#include "cell.h"

typedef struct character_s character_t;
typedef struct source_agent_s source_agent_t;
typedef struct attending_officer_s attending_officer_t;
typedef struct counter_intelligence_officer_s counter_intelligence_officer_t;

/* basic structure of character */
struct character_s {
	int id; /*!< The id of the character. */
	int x;	/*!< The x position of the character. */
	int y;	/*!< The y position of the character. */
};

/* Structure of source_agent */
struct source_agent_s {
	character_t character; /*!< The character. */
	int level_agent;	   /*!< The level of the agent. */
};

/* Structure of attending_officer */
struct attending_officer_s {
	character_t character; /*!< The character. */
	int nb_agent;		   /*!< The number of agent under his control. */
};

/* Structure of counter_intelligence_officer */
struct counter_intelligence_officer_s {
	character_t character; /*!< The character. */
	int radius_action;	   /*!< The radius of action of the officer. */
};

/* Character management functions */
character_t* create_character(int id, int x, int y);
void delete_charactere(character_t* character);
void move_character(character_t* character, int newX, int newY);
void initialize_characters(city_t* city);

#endif /* CHARACTER_H */
