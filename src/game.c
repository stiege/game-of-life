#include "game.h"
#include "stdio.h"
#include "stdlib.h"

struct game_board_t
{
    unsigned int size;
};

struct game_board_t * GME_init(unsigned int _size)
{
    struct game_board_t * game_board = malloc(sizeof(struct game_board_t));
    game_board->size = _size;
    return game_board;
}

char * GME_show(struct game_board_t * board)
{
    unsigned int string_size = board->size * (board->size + 2 ) + 1;
    char * result = malloc(string_size);
    char * current = result;
    for (int i = 0; i < board->size; ++i)
    {
        for (int j = 0; j < board->size; ++j)
        {
            *current++ = ' ';

        }
        *current++ = '\n';
    }
    *current = '\0';

    return result;
}