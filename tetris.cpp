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

class SquarePiece :public Piece {
    public:
        SquarePiece() {
            offset_ = 0;
        }
        virtual void construct() {
            dir_=HORIZONAL;
            blocks.push_back(Block(offset_ + 0,0));
            sitr_ = blocks.end();
            --sitr_;
            blocks.push_back(Block(offset_ + 1,0));
            blocks.push_back(Block(offset_ + 0,1));
            blocks.push_back(Block(offset_ + 1,1));
            litr_ = blocks.end();
            --litr_;
        }
        virtual void rotateClockwise() {
        }
        virtual void rotateCounterClockwise() {
        }
    private:
        int offset_;
};
class LZPiece :public Piece {
    public:
        LZPiece() {
            offset_ = 0;
        }
        virtual void construct() {
            dir_=HORIZONAL;
            blocks.push_back(Block(offset_ + 0,1));
            sitr_ = blocks.end();
            --sitr_;
            blocks.push_back(Block(offset_ + 1,1));
            blocks.push_back(Block(offset_ + 1,0));
            blocks.push_back(Block(offset_ + 2,0));
            litr_ = blocks.end();
            --litr_;
        }
    private:
        int offset_;
};
class LEl:public Piece {
    public:
        LEl() {
            offset_ = 0;
        }
        virtual void construct() {
            dir_=HORIZONAL;
            blocks.push_back(Block(offset_ + 0,0));
            sitr_ = blocks.end();
            --sitr_;
            blocks.push_back(Block(offset_ + 0,1));
            blocks.push_back(Block(offset_ + 1,1));
            blocks.push_back(Block(offset_ + 2,1));
            litr_ = blocks.end();
            --litr_;
        }
    private:
        int offset_;
};
class El:public Piece {
    public:
        El() {
            offset_ = 0;
        }
        virtual void construct() {
            dir_=HORIZONAL;
            blocks.push_back(Block(offset_ + 0,1));
            sitr_ = blocks.end();
            --sitr_;
            blocks.push_back(Block(offset_ + 0,0));
            blocks.push_back(Block(offset_ + 1,0));
            blocks.push_back(Block(offset_ + 2,0));
            litr_ = blocks.end();
            --litr_;
        }
    private:
        int offset_;
};
class Pyramid :public Piece {
    public:
        Pyramid() {
            offset_ = 0;
        }
        virtual void construct() {
            dir_=HORIZONAL;
            blocks.push_back(Block(offset_ + 0,1));
            sitr_ = blocks.end();
            --sitr_;
            blocks.push_back(Block(offset_ + 1,0));
            blocks.push_back(Block(offset_ + 1,1));
            blocks.push_back(Block(offset_ + 2,1));
            litr_ = blocks.end();
            --litr_;
        }
    private:
        int offset_;
};
class ZPiece :public Piece {
    public:
        ZPiece() {
            offset_ = 0;
        }
        virtual void construct() {
            dir_=HORIZONAL;
            blocks.push_back(Block(offset_ + 0,0));
            sitr_ = blocks.end();
            --sitr_;
            blocks.push_back(Block(offset_ + 1,0));
            blocks.push_back(Block(offset_ + 1,1));
            blocks.push_back(Block(offset_ + 2,1));
            litr_ = blocks.end();
            --litr_;
        }
    private:
        int offset_;
};
class PieceSelector {
    public:
        PieceSelector(): currentPiece_(SQUARE) {}
        Piece * getNextPiece() {
            piece_type nextPiece = ( piece_type) (rand() % (LEL + 1));
            switch(currentPiece_) {
                case ZED:
                    currentPiece_ = nextPiece;
                    z_.construct();
                    return &z_;
                case LZED:
                    currentPiece_ = nextPiece;
                    lz_.construct();
                    return &lz_;
                case LOG:
                    currentPiece_ = nextPiece;
                    log_.construct();
                    return &log_;
                case PYRAMID:
                    currentPiece_ = nextPiece;
                    pyramid_.construct();
                    return &pyramid_;
                case EL:
                    currentPiece_ = nextPiece;
                    el_.construct();
                    return &el_;
                case LEL:
                    currentPiece_ = nextPiece;
                    lel_.construct();
                    return &lel_;
                default:
                case SQUARE:
                    currentPiece_ = nextPiece;
                    square_.construct();
                    return &square_;
            }
        }
    private:
        enum piece_type {ZED, LZED, LOG, SQUARE, PYRAMID, EL, LEL} currentPiece_ ;
        ZPiece z_;
        LZPiece lz_;
        SquarePiece square_;
        LogPiece log_;
        Pyramid pyramid_;
        El el_;
        LEl lel_;
};

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
            std::for_each(blocks.begin(), blocks.end(), [](Block & b) {b.draw();});
            wnoutrefresh(stdscr);
            doupdate();
        }
    }
    endwin();
    std::cout << "final score:" <<  score << std::endl;
};

