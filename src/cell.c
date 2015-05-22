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

static bool cell_is_underpopulated(struct cell_t cell);

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
            if (cell_is_underpopulated(cells->list[a]))
            {
                underpopulated_count++;
            }
        }

        cell_list_t * underpopulated_cells = 
            malloc(sizeof(struct cell_t) * underpopulated_count);

        underpopulated_count = 0;
        for (unsigned int a = 0; a < cells->length; a++)
        {
            if (cell_is_underpopulated(cells->list[a]))
            {
                underpopulated_cells->list[underpopulated_count] 
                    = cells->list[a];
                underpopulated_count++;
            }
        }

    }

    return ret_val;
}

static bool cell_is_underpopulated(struct cell_t cell)
{
    return false;
}