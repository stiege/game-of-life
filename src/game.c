#include "game.h"
#include "stdio.h"
#include "stdlib.h"
#include "strings.h"

struct game_board_t
{
    unsigned int size;
    char * board_string;
};

#if 0
struct cell_t
{

};

struct cell_list_t
{
    struct cell_t * list;
    unsigned int length;
};
#endif

struct game_board_t * GME_init(unsigned int _size)
{
    unsigned int string_size = _size * (_size + 1 ) + 1;
    struct game_board_t * game_board = malloc(sizeof(struct game_board_t));
    game_board->board_string = malloc(string_size * sizeof(char));
    memset(game_board->board_string,' ',string_size * sizeof(char));
    unsigned int offset = _size;
    for (int i = 0; i < _size; ++i)
    {
        game_board->board_string[offset] = '\n';
        offset += _size + 1;
    }
    offset -= _size;

    game_board->board_string[offset] = '\0';
    game_board->size = _size;
    return game_board;
}

char * GME_show(struct game_board_t * board)
{
    return board->board_string;
}

void GME_set(struct game_board_t * board, struct point_t point)
{
    unsigned int offset = point.x + point.y * (board->size + 1);
    board->board_string[offset] = 'o';
}

void GME_iterate(struct game_board_t * board)
{
#if 0
    cells * living_cells = cells_from_board(board->board_string);
    cells * dying_cells = find_underpopulation_cells(living_cells);
    cells * new_board = remove_cells(living_cells, dying_cells);
    return board_from_cells(new_board);
#endif
}