#include "game.h"
#include "cell.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

int main(int argc, char const *argv[])
{
    struct game_board_t * board = GME_init(10);

    GME_set(board, (struct point_t) {.x = 1, .y = 0});
    GME_set(board, (struct point_t) {.x = 1, .y = 1});
    GME_set(board, (struct point_t) {.x = 1, .y = 2});

    while(true) 
    {
        printf("%s", GME_show(board));
        GME_iterate(board);
        sleep(1);
    }
    return 0;
}