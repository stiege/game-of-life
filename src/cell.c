#include "cell.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "stdbool.h"
#include "project.h"
#include "stdlib.h"

struct cell_list_t
{
    struct cell_t * list;
    unsigned int length;
};

static bool cell_is_underpopulated(struct cell_list_t * cell_list,
    struct cell_t cell);
static unsigned int living_neighbours (
    struct cell_list_t * cell_list,
    struct cell_t cell);
static bool cells_are_close(struct cell_t cell_a, struct cell_t cell_b);
static bool uints_are_close(unsigned int a, unsigned int b);
static bool list_contains(cell_list_t * list, struct cell_t cell);
static void list_add(cell_list_t * list, struct cell_t cell);
static int cell_compare(const void * cell_a, const void * cell_b);
static cell_list_t * find_birth_cells(cell_list_t * list);

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
                cell_list->list[list_offset] = (struct cell_t){.x = x, .y = y};
                list_offset++;
            }
            else if ('\n' == *board_place)
            {
                y++;
                x = (unsigned int)-1;
            }
            else
            {
                if (' ' != *board_place)
                {
                    UNSPECIFIED();
                }
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
    if (NULL != cell_list)
    {
        free(cell_list->list);
        free(cell_list);
    }
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

        unsigned int new_list_length = cell_list->length - 1;
        unsigned int new_list_size = new_list_length * sizeof(struct cell_t);

        struct cell_t * new_list = malloc(new_list_size);
        memcpy(new_list, cell_list->list, new_list_size);

        *cell = cell_list->list[new_list_length];
        free(cell_list->list);
        cell_list->list = new_list;
        cell_list->length = new_list_length;

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

cell_list_t * CELL_filter_for_births(cell_list_t * cells)
{
    cell_list_t * ret_val = NULL;
    if (NULL == cells)
    {
        UNSPECIFIED();
    }
    else
    {
        cell_list_t big_list;
        big_list.length = cells->length * 8;
        big_list.list = malloc(sizeof(struct cell_t) * big_list.length);
        big_list.length = 0;
        for (unsigned int i = 0; i < cells->length; ++i)
        {
            list_add(&big_list, (struct cell_t)
                {   .x=cells->list[i].x-1,
                    .y=cells->list[i].y-1});
            list_add(&big_list, (struct cell_t)
                {   .x=cells->list[i].x,
                    .y=cells->list[i].y-1});
            list_add(&big_list, (struct cell_t)
                {   .x=cells->list[i].x+1,
                    .y=cells->list[i].y-1});
            list_add(&big_list, (struct cell_t)
                {   .x=cells->list[i].x-1,
                    .y=cells->list[i].y});
            list_add(&big_list, (struct cell_t)
                {   .x=cells->list[i].x+1,
                    .y=cells->list[i].y});
            list_add(&big_list, (struct cell_t)
                {   .x=cells->list[i].x-1,
                    .y=cells->list[i].y+1});
            list_add(&big_list, (struct cell_t)
                {   .x=cells->list[i].x,
                    .y=cells->list[i].y-1});
            list_add(&big_list, (struct cell_t)
                {   .x=cells->list[i].x+1,
                    .y=cells->list[i].y-1});
        }
        qsort(big_list.list,
            big_list.length,
            sizeof(struct cell_t),
            cell_compare);
        ret_val = find_birth_cells(&big_list);
        free(big_list.list);
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
        &&  (memcmp(&cell_a, &cell_b, sizeof(struct cell_t)))
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

static void list_add(cell_list_t * list, struct cell_t cell)
{
    list->list[list->length++] = cell;
}

static int cell_compare(const void * _cell_a, const void * _cell_b)
{
    int ret_val = 0;
    struct cell_t const * cell_b = _cell_b;
    struct cell_t const * cell_a = _cell_a;

    if (0 != (cell_a->y - cell_b->y))
    {
        ret_val = (cell_a->y - cell_b->y);
    }
    else
    {
        ret_val = (cell_a->x - cell_b->x);
    }

    return ret_val;
}

static cell_list_t * find_birth_cells(cell_list_t * list)
{
    cell_list_t * ret_val = NULL;
    unsigned int birth_cells = 0;
    for (unsigned int i = 0; i < list->length; ++i)
    {
        struct cell_t current_cell = list->list[i];
        unsigned int matching_cells = 0;
        while (!memcmp(
            &list->list[i + matching_cells], 
            &current_cell, 
            sizeof(struct cell_t)))
        {
            matching_cells++;
        }

        if (matching_cells > 2)
        {
            birth_cells++;
        }
    }

    if (0 < birth_cells)
    {
        cell_list_t * birth_list = malloc(sizeof(cell_list_t));
        birth_list->length = 0;
        birth_list->list = malloc(sizeof(struct cell_t) * birth_cells);
        for (unsigned int i = 0; i < list->length; ++i)
        {
            struct cell_t current_cell = list->list[i];
            unsigned int matching_cells = 0;
            while (!memcmp(
                &list->list[i + matching_cells], 
                &current_cell, 
                sizeof(struct cell_t)))
            {
                matching_cells++;
            }

            if (matching_cells > 2)
            {
                birth_list->list[birth_list->length++]=current_cell;
            }
        }
        ret_val = birth_list;
    }

    return ret_val;
}