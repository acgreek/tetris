#include <ncurses.h>
#include <unistd.h>
#include <list>
#include <algorithm>
#include <string.h>
#include <math.h>
#include <iostream>
#include "keyboard.h"
int delay = 3000;

#define MAXBOARDH 1000
#define MAXBOARDW 1000

#define CLEAR_BLOCK ' '

static char board[MAXBOARDW][MAXBOARDH];

static int max_y = 0, max_x = 0;
#include "movable.hpp"
#include "block.hpp"

typedef std::list<Block> blist;
static blist blocks;

#include "piece.hpp"
#include "logpiece.hpp"
#include "pieces.hpp"
#include "piece_selector.hpp"

static void shiftRowsDown(int row) {
    std::for_each(blocks.begin(), blocks.end(), [&row ](Block & b) {if (b.y_ == row ){  b.unmark();};});
    blocks.erase(std::remove_if(blocks.begin(), blocks.end(), [&row](Block & b)->bool { return b.y_ == row;}), blocks.end()  );
    std::for_each(blocks.begin(), blocks.end(), [&row ](Block & b) {if (b.y_ < row ){ b.unmark(); b.y_++; b.mark();};});
    std::for_each(blocks.begin(), blocks.end(), [&row ](Block & b) {b.mark();});
}
#define MIN(A,B) (A >B ? B :A)
static int checkCompleteRows() {
    int rowsRemoved=0;
    int i=max_y;
    while (i > 0) {
        int j=0;
        for (; j<= max_x;j++) {
            if (board[j][i]==CLEAR_BLOCK)
                break;
        }
        if (max_x +1 ==j) {
             rowsRemoved++;
             shiftRowsDown(i);
        } else {
            i--;
        }
    }
    return rowsRemoved;
}
WINDOW *create_newwin(int height, int width, int starty, int startx)
{
    WINDOW *local_win;
    local_win = newwin(height, width, starty, startx);
    box(local_win, 0 , 0);/* 0, 0 gives default characters
                           * for the vertical and horizontal
                           *                   * lines*/
    wrefresh(local_win);/* Show that box */
    return local_win;
}


int main() {
    int score = 0;
     /* initialize random seed: */
    srand (time(NULL));
    initscr();
    noecho();
    curs_set(FALSE);
    memset(board, CLEAR_BLOCK, sizeof(board[0][0]) * MAXBOARDW* MAXBOARDH);

    getmaxyx(stdscr, max_y, max_x);
    max_x = MIN(max_x,9);
    WINDOW * score_win= create_newwin(3, 11, max_y-4, 0);
    WINDOW * next_piece_win= create_newwin(5, 5, 2, 11);
    max_y = max_y-4;

    bool done= false;
    int moveDownCount = 100;
    int currentCount = 1;
    PieceSelector  pieceSelector;
    Piece * curPiecep =pieceSelector.getNextPiece();
    while(!done) {
        bool needRedraw =false;
        clear();
        if (currentCount % moveDownCount == 0 ) {
            curPiecep->move();
            needRedraw = true;
        }
        currentCount++;
        if (kbhit()) {
            char c = lgetch();
            needRedraw = true;
            switch (c){
                case 'q': done=true;break;
                case 'a': curPiecep->left();break;
                case 'd': curPiecep->right();break;
                case 'z': curPiecep->rotateCounterClockwise();break;
                case 'c': curPiecep->rotateClockwise();break;
                case 's': curPiecep->move();break;
            }
        }
        usleep(delay);
        if (needRedraw) {
            if (curPiecep->done_moving()) {
                switch (checkCompleteRows()) {
                    case 4: score +=100; break;
                    case 3: score +=50; break;
                    case 2: score +=25; break;
                    case 1: score +=10; break;
                    default:
                    case 0: score +=1; break;
                }
                curPiecep =pieceSelector.getNextPiece();
                curPiecep->markAll();
            }
            std::for_each(blocks.begin(), blocks.end(), [](Block & b) {b.draw(stdscr);});
            wnoutrefresh(stdscr);
            box(score_win, 0 , 0);
            mvwprintw(score_win, 1, 1, "score: %d",score);
            wnoutrefresh(score_win);/* Show that box */
            box(next_piece_win, 0 , 0);
            pieceSelector.drawNextPiece(next_piece_win);
            wnoutrefresh(next_piece_win);/* Show that box */
            doupdate();
        }
    }
    endwin();
    std::cout << "final score:" <<  score << std::endl;
};

