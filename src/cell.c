#include "cell.h"
#include <stdio.h>
#include <stdlib.h>

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
            switch (city->cells[i][j].type) {
                case WASTELAND:
                    printf("W");
                    break;
                case RESIDENTIAL_BUILDING:
                    printf("R");
                    break;
                case CITY_HALL:
                    printf("C");
                    break;
                case COMPANY:
                    printf("O");
                    break;
                case SUPERMARKET:
                    printf("S");
                    break;
                default:
                    printf("?");
                    break;
            }
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

void clear_city(city_t* city) {
    for (int i = 0; i < city->height; i++) {
        for (int j = 0; j < city->width; j++) {
            city->cells[i][j].type = WASTELAND;
            city->cells[i][j].nb_of_characters = 0;
        }
    }
}

void init_city(city_t* city)
{
    city->width = 7;
    city->height = 7;

    printf("Init city...\n");

	if(city == NULL)
	{
		printf("Error: city is NULL in init_city\n");
		return;
	}
	city->cells[0][0].type = COMPANY;
    city->cells[0][1].type = CITY_HALL;
    city->cells[3][3].type = SUPERMARKET;
	city->cells[5][4].type = RESIDENTIAL_BUILDING;
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
