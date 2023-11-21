#include "minunit.h"
#include "cell.h"
#include <stdio.h>

MU_TEST(test_create_city)
{
	city_t* city = create_city(5, 5);

	mu_check(city != NULL);
	mu_assert_int_eq(5, city->width);
	mu_assert_int_eq(5, city->height);
	for (int i = 0; i < city->height; i++) {
		mu_check(city->cells[i] != NULL);
	}

	delete_city(city);
}

MU_TEST(test_get_cell)
{
	city_t* city = create_city(5, 5);
	cell_t* cell = get_cell(city, 2, 2);

	mu_check(cell != NULL);
	mu_assert_int_eq(WASTELAND, cell->type); // Assuming default type is WASTELAND

	delete_city(city);
}

MU_TEST(test_define_monitoring)
{
	city_t* city = create_city(5, 5);
	define_monitoring(city, 2, 2, 10);
	cell_t* cell = get_cell(city, 2, 2);

	mu_assert_int_eq(10, cell->nb_of_characters);

	delete_city(city);
}

MU_TEST_SUITE(city_test_suite)
{
	MU_RUN_TEST(test_create_city);
	MU_RUN_TEST(test_get_cell);
	MU_RUN_TEST(test_define_monitoring);
}

int main()
{
	MU_RUN_SUITE(city_test_suite);
	MU_REPORT();
	return 0;
}
