#include "minunit.h"
#include "character.h"
#include <stdio.h>

MU_TEST(test_create_character)
{
	character_t* character = create_character(1, 10, 20);

	mu_check(character != NULL);
	mu_assert_int_eq(1, character->id);
	mu_assert_int_eq(10, character->x);
	mu_assert_int_eq(20, character->y);

	delete_charactere(character);
}

MU_TEST(test_move_character)
{
	character_t* character = create_character(2, 10, 20);
	move_character(character, 30, 40);

	mu_assert_int_eq(30, character->x);
	mu_assert_int_eq(40, character->y);

	delete_charactere(character);
}

MU_TEST_SUITE(character_test_suite)
{
	MU_RUN_TEST(test_create_character);
	MU_RUN_TEST(test_move_character);
}

int main()
{
	MU_RUN_SUITE(character_test_suite);
	MU_REPORT();
	return 0;
}
