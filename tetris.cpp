#include <ncurses.h>
#include <unistd.h>
#include <list>
#include <algorithm>
#include <string.h>
#include <math.h>
#include <iostream>
#include <memory>
#include "keyboard.h"
#include "gameboard.h"

int delay = 3000;

#include "movable.hpp"
#include "block.hpp"

typedef std::list<Block> blist;

#include "piece.hpp"
#include "logpiece.hpp"
#include "pieces.hpp"
#include "piece_selector.hpp"

#define TETRIS_DEFAULT_WIDTH 9

static void shiftRowsDown(blist & blocks, int row) {
    std::for_each(blocks.begin(), blocks.end(), [&row ](Block & b) {if (b.y_ == row ){  b.unmark();};});
    blocks.erase(std::remove_if(blocks.begin(), blocks.end(), [&row](Block & b)->bool { return b.y_ == row;}), blocks.end()  );
    std::for_each(blocks.begin(), blocks.end(), [&row ](Block & b) {if (b.y_ < row ){ b.unmark(); b.y_++; b.mark();};});
    std::for_each(blocks.begin(), blocks.end(), [&row ](Block & b) {b.mark();});
}
#define MIN(A,B) (A >B ? B :A)
static int checkCompleteRows(blist & blocks) {
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
             shiftRowsDown(blocks, i);
        } else {
            i--;
        }
    }
    return rowsRemoved;
}
WINDOW *create_newwin(int height, int width, int starty, int startx) {
    auto *local_win = newwin(height, width, starty, startx);
    box(local_win, 0 , 0);/* 0, 0 gives default characters
                           * for the vertical and horizontal
                           *                   * lines*/
    wrefresh(local_win);/* Show that box */
    return local_win;
}

class CursesSetup {
    public:
        CursesSetup() {
            initscr();
            noecho();

        }
        ~CursesSetup (){
            endwin();
        }

};

class Window {
    public:
        Window(WINDOW *w) : w_(w,delwin) {}
        WINDOW * get() { return w_.get(); }
    private:
        std::shared_ptr<WINDOW> w_;
};

int main() {
    blist blocks;
    blist nextblocks;
    int score = 0;
     /* initialize random seed: */
    srand (time(NULL));
    curs_set(FALSE);
    memset(board, CLEAR_BLOCK, sizeof(board[0][0]) * MAXBOARDW* MAXBOARDH);
    CursesSetup cursesSetup;

    getmaxyx(stdscr, max_y, max_x);
    int centerX = max_x/2;
    int offsetCenterX = centerX + TETRIS_DEFAULT_WIDTH/2 -10;

    max_x = MIN(max_x,TETRIS_DEFAULT_WIDTH);
    Window board_win(create_newwin(max_y-4,12,0, offsetCenterX-1));
    Window score_win(create_newwin(3, 12, max_y-4, offsetCenterX-1));
    Window next_piece_win(create_newwin(5, 6, 2, offsetCenterX + 11));
    max_y = max_y-6;
    GameBoard tetrisGameBoard(max_x, max_y);

    bool done= false;
    int moveDownCount = 100;
    int currentCount = 1;
    PieceSelector  pieceSelector;
    std::shared_ptr<Piece> curPiecep(pieceSelector.getNextPiece(blocks));
    std::shared_ptr<Piece> nextPiecep(pieceSelector.getNextPiece(nextblocks));
    std::for_each(nextblocks.begin(), nextblocks.end(), [&](Block & b) {b.draw(next_piece_win.get(), "o", 1, 1);});
    while(!done) {
        bool needRedraw =false;
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
            clear();
            wclear(board_win.get());
            if (curPiecep->done_moving()) {
                switch (checkCompleteRows(blocks)) {
                    case 4: score +=100; break;
                    case 3: score +=50; break;
                    case 2: score +=25; break;
                    case 1: score +=10; break;
                    default:
                    case 0: score +=1; break;
                }
                std::for_each(nextblocks.begin(), nextblocks.end(), [&](Block & b) {b.draw(next_piece_win.get()," ", 1,1  );});
                blocks.splice(blocks.begin(), nextblocks);
                curPiecep = std::move(nextPiecep);
                nextPiecep.reset( pieceSelector.getNextPiece(nextblocks));
                std::for_each(nextblocks.begin(), nextblocks.end(), [&](Block & b) {b.draw(next_piece_win.get(), "o", 1, 1);});
                curPiecep->markAll();
            }
            std::for_each(blocks.begin(), blocks.end(), [&](Block & b) {b.draw(board_win.get(),"o",1,1);});
            box(board_win.get(), 0 , 0);
            wnoutrefresh(stdscr);
            wnoutrefresh(board_win.get());
            box(score_win.get(), 0 , 0);
            mvwprintw(score_win.get(), 1, 1, "score: %d",score);
            wnoutrefresh(score_win.get());/* Show that box */
            box(next_piece_win.get(), 0 , 0);
            wnoutrefresh(next_piece_win.get());/* Show that box */
            doupdate();
        }
    }
    std::cout << "final score:" <<  score << std::endl;
};

