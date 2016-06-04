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

static void shiftRowsDown(GameBoard & gb,blist & blocks, int row) {
    std::for_each(blocks.begin(), blocks.end(), [&row, &gb ](Block & b) {if (b.y_ == row ){  b.unmark(gb);};});
    blocks.erase(std::remove_if(blocks.begin(), blocks.end(), [&row](Block & b)->bool { return b.y_ == row;}), blocks.end()  );
    std::for_each(blocks.begin(), blocks.end(), [&row, &gb ](Block & b) {if (b.y_ < row ){ b.unmark(gb); b.y_++; b.mark(gb);};});
    std::for_each(blocks.begin(), blocks.end(), [&row, &gb ](Block & b) {b.mark(gb);});
}
#define MIN(A,B) (A >B ? B :A)
static int checkCompleteRows(GameBoard & gb, blist & blocks) {
    int rowsRemoved=0;
    int i=gb.maxy();
    while (i > 0) {
        int j=0;
        for (; j<= gb.maxx();j++) {
            if (gb.isClear(j,i))
                break;
        }
        if (gb.maxx() +1 ==j) {
             rowsRemoved++;
             shiftRowsDown(gb,blocks, i);
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
        Window() {}
        WINDOW * get() { return w_.get(); }
        void set(WINDOW *w)   {
            w_.reset(w,delwin) ;
        }

        void refresh() {
            box(get(), 0 , 0);
            wnoutrefresh(get());

        }
    private:
        std::shared_ptr<WINDOW> w_;
};
class GameScreen {
    public:
        GameScreen():  cursesSetup() {
            curs_set(FALSE);
        }
        ~GameScreen() {

        }

        void getMaxyx(int &y, int &x) {
            getmaxyx(stdscr, y, x);
        }
        Window & getWindow(int height, int width, int yoffset, int xoffset){
            windows.push_back(Window());
            windows.back().set(create_newwin(height,width,yoffset,xoffset));
            return windows.back();
        }
        void refreshMain() {
            wnoutrefresh(stdscr);

        };
        void doupdate(){
            ::doupdate();
        }
    private:
        CursesSetup cursesSetup;
        std::list<Window> windows;

};

class TetrisGame {
    public:
        TetrisGame() {}
        void play() {
            GameScreen gameScreen;

            int real_max_x, real_max_y;
            gameScreen.getMaxyx(real_max_y, real_max_x);
            int centerX = real_max_x/2;
            int offsetCenterX = centerX + TETRIS_DEFAULT_WIDTH/2 -10;

            int max_x = MIN(real_max_x,TETRIS_DEFAULT_WIDTH);
            int max_y = real_max_y;
            Window & board_win = gameScreen.getWindow(max_y-4,12,0, offsetCenterX-1);
            Window & score_win = gameScreen.getWindow(3, 12, max_y-4, offsetCenterX-1);
            Window & next_piece_win = gameScreen.getWindow(5, 6, 2, offsetCenterX + 11);
            max_y = max_y-6;
            GameBoard tetrisGameBoard(max_x, max_y);

            bool done= false;
            int moveDownCount = 100;
            int currentCount = 1;
            PieceSelector  pieceSelector;
            std::shared_ptr<Piece> curPiecep(pieceSelector.getNextPiece(blocks_));
            std::shared_ptr<Piece> nextPiecep(pieceSelector.getNextPiece(nextblocks_));
            std::for_each(nextblocks_.begin(), nextblocks_.end(), [&](Block & b) {b.draw(next_piece_win.get(), "o", 1, 1);});
            while(!done) {
                bool needRedraw =false;
                if (currentCount % moveDownCount == 0 ) {
                    curPiecep->move(tetrisGameBoard);
                    needRedraw = true;
                }
                currentCount++;
                if (kbhit()) {
                    char c = lgetch();
                    needRedraw = true;
                    switch (c){
                        case 'q': done=true;break;
                        case 'a': curPiecep->left(tetrisGameBoard);break;
                        case 'd': curPiecep->right(tetrisGameBoard);break;
                        case 'z': curPiecep->rotateCounterClockwise(tetrisGameBoard);break;
                        case 'c': curPiecep->rotateClockwise(tetrisGameBoard);break;
                        case 's': curPiecep->move(tetrisGameBoard);break;
                    }
                }
                usleep(delay);
                if (needRedraw) {
                    clear();
                    wclear(board_win.get());
                    if (curPiecep->done_moving()) {
                        switch (checkCompleteRows(tetrisGameBoard, blocks_)) {
                            case 4: score_ +=100; break;
                            case 3: score_ +=50; break;
                            case 2: score_ +=25; break;
                            case 1: score_ +=10; break;
                            default:
                            case 0: score_ +=1; break;
                        }
                        std::for_each(nextblocks_.begin(), nextblocks_.end(), [&](Block & b) {b.draw(next_piece_win.get()," ", 1,1  );});
                        blocks_.splice(blocks_.begin(), nextblocks_);
                        curPiecep = std::move(nextPiecep);
                        nextPiecep.reset( pieceSelector.getNextPiece(nextblocks_));
                        std::for_each(nextblocks_.begin(), nextblocks_.end(), [&](Block & b) {b.draw(next_piece_win.get(), "o", 1, 1);});
                        curPiecep->markAll(tetrisGameBoard);
                    }
                    std::for_each(blocks_.begin(), blocks_.end(), [&](Block & b) {b.draw(board_win.get(),"o",1,1);});
                    gameScreen.refreshMain();
                    board_win.refresh();
                    mvwprintw(score_win.get(), 1, 1, "score: %d",score_);
                    score_win.refresh();
                    next_piece_win.refresh();

                    gameScreen.doupdate();
                }
            }
            std::cout << "final score:" <<  score_ << std::endl;
        }
    private:
        blist blocks_;
        blist nextblocks_;
        int score_ = 0;
};

int main() {
    /* initialize random seed: */
    srand (time(NULL));
    TetrisGame tetris;
    tetris.play();
};

