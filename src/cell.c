#include "cell.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "stdbool.h"
#include "project.h"

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
static bool list_contains(cell_list_t * list, struct cell_t cell);

cell_list_t * CELL_list_from_string(char const * string)
{
    unsigned int cell_count = 0;
    char const * board_place = string;
    cell_list_t * ret_val = NULL;

    while('\0' != *board_place)
    {
        if ('o' == *board_place)
        {
            cell_count++;
        }
        board_place++;
    }

    if (cell_count)
    {
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
            if ('\n')
            {
                y++;
                x = (unsigned int)-1;
            }
            x++;
            board_place++;
        }
        ret_val = cell_list;
    }

    return ret_val;
}

void CELL_list_dtor(cell_list_t * cell_list)
{
    free(cell_list->list);
    free(cell_list);
}

unsigned int CELL_get_list_length(cell_list_t * cell_list)
{
    return cell_list->length;
}

bool CELL_pop_from_list(cell_list_t * cell_list, struct cell_t * cell)
{
    bool ret_val = false;
    if (NULL == cell_list)
    {
        UNSPECIFIED();
    }
    else if (0 == cell_list->length)
    {
    }
    else
    {
        unsigned int new_list_size = (cell_list->length - 1) 
            * sizeof(struct cell_t);
        struct cell_t * new_list = malloc(new_list_size);
        memcpy(new_list, cell_list->list, new_list_size);

        *cell = cell_list->list[new_list_size];
        free(cell_list->list);
        cell_list->list = new_list;
        cell_list->length = new_list_size;

        ret_val = true;
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

        if (underpopulated_count)
        {
            cell_list_t * underpopulated_cells = malloc(sizeof(cell_list_t));
            if (NULL == underpopulated_cells)
            {
                UNSPECIFIED();
            }
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
    }

    return ret_val;
}

#if 0
void CELL_remove_from_list(cell_list_t * cell_list, cell_list_t * to_remove)
{
    unsigned int size_of_new_list = cell_list->length - to_remove->length;
    cell_list_t new_list;
    new_list.list = malloc(size_of_new_list * sizeof(struct cell_t));
    new_list.length = 0;

    for (unsigned int i = 0; i < cell_list->length; i++)
    {
        if (!list_contains(to_remove, cell_list->list[i]))
        {
            new_list.list[new_list.length++]=cell_list->list[i];
        }
    }
}
#endif

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
        least = b;
    }

    if ((greatest - least) < 2)
    {
        ret_val = true;
    }

    return ret_val;
}

#if 0
static bool list_contains(cell_list_t * list, struct cell_t cell)
{
    bool ret_val = false;
    for (unsigned int i = 0; i < list->length; i++)
    {
        if (memcmp(&list->list[i], &cell, sizeof(struct cell_t)))
        {
            ret_val = true;
        }
    }
    return ret_val;
}
#endif