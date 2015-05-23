#include "unity.h"
#include "game.h"
#include "cell.h"

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

void test_empty_size_5_gameboard_shows_blank(void)
{
    struct game_board_t * game_board = GME_init(5);
    TEST_ASSERT_EQUAL_STRING(
        "     \n"
        "     \n"
        "     \n"
        "     \n"
        "     \n", GME_show(game_board)
        );
}

void test_can_set_game_board(void)
{
    struct game_board_t * game_board = GME_init(3);
    GME_set(game_board, (struct point_t) {
        .x = 1, .y = 1
    });
    TEST_ASSERT_EQUAL_STRING(
        "   \n"
        " o \n"
        "   \n", GME_show(game_board)
        );
}

void test_single_cell_dies(void)
{
    struct game_board_t * game_board = GME_init(3);
    GME_set(game_board, (struct point_t) {.x = 1, .y = 1});
    GME_iterate(game_board);
    TEST_ASSERT_EQUAL_STRING(
        "   \n"
        "   \n"
        "   \n", GME_show(game_board)
        );
}
