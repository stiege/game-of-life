#ifndef game_H
#define game_H

#include "cell.h"

struct point_t
{
    unsigned int x;
    unsigned int y;
};

struct new_unit_t
{
    cell_list_t * p_cell_list;
    unsigned int x;
    unsigned int y;
};

struct game_board_t * GME_init(unsigned int size);
char * GME_show(struct game_board_t * board);
void GME_set(struct game_board_t * board, struct point_t point);
void GME_iterate(struct game_board_t * board);
void GME_game_board_dtor(struct game_board_t * game_board);
void GME_deinit(void);

void GME_add_unit(struct game_board_t * board, struct new_unit_t * p_unit);

#endif // game_H
