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
    int row;	/*!< The row position of the character. */
    int column;	/*!< The column position of the character. */
    int health; /*!< The health points of the character. */
    int home_row;		  /*!< The row position of the home of the character. */
    int home_column;		  /*!< The column position of the home of the character. */
    int work_row;		  /*!< The row position of the work of the character. */
    int work_column;		  /*!< The column position of the work of the character. */
    int pid;		  /*!< The pid of the character. */

};


/* Structure of source_agent */
struct source_agent_s {
	character_t character; /*!< The character. */
	int nb_of_stolen_companies; /*!< The number of stolen companies. */
	int has_licence_to_kill; /*!< The licence to kill. 1 if he has it, 0 otherwise. */
	int is_attacked; /*!< 1 if he is attacked, 0 otherwise. */
};

/* Structure of attending_officer */
struct attending_officer_s {
	character_t character; /*!< The character. */
	int mailbox_row; /*!< The row of the mailbox. */
	int mailbox_column; /*!< The column of the mailbox. */
	int is_attacked; /*!< 1 if he is attacked, 0 otherwise. */
};

/* Structure of counter_intelligence_officer */
struct counter_intelligence_officer_s {
	character_t character; /*!< The character. */
    int city_hall_row;
    int city_hall_column;
    int mailbox_row;
    int mailbox_column;
    int targeted_character_id;
};

/* Character management functions */
character_t* create_character(int id, int x, int y);
void delete_charactere(character_t* character);
void move_character(character_t* character, int newX, int newY);
void initialize_characters(city_t* city);

#endif /* CHARACTER_H */
