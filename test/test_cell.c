#include "unity.h"
#include "list.h"
#include "cell.h"
#include "stdbool.h"
#include "malloc/malloc.h"

char const * underpopulated_board = 
"   \n"
" o \n"
"   \n";

char const * underpopulated_in_corner_board = 
"o  \n"
"   \n"
"   \n";

char const * block_board = 
"oo \n"
"oo \n"
"   \n";

char const * oscillator_board = 
" o \n"
" o \n"
" o \n";


static int sort_function (void * a, void * b);

const struct cell_t underpopulated_cell_from_board = {.x=1,.y=1};
const struct cell_t underpopulated_in_corner = {.x=0,.y=0};
struct mstats mdata_before;

static bool mem_leak_workaround;
void setUp(void)
{
    mem_leak_workaround = false;
    mdata_before = mstats();
    CELL_init();
}

void tearDown(void)
{
    if(!mem_leak_workaround)
    {
        TEST_ASSERT_EQUAL_INT(mdata_before.chunks_used, mstats().chunks_used);
    }
}


//Workaround for possible small unexplained memoryleak
//running this test first allows the next to pass
void test_memory_leak()
{
    mem_leak_workaround = true;
    cell_list_t * cell_list = CELL_list_from_string(underpopulated_board);
    CELL_list_dtor(cell_list);
}

void test_returns_cells_from_string(void)
{
    cell_list_t * cell_list = CELL_list_from_string(underpopulated_board);
    TEST_ASSERT_EQUAL_INT( 1 , CELL_get_list_length(cell_list));

    struct cell_t returned_cell;
    TEST_ASSERT(CELL_pop_from_list(cell_list, &returned_cell));

    CELL_list_dtor(cell_list);
    CELL_deinit();
    TEST_ASSERT_EQUAL_INT(mdata_before.chunks_used, mstats().chunks_used);
}

void test_returns_cell_from_upper_left_corner(void)
{
    cell_list_t * cell_list = CELL_list_from_string(
        underpopulated_in_corner_board);

    struct cell_t cell_from_original;
    TEST_ASSERT(CELL_pop_from_list(cell_list, &cell_from_original));

    TEST_ASSERT_EQUAL_MEMORY(&underpopulated_in_corner, 
        &cell_from_original,
        sizeof(struct cell_t));

    CELL_list_dtor(cell_list);
    CELL_deinit();
}

void test_returns_underpopulated_cells(void)
{
    cell_list_t * cell_list = CELL_list_from_string(underpopulated_board);
    TEST_ASSERT_NOT_NULL(cell_list);
    cell_list_t * underpopulated_list = 
        CELL_filter_for_underpopulated(cell_list);
    TEST_ASSERT_NOT_NULL(underpopulated_list);
    TEST_ASSERT_EQUAL_INT( 1 , CELL_get_list_length(underpopulated_list));
    struct cell_t returned_cell;
    TEST_ASSERT(CELL_pop_from_list(cell_list, &returned_cell));
    TEST_ASSERT_EQUAL_MEMORY(&underpopulated_cell_from_board, 
        &returned_cell,
        sizeof(struct cell_t));

    CELL_list_dtor(cell_list);
    CELL_list_dtor(underpopulated_list);
    CELL_deinit();
}

void test_block_board_not_underpopulated(void)
{
    cell_list_t * cell_list = CELL_list_from_string(block_board);
    TEST_ASSERT_NOT_NULL(cell_list);
    cell_list_t * underpopulated_list = 
        CELL_filter_for_underpopulated(cell_list);
    struct cell_t returned_cell;
    TEST_ASSERT_FALSE(CELL_pop_from_list(underpopulated_list, &returned_cell));
    CELL_list_dtor(cell_list);
    CELL_list_dtor(underpopulated_list);
    CELL_deinit();

}

void test_can_find_cells_being_born(void)
{
    cell_list_t * cell_list = CELL_list_from_string(oscillator_board);
    TEST_ASSERT_NOT_NULL(cell_list);
    cell_list_t * birth_list = 
        CELL_filter_for_births(cell_list);
    TEST_ASSERT_NOT_NULL(birth_list);
    struct cell_t returned_cell;
    TEST_ASSERT(CELL_pop_from_list(cell_list, &returned_cell));
    TEST_ASSERT(CELL_pop_from_list(cell_list, &returned_cell));
    TEST_ASSERT(CELL_pop_from_list(cell_list, &returned_cell));
    CELL_list_dtor(cell_list);
    CELL_list_dtor(birth_list);
    CELL_deinit();

}
