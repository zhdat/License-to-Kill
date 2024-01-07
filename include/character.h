#ifndef CHARACTER_H
#define CHARACTER_H

#include "cell.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

/**
 * \file character.h
 *
 *
 * \brief Constants and prototypes of the character structure.
 *
 * \details Defines a character within the city. A character may be:
 * - A citizen;
 * - A source agent;
 * - An attending officer;
 * - A counter intelligence officer.
 */

/**
 * "character_t" defines the character and its attributes.
 */

typedef struct character_s character_t;
typedef struct source_agent_s source_agent_t;
typedef struct attending_officer_s attending_officer_t;
typedef struct counter_intelligence_officer_s counter_intelligence_officer_t;

/**
 * @brief Structure of a character.
 *
 */
struct character_s {

    int id; /*!< The id of the character. */
    int row;    /*!< The row position of the character. */
    int column;    /*!< The column position of the character. */
    int health; /*!< The health points of the character. */
    int home_row;          /*!< The row position of the home of the character. */
    int home_column;          /*!< The column position of the home of the character. */
    int work_row;          /*!< The row position of the work of the character. */
    int work_column;          /*!< The column position of the work of the character. */
    int pid;          /*!< The pid of the character. */

};


/**
 * @brief Structure of a source agent.
 *
 */
struct source_agent_s {
    character_t character; /*!< The character. */
    int nb_of_stolen_companies; /*!< The number of stolen companies. */
    int has_licence_to_kill; /*!< The licence to kill. 1 if he has it, 0 otherwise. */
    int is_attacked; /*!< 1 if he is attacked, 0 otherwise. */
    coordinate_t targeted_companies[NUMBER_OF_COMPANIES]; /*!< The targeted companies. */
    int targeted_companies_count; /*!< The number of targeted companies. */
    char stolen_message[MAX_LENGTH_OF_MESSAGE]; /*!< The stolen message. */
};

/**
 * @brief Structure of an attending officer.
 *
 */
struct attending_officer_s {
    character_t character; /*!< The character. */
    int mailbox_row; /*!< The row of the mailbox. */
    int mailbox_column; /*!< The column of the mailbox. */
    int is_attacked; /*!< 1 if he is attacked, 0 otherwise. */
    int have_messages ; /*!< 1 if he has messages, 0 otherwise. */
};

/**
 * @brief Structure of a counter intelligence officer.
 *
 */
struct counter_intelligence_officer_s {
    character_t character; /*!< The character. */
    int city_hall_row; /*!< The row of the city hall. */
    int city_hall_column; /*!< The column of the city hall. */
    int mailbox_row; /*!< The row of the mailbox. */
    int mailbox_column; /*!< The column of the mailbox. */
    int targeted_character_id; /*!< The id of the targeted character. */
};


#endif /* CHARACTER_H */
