#ifndef cell_H
#define cell_H

#include "stdbool.h"

struct cell_t
{
    unsigned int x;
    unsigned int y;
};

typedef struct cell_list_t cell_list_t;

cell_list_t * CELL_list_from_string(char const * string);
cell_list_t * CELL_filter_for_underpopulated(cell_list_t * cells);
unsigned int CELL_get_list_length(cell_list_t * cell_list);
bool CELL_pop_from_list(cell_list_t * cell_list, struct cell_t * cell);
// void CELL_remove_from_list(cell_list_t * cell_list, cell_list_t * to_remove);



#endif // cell_H
