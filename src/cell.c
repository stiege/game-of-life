#include "cell.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "stdbool.h"
#include "project.h"
#include "stdlib.h"
#include "list.h"

struct underpopulated_arg_t
{
    cell_list_t * underpopulated_list;
    cell_list_t * this_list;
};

struct count_neighbour_arg_t
{
    struct cell_t * center_cell;
    unsigned int * neighbour_count;
};

struct add_birth_cells_to_list_arg_t
{
    cell_list_t * this_list;
    cell_list_t * birth_list;
};

struct count_duplicates_arg_t
{
    unsigned int * duplicate_count;
    cell_list_t * current_cell;
};

struct find_birth_cells_args_t
{
    cell_list_t * this_list;
    cell_list_t * birth_list;
};

static void get_underpopulated (void * cell_list,
    void * args);
static bool cells_are_close(struct cell_t * cell_a, struct cell_t * cell_b);
static bool uints_are_close(unsigned int a, unsigned int b);
static int cell_compare(const void * cell_a, const void * cell_b);
static void find_birth_cells(void * list, void * birth_list);
static unsigned int count_repeated(cell_list_t * list, unsigned int n_repeated);
static void add_birth_cells_to_list(
    void * birth_list,
    void * list);
static void add_neighbours(void * cell_list,
    void * args);
static void count_neighbours(void * element, void * count);
static void count_duplicates(void * element, void * _args);

/*Not to be modified, "const" but assigned at runtime*/
static struct list_interface_t * list_interface;

void CELL_init(void)
{
    list_interface = (struct list_interface_t *)LIST_factory_ctor(
        sizeof(struct cell_t));
}

void CELL_deinit(void)
{
    LIST_factory_dtor(list_interface);
}

cell_list_t * CELL_list_from_string(char const * string)
{
    unsigned int cell_count = 0;
    char const * board_place = string;
    cell_list_t * cell_list = list_interface->ctor(list_interface);

    board_place = string;
    unsigned int x = 0;
    unsigned int y = 0;
    unsigned int list_offset = 0;
    while('\0' != *board_place)
    {
        if ('o' == *board_place)
        {
            struct cell_t new_cell = {.x = x, .y = y};
            list_interface->add(cell_list, &new_cell);
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

    return cell_list;
}

void CELL_list_dtor(cell_list_t * cell_list)
{
    list_interface->dtor(cell_list);
}

unsigned int CELL_get_list_length(cell_list_t * cell_list)
{
    return list_interface->get_length(cell_list);
}

bool CELL_pop_from_list(cell_list_t * cell_list, struct cell_t * cell)
{
    bool ret_val = false;
    if (NULL == cell_list)
    {
        UNSPECIFIED();
    }
    else if (0 == list_interface->get_length(cell_list))
    {
    }
    else
    {
        ret_val = list_interface->pop(cell_list, cell);
    }

    return ret_val;
}

cell_list_t * CELL_filter_for_underpopulated(cell_list_t * cells)
{
    cell_list_t * underpopulated_cells = list_interface->ctor(list_interface);
    if (NULL == cells)
    {

    }
    else
    {
        /*create new list for underpopulated*/
        struct underpopulated_arg_t args = {
            .underpopulated_list = underpopulated_cells,
            .this_list = cells};

        list_interface->iterate(
            cells, get_underpopulated, &args);
    }

    return underpopulated_cells;
}

cell_list_t * CELL_filter_for_births(cell_list_t * cells)
{
    cell_list_t * ret_val = list_interface->ctor(list_interface);
    if (NULL == cells)
    {
        UNSPECIFIED();
    }
    else
    {
        cell_list_t * big_list = list_interface->ctor(list_interface);
        list_interface->iterate(cells, add_neighbours, big_list);
        struct find_birth_cells_args_t find_birth_cells_args =
        {
            .this_list = big_list,
            .birth_list = ret_val
        };
        list_interface->iterate(big_list,find_birth_cells,&find_birth_cells_args);
        list_interface->dtor(big_list);
    }

    return ret_val;
}

static void add_neighbours(void * _cell,
    void * _args)
{
    struct cell_t cell = *(struct cell_t *)_cell;
    cell_list_t * list = _args;
    struct cell_t add_cell = { .x = cell.x - 1,.y = cell.y - 1};
    list_interface->add(list, &add_cell);
    add_cell= (struct cell_t){ .x = cell.x    ,.y = cell.y - 1};
    list_interface->add(list, &add_cell);
    add_cell= (struct cell_t){ .x = cell.x + 1,.y = cell.y - 1};
    list_interface->add(list, &add_cell);
    add_cell= (struct cell_t){ .x = cell.x - 1,.y = cell.y    };
    list_interface->add(list, &add_cell);
    add_cell= (struct cell_t){ .x = cell.x + 1,.y = cell.y    };
    list_interface->add(list, &add_cell);
    add_cell= (struct cell_t){ .x = cell.x - 1,.y = cell.y + 1};
    list_interface->add(list, &add_cell);
    add_cell= (struct cell_t){ .x = cell.x    ,.y = cell.y + 1};
    list_interface->add(list, &add_cell);
    add_cell= (struct cell_t){ .x = cell.x + 1,.y = cell.y + 1};
    list_interface->add(list, &add_cell);
}

/*Iterated function*/
static void get_underpopulated(void * cell,
    void * _args)
{
    /*For each cell count its neighbours and if < 3 then add it to the
    underpopulated list*/
    struct underpopulated_arg_t * args = _args;
    unsigned int neighbour_count = 0;
    struct count_neighbour_arg_t count_neighbours_arg = 
    {
        .center_cell = cell,
        .neighbour_count = &neighbour_count
    };

    list_interface->iterate(args->this_list,
        count_neighbours,
        &count_neighbours_arg);

    if (neighbour_count < 3)
    {
        list_interface->add(args->underpopulated_list, cell);
    }

}

static void count_neighbours(void * cell, void * _arg)
{
    struct count_neighbour_arg_t * arg = _arg;

    if (cells_are_close(cell, arg->center_cell))
    {
        *arg->neighbour_count += 1;
    }
}

static bool cells_are_close(struct cell_t * cell_a, struct cell_t * cell_b)
{
    bool ret_val = false;
    if (    uints_are_close(cell_a->x, cell_b->x) 
        &&  uints_are_close(cell_a->y,cell_b->y)
        &&  (memcmp(cell_a, cell_b, sizeof(struct cell_t)))
        )
    {//close but not the same
        ret_val = true;
    }

    return ret_val;
}

static bool uints_are_close(unsigned int a, unsigned int b)
{
    bool ret_val = false;
    unsigned int difference = abs((int)((long)a - (long)b));
    if (difference < 2)
    {
        ret_val = true;
    }

    return ret_val;
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

static void find_birth_cells(void * element,
    void * _args)
{
    struct find_birth_cells_args_t * args = _args;
    /*Iterate through list*/
    list_interface->iterate(args->this_list,
        add_birth_cells_to_list,
        args);
}

static void add_birth_cells_to_list(void * _cell, void * _args)
{
    /*For each cell iterate through again and find replicates*/
    struct cell_t * cell = _cell;
    unsigned int duplicate_count = 0;
    struct add_birth_cells_to_list_arg_t * args = _args;
    struct count_duplicates_arg_t count_duplicates_arg = 
    {
        .duplicate_count = &duplicate_count,
        .current_cell = _cell
    };
    /*need to pass in duplicate_count and current cell and iterate over the
    current list*/
    list_interface->iterate(args->this_list, 
        count_duplicates, &count_duplicates_arg);

    if (duplicate_count == 2)
    {
        if (!list_interface->contains(args->birth_list, cell))
        {
            list_interface->add(args->birth_list, cell);
        }
    }
}

static void count_duplicates(void * _cell, void * _args)
{
    struct cell_t * cell = _cell;
    struct count_duplicates_arg_t * args = _args;
    if (
        (!memcmp(
        cell, 
        args->current_cell, 
        sizeof(struct cell_t))))
    {
        *args->duplicate_count += 1;
    }
}
