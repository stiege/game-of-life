#include "unity.h"
#include "unit.h"
#include "game.h"
#include "cell.h"
#include "list.h"

struct unit_interface_t * p_unit_interface = NULL;
struct game_board_t * p_board = NULL;

void setUp(void)
{
    UNIT_init();
    p_board = GME_init(5);
    p_unit_interface = UNIT_factory_ctor(p_unit_interface);
    TEST_ASSERT_NOT_NULL(p_unit_interface);
}

void tearDown(void)
{
    GME_deinit();
}

void test_can_get_a_basic_block_unit(void)
{
    unit_t * p_block_unit = p_unit_interface->ctor(
        p_unit_interface, BLOCK_UNIT);
    TEST_ASSERT_NOT_NULL(p_block_unit);
    cell_list_t * p_cell_list = p_unit_interface->get_cell_list(p_block_unit);
    TEST_ASSERT_NOT_NULL(p_cell_list);
    struct new_unit_t new_unit = 
    {
        .p_cell_list = p_cell_list,
        .x = 0,
        .y = 0
    };
    GME_add_unit(p_board, &new_unit);
    TEST_ASSERT_EQUAL_STRING(
        "oo   \n"
        "oo   \n"
        "     \n"
        "     \n"
        "     \n", GME_show(p_board));
    GME_game_board_dtor(p_board);
}


