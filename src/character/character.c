#include "character.h"
#include <stdlib.h>
#include <stdio.h>

character_t* create_character(int id, int x, int y)
{
	character_t* new_character = (character_t*)malloc(sizeof(character_t));
	if (new_character == NULL) {
		printf("Error: malloc failed in create_character\n");
		return NULL;
	}
	new_character->id = id;
	new_character->row = x;
	new_character->column = y;
	return new_character;
}

void delete_charactere(character_t* character)
{
	if (character != NULL)
		free(character);
}

void move_character(character_t* character, int newX, int newY)
{
	if (character != NULL) {
		character->row = newX;
		character->column = newY;
	}
}

void initialize_characters(city_t* city)
{
	if (city == NULL) {
		printf("Error: city is NULL in initialize_characters\n");
		return;
	}

	// Exemple de création et positionnement de personnages
	// Vous pouvez ajuster la logique selon vos besoins spécifiques

	// Création d'un agent source
	character_t* source_agent = create_character(1, 2, 3); // ID, row, column
	if (source_agent != NULL) {
		cell_t* cell = get_cell(city, source_agent->row, source_agent->column);
		cell->nb_of_characters++; // Incrémenter le nombre de personnages sur la cellule
								  // Vous pouvez également ajouter des informations supplémentaires à la cellule si nécessaire
	}

	// Création d'un officier traitant
	character_t* attending_officer = create_character(2, 4, 5); // ID, row, column
	if (attending_officer != NULL) {
		cell_t* cell = get_cell(city, attending_officer->row, attending_officer->column);
		cell->nb_of_characters++;
		// Ajoutez ici d'autres actions pour l'officier traitant
	}

	// Ajoutez d'autres types de personnages selon les besoins de votre jeu
	// Par exemple, un officier de contre-espionnage, des citoyens, etc.

	// Notez que vous devriez également gérer la mémoire des personnages créés
	// Vous pouvez stocker les pointeurs dans un tableau ou une autre structure de données
	// pour les gérer efficacement pendant la durée de vie de la simulation
}