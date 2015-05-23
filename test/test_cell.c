#include "unity.h"
#include "cell.h"
#include "stdbool.h"

char const * underpopulated_board = 
"   "
" o "
"   ";

const struct cell_t underpopulated_cell_from_board = {.x=1,.y=1};

void setUp(void)
{
}

void tearDown(void)
{
}

void test_returns_cells_from_string(void)
{
    cell_list_t * cell_list = CELL_list_from_string(underpopulated_board);
    TEST_ASSERT_EQUAL_INT( 1 , CELL_get_list_length(cell_list));

    struct cell_t returned_cell;
    TEST_ASSERT(CELL_pop_from_list(cell_list, &returned_cell));
}

void test_returns_underpopulated_cells(void)
{
    cell_list_t * cell_list = CELL_list_from_string(underpopulated_board);
    cell_list_t * underpopulated_list = 
        CELL_filter_for_underpopulated(cell_list);
    TEST_ASSERT_NOT_NULL(underpopulated_list);
    TEST_ASSERT_EQUAL_INT( 1 , CELL_get_list_length(underpopulated_list));
    struct cell_t returned_cell;
    TEST_ASSERT(CELL_pop_from_list(cell_list, &returned_cell));
    TEST_ASSERT_EQUAL_MEMORY(&underpopulated_cell_from_board, 
        &returned_cell,
        sizeof(struct cell_t));
}
