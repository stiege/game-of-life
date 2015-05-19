#include "unity.h"
#include "game.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_empty_size_3_gameboard_shows_blank(void)
{
    struct game_board_t * game_board = GME_init(3);
    TEST_ASSERT_EQUAL_STRING(
        "   \n"
        "   \n"
        "   \n", GME_show(game_board)
        );
}