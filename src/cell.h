#ifndef cell_H
#define cell_H


struct cell_t
{
    unsigned int x;
    unsigned int y;
};

typedef struct cell_list_t cell_list_t;

cell_list_t * CELL_list_from_string(char const * string);
cell_list_t * CELL_filter_for_underpopulated(cell_list_t * cells);
unsigned int CELL_get_list_length(cell_list_t * cell_list);
struct cell_t * CELL_pop_from_list(cell_list_t * cell_list);


#endif // cell_H
