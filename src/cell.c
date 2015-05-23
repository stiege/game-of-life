#include "cell.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "stdbool.h"

struct cell_list_t
{
    struct cell_t * list;
    unsigned int length;
};

static bool cell_is_underpopulated (
    struct cell_list_t * cell_list,
    struct cell_t cell);
static unsigned int living_neighbours (
    struct cell_list_t * cell_list,
    struct cell_t cell);
static bool cells_are_close(struct cell_t cell_a, struct cell_t cell_b);
static bool uints_are_close(unsigned int a, unsigned int b);

cell_list_t * CELL_list_from_string(char const * string)
{
    unsigned int cell_count = 0;
    char const * board_place = string;
    while('\0' != *board_place)
    {
        if ('o' == *board_place)
        {
            cell_count++;
        }
        board_place++;
    }

    cell_list_t * cell_list = malloc(sizeof(cell_list_t));
    cell_list->list = malloc(cell_count * sizeof(struct cell_t));
    cell_list->length = cell_count;

    board_place = string;
    unsigned int x = 0;
    unsigned int y = 0;
    unsigned int list_offset = 0;
    while('\0' != *board_place)
    {
        if ('o' == *board_place)
        {
            cell_list->list[list_offset] = (struct cell_t){.x = 1, .y = 1};
            list_offset++;
        }
        else if ('\n')
        {
            y++;
            x = (unsigned int)-1;
        }
        x++;
        board_place++;
    }

    return cell_list;
}

unsigned int CELL_get_list_length(cell_list_t * cell_list)
{
    return cell_list->length;
}

struct cell_t * CELL_pop_from_list(cell_list_t * cell_list)
{
    struct cell_t * ret_val = NULL;
    if (0 == cell_list->length)
    {

    }
    else
    {
        unsigned int new_list_size = (cell_list->length - 1) 
            * sizeof(struct cell_t);
        struct cell_t * new_list = malloc(new_list_size);
        memcpy(new_list, cell_list->list, new_list_size);

        struct cell_t * popped_value = malloc(sizeof(struct cell_t));

        *popped_value = cell_list->list[new_list_size];
        free(cell_list->list);
        cell_list->list = new_list;
        cell_list->length = new_list_size;

        ret_val = popped_value;
    }

    return ret_val;
}

cell_list_t * CELL_filter_for_underpopulated(cell_list_t * cells)
{
    cell_list_t * ret_val = NULL;
    if (NULL == cells)
    {

    }
    else
    {
        unsigned int underpopulated_count = 0;
        for (unsigned int a = 0; a < cells->length; a++)
        {
            if (cell_is_underpopulated(cells,cells->list[a]))
            {
                underpopulated_count++;
            }
        }

        cell_list_t * underpopulated_cells = malloc(sizeof(cell_list_t));
        underpopulated_cells->list = 
            malloc(sizeof(struct cell_t) * underpopulated_count);
        underpopulated_cells->length = 0;

        for (unsigned int a = 0; a < cells->length; a++)
        {
            if (cell_is_underpopulated(cells,cells->list[a]))
            {
                underpopulated_cells->list[underpopulated_cells->length] 
                    = cells->list[a];
                underpopulated_cells->length++;
            }
        }
        ret_val = underpopulated_cells;
    }

    return ret_val;
}

static bool cell_is_underpopulated(struct cell_list_t * cell_list,
    struct cell_t cell)
{
    bool ret_val = false;
    if (3 > living_neighbours(cell_list,cell))
    {
        ret_val = true;
    }
    return ret_val;
}

static unsigned int living_neighbours(struct cell_list_t * cell_list,
    struct cell_t cell)
{
    unsigned int ret_val = 0;
    for (unsigned int a = 0; a < cell_list->length; a++)
    {
        if (cells_are_close(cell, cell_list->list[a]))
        {
            ret_val++;
        }
    }
    return ret_val;
}

static bool cells_are_close(struct cell_t cell_a, struct cell_t cell_b)
{
    bool ret_val = false;
    if (    uints_are_close(cell_a.x, cell_b.x) 
        &&  uints_are_close(cell_a.y,cell_b.y)
        &&  !(memcmp(&cell_a, &cell_b, sizeof(struct cell_t)))
        )
    {//close but not the same
        ret_val = true;
    }

    return ret_val;
}

static bool uints_are_close(unsigned int a, unsigned int b)
{
    unsigned int greatest = 0;
    unsigned int least = 0;
    bool ret_val = false;

    if (a <= b)
    {
        greatest = b;
        least = a;
    }
    else
    {
        greatest = a;
        least=b;
    }

    if (greatest - least < 2)
    {
        ret_val = true;
    }

    return ret_val;
}