#ifndef cell_H
#define cell_H

#include "stdbool.h"
#include "list.h"

struct cell_t
{
    unsigned int x;
    unsigned int y;
};

typedef list_t cell_list_t;

struct cell_list_interface_t
{
    struct list_interface_t list_interface;

};

struct cell_list_interface_t * CELL_list_factory_ctor();
struct cell_list_interface_t * CELL_list_factory_ctor();


cell_list_t * CELL_list_from_string (char const * string);
cell_list_t * CELL_filter_for_underpopulated (cell_list_t * cells);
cell_list_t * CELL_filter_for_births (cell_list_t * cells);
unsigned int CELL_get_list_length (cell_list_t * cell_list);
bool CELL_pop_from_list (cell_list_t * cell_list, struct cell_t * cell);
void CELL_list_dtor (cell_list_t * cell_list);
void CELL_init(void);
void CELL_deinit(void);



#endif // cell_H
