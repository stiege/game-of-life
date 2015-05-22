#include "unity.h"
#include "cell.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_returns_cells_from_string(void)
{
    struct cell_t cell = {.x=1,.y=1};
    cell_list_t * cell_list = CELL_list_from_string(        
        "   \n"
        " o \n"
        "   \n");
    TEST_ASSERT_EQUAL_INT( 1 , CELL_get_list_length(cell_list));

    struct cell_t * returned_cell = CELL_pop_from_list(cell_list);
    TEST_ASSERT_NOT_NULL(returned_cell);
    TEST_ASSERT_EQUAL_MEMORY(&cell, 
        returned_cell,
        sizeof(struct cell_t));
}
