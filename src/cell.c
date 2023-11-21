#include "cell.h"
#include <stdio.h>
#include <stdlib.h>

city_t* create_city(int width, int height)
{
	int i;
	city_t* new_city = (city_t*)malloc(sizeof(city_t));
	if(new_city == NULL)
	{
		printf("Error: malloc failed in create_city\n");
		return NULL;
	}

	new_city->width = width;
	new_city->height = height;
	new_city->cells = (cell_t**)malloc(height * sizeof(cell_t*));
	if(new_city->cells == NULL)
	{
		printf("Error: malloc of cells failed in create_city\n");
		free(new_city);
		return NULL;
	}

	for(i = 0; i < height; i++)
	{
		new_city->cells[i] = (cell_t*)calloc(width, sizeof(cell_t));
		if(new_city->cells[i] == NULL)
		{
			printf("Error: malloc of cells[] failed in create_city\n", i);
			while(i--)
			{
				free(new_city->cells[i]);
			}
			free(new_city->cells);
			free(new_city);
			return NULL;
		}
	}
	return new_city;
}

city_t* delete_city(city_t* city)
{
	int i;
	if(city != NULL)
	{
		for(i = 0; i < city->height; i++)
		{
			free(city->cells[i]);
		}
		free(city->cells);
		free(city);
	}
}

city_t* print_city(city_t* city)
{
	int i, j;
	if(city == NULL)
	{
		printf("Error: city is NULL in print_city\n");
		return NULL;
	}

	for(i = 0; i < city->height; i++)
	{
		for(j = 0; j < city->width; j++)
		{
			printf("%d ", city->cells[i][j].type);
		}
		printf("\n");
	}
}

cell_t* get_cell(city_t* city, int x, int y)
{
	if(city == NULL || x < 0 || x >= city->width || y < 0 || y >= city->height)
	{
		printf("Error: invalid parameters in get_cell\n");
		return NULL;
	}
	return &city->cells[y][x];
}

void define_monitoring(city_t* city, int x, int y, int nb_of_characters)
{
	cell_t* cell;
	cell = get_cell(city, x, y);
	if(cell != NULL)
	{
		cell->nb_of_characters = nb_of_characters;
	}
}

void init_city(city_t* city)
{
	if(city == NULL)
	{
		printf("Error: city is NULL in init_city\n");
		return;
	}

	// Parcours de chaque cellule de la ville
	/* for(int i = 0; i < city->height; i++)
	{
		for(int j = 0; j < city->width; j++)
		{
			// Ici, vous pouvez définir la logique pour déterminer le type de chaque cellule
			// Exemple : assigner des types de bâtiments basés sur certaines règles ou de manière aléatoire
			// Pour l'exemple, on assigne WASTELAND à toutes les cellules
			city->cells[i][j].type = WASTELAND;
			city->cells[i][j].nb_of_characters = 0; // Initialisation du nombre de personnages à 0
		}
	} */
	city->cells[0][0].type = WASTELAND;
	city->cells[5][4].type = RESIDENTIAL_BUILDING;
	switch(city->cells[5][4].type)
	{
	case RESIDENTIAL_BUILDING:
		printf("residential building \n");
		break;
	case WASTELAND:
		printf("wasteland \n");
		break;
	default:
		printf("Invalid type \n");
		break;
	}
	printf("BIP\n");
}

int should_be_monitored(cell_type_t cell_type)
{
	switch(cell_type)
	{
	case COMPANY:
	case CITY_HALL:
		return 1; // Surveillance accrue pour les bâtiments importants
	default:
		return 0; // Pas de surveillance ou surveillance minimale
	}
}

void initialize_surveillance_system(city_t* city)
{
	if(city == NULL)
	{
		printf("Error: city is NULL in initialize_surveillance_system\n");
		return;
	}

	for(int i = 0; i < city->height; i++)
	{
		for(int j = 0; j < city->width; j++)
		{
			cell_t* cell = &city->cells[i][j];
			if(should_be_monitored(cell->type))
			{
				// Configurez ici la surveillance pour la cellule
				// Par exemple, augmenter un niveau de surveillance ou assigner des ressources de surveillance
				// Pour l'exemple, on va simplement marquer la cellule comme surveillée
				cell->nb_of_characters = 1;
			}
		}
	}
}
