#include "unity.h"
#include "game.h"
#include "cell.h"
#include "stdlib.h"
#include "malloc/malloc.h"


struct mstats mdata_before;

void setUp(void)
{
    mdata_before = mstats();
}

void tearDown(void)
{
}

//This test is run first and results in the appearance of a memory leak
//leak could be due to unity as no other tests have this leak.
void test_memory_leak(void)
{
    struct game_board_t * game_board = GME_init(3);
    GME_game_board_dtor(game_board);
}

void test_empty_size_3_gameboard_shows_blank(void)
{
    struct game_board_t * const game_board = GME_init(3);
    TEST_ASSERT_EQUAL_STRING(
        "   \n"
        "   \n"
        "   \n", GME_show(game_board)
        );
    GME_game_board_dtor(game_board);
    TEST_ASSERT_EQUAL_INT(mdata_before.chunks_used, mstats().chunks_used);
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
    GME_game_board_dtor(game_board);
    TEST_ASSERT_EQUAL_INT(mdata_before.chunks_used, mstats().chunks_used);
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
    GME_game_board_dtor(game_board);
    TEST_ASSERT_EQUAL_INT(mdata_before.chunks_used, mstats().chunks_used);
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
    GME_game_board_dtor(game_board);
    TEST_ASSERT_EQUAL_INT(mdata_before.chunks_used, mstats().chunks_used);
}

void test_persistent_block_remains(void)
{
    struct game_board_t * game_board = GME_init(3);
    GME_set(game_board, (struct point_t) {.x = 1, .y = 1});
    GME_set(game_board, (struct point_t) {.x = 1, .y = 2});
    GME_set(game_board, (struct point_t) {.x = 0, .y = 1});
    GME_set(game_board, (struct point_t) {.x = 0, .y = 2});
    TEST_ASSERT_EQUAL_STRING(
        "   \n"
        "oo \n"
        "oo \n", GME_show(game_board)
        );
    GME_iterate(game_board);
    TEST_ASSERT_EQUAL_STRING(
        "   \n"
        "oo \n"
        "oo \n", GME_show(game_board)
        );
    GME_game_board_dtor(game_board);
    TEST_ASSERT_EQUAL_INT(mdata_before.chunks_used, mstats().chunks_used);
}