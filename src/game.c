#include "game.h"
#include "stdio.h"
#include "stdlib.h"
#include "strings.h"
#include "cell.h"
#include "project.h"

struct game_board_t
{
    unsigned int size;
    char * board_string;
};

static void remove_cell_from_board(struct game_board_t * board,
    struct cell_t cell);
static void add_cell_to_board(struct game_board_t * board,
    struct cell_t cell);

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

void GME_game_board_dtor(struct game_board_t * game_board)
{
    free(game_board->board_string);
    free(game_board);
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
    cell_list_t * living_cells = CELL_list_from_string(board->board_string);
    cell_list_t * dying_cells = CELL_filter_for_underpopulated(living_cells);
    cell_list_t * birth_cells = CELL_filter_for_births(living_cells);

    struct cell_t cell;

    while (CELL_pop_from_list(birth_cells, &cell))
    {
        add_cell_to_board(board,cell);
    }
    while (CELL_pop_from_list(dying_cells, &cell))
    {
        remove_cell_from_board(board, cell);
    }

    CELL_list_dtor(birth_cells);
    CELL_list_dtor(living_cells);
    CELL_list_dtor(dying_cells);
}

static void remove_cell_from_board(struct game_board_t * board,
    struct cell_t cell)
{
    char * board_cell = 
        &board->board_string[cell.y * (board->size + 1) + cell.x];
    if ('o' == *board_cell)
    {
        *board_cell = ' ';
    }
}

static void add_cell_to_board(struct game_board_t * board,
    struct cell_t cell)
{
    char * board_cell = 
        &board->board_string[cell.y * (board->size + 1) + cell.x];
    if (' ' == *board_cell)
    {
        *board_cell = 'o';
    }
}