#ifndef game_H
#define game_H

struct game_board_t * GME_init(unsigned int size);
char * GME_show(struct game_board_t * board);
void GME_set(struct game_board_t * board,
    unsigned int x,
    unsigned int y);
void GME_iterate(struct game_board_t * board);

#endif // game_H
