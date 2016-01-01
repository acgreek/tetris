#include <ncurses.h>
#include <unistd.h>
#include <list>
#include <algorithm>
#include <string.h>
#include <math.h>
#include "keyboard.h"
int delay = 3000;

#define MAXBOARDH 1000
#define MAXBOARDW 1000

#define CLEAR_BLOCK ' '

static char board[MAXBOARDW][MAXBOARDH];

static int max_y = 0, max_x = 0;
class Movable {
    public:
        virtual void move() = 0;
        virtual bool left() = 0;
        virtual bool right() = 0;
        virtual bool done_moving() = 0;
};

class Block : public Movable {
    public:
        Block() :x_(0), y_(0){dy_ = 1;dx_ = 0;}
        Block(int x, int y) :x_(x), y_(y){dy_ = 1;dx_ =0;}
        void set(int x, int y) {
            unmark();
            x_ = x;
            y_ = y;
            mark();
        }
        void draw() {
            if (x_ >=0)
                mvprintw(y_, x_, "o");
        }
        void unmark() {
            board[x_][y_] = CLEAR_BLOCK;
        }
        void mark() {
            board[x_][y_] = 'o';
        }
        bool canmove() {
            if (y_ == max_y-1 || board[dx_ + x_][y_ + dy_] != CLEAR_BLOCK){
                return false;
            }
            return true;
        }
        void uncheckedMove() {
            x_ += dx_;
            y_ += dy_;
        }
        void move(){
            unmark();
            if (canmove()) {
                uncheckedMove();
            }else   {
                stopMoving();
            }
           mark();
        }
        bool canMoveLeft() {
            return (!done_moving() && x_ > 0 && board[x_-1][y_] == CLEAR_BLOCK);
        }
        void stopMoving() {

            dx_= dy_ = 0;
        }
        void uncheckedMoveLeft() {
            board[x_][y_]= CLEAR_BLOCK;
            x_--;
            board[x_][y_]='o';
        }
        bool left() {
            if (canMoveLeft()) {
                uncheckedMoveLeft();
                return true;
            };
            return false;
        }
        bool canMoveRight() {
            return (!done_moving() && x_ < max_x&& board[x_+1][y_] == CLEAR_BLOCK);
        }
        void uncheckedMoveRight() {
            board[x_][y_]=CLEAR_BLOCK;
            x_++;
            board[x_][y_]='o';
        }
        bool right() {
            if (canMoveRight()) {
                uncheckedMoveRight();
                return true;
            };
            return false;
        }
        bool done_moving() {
            return dy_ == 0 && dx_ == 0;
        }
        int x_;
        int y_;
    private:
        int dx_;
        int dy_;
};

typedef std::list<Block> blist;
static blist blocks;

class Piece {
    public:
        Piece():dir_(HORIZONAL) {}
        virtual ~Piece() {}
        virtual void construct() = 0 ;
        virtual void rotate() = 0;
        virtual void move() {
            blist::iterator eitr = litr_;
            eitr++;
            std::for_each(sitr_, eitr, [](Block & b) {b.unmark();});
            if (eitr == std::find_if_not(sitr_, eitr, [](Block &b) {return b.canmove();})) {
                std::for_each(sitr_, eitr, [](Block & b) {b.uncheckedMove();});
            }
            else {
                sitr_->stopMoving();

            }
            std::for_each(sitr_, eitr, [](Block & b) {b.mark();});
        }
        virtual bool left() {
            bool result = false;
            blist::iterator eitr = litr_;
            eitr++;
            std::for_each(sitr_, eitr, [](Block & b) {b.unmark();});
            if (eitr == std::find_if_not(sitr_, eitr, [](Block &b) {return b.canMoveLeft();})) {
                std::for_each(sitr_, eitr, [](Block & b) {b.uncheckedMoveLeft();});
                result =true;
            }
            std::for_each(sitr_, eitr, [](Block & b) {b.mark();});
            return result;
        }
        virtual bool right() {
            bool result = false;
            blist::iterator eitr = litr_;
            eitr++;
            std::for_each(sitr_, eitr, [](Block & b) {b.unmark();});
            if (eitr == std::find_if_not(sitr_, eitr, [](Block &b) {return b.canMoveRight();})) {
                std::for_each(sitr_, eitr, [](Block & b) {b.uncheckedMoveRight();});
                result =true;
            }
            std::for_each(sitr_, eitr, [](Block & b) {b.mark();});
            return result;
        }
        bool done_moving() {
            blist::iterator eitr = litr_;
            eitr++;
            return (eitr != std::find_if(sitr_, eitr, [](Block &b)->bool {return b.done_moving();}));
        }
        void unmarkAll() {
            blist::iterator eitr = litr_;
            eitr++;
            std::for_each(sitr_, eitr, [](Block & b) {b.unmark();});
        }
        void markAll() {
            blist::iterator eitr = litr_;
            eitr++;
            std::for_each(sitr_, eitr, [](Block & b) {b.unmark();});

        }
    protected:
        blist::iterator sitr_;
        blist::iterator litr_;
        enum {HORIZONAL, VERTICAL} dir_;
};

class LogPiece :public Piece {
    public:
        LogPiece() {
            offset_ = 0;
        }
        virtual void construct() {
            dir_=HORIZONAL;
            blocks.push_back(Block(offset_ + 0,0));
            sitr_ = blocks.end();
            --sitr_;
            blocks.push_back(Block(offset_ + 1,0));
            blocks.push_back(Block(offset_ + 2,0));
            blocks.push_back(Block(offset_ + 3,0));
            litr_ = blocks.end();
            --litr_;
        }
        virtual void rotate() {
            unmarkAll();
            if (dir_ == HORIZONAL) {
                if (sitr_->y_ > 0 && CLEAR_BLOCK != board[sitr_->x_+1][sitr_->y_-1]){
                    markAll();
                    return;
                }
                if (sitr_->y_ + 2 > max_y || CLEAR_BLOCK != board[sitr_->x_+1][sitr_->y_+1]) {
                    markAll();
                    return;
                }
                if (CLEAR_BLOCK != board[sitr_->x_+1][sitr_->y_+2]) {
                    markAll();
                    return;
                }
                blist::iterator citr = sitr_;
                citr->x_++;
                citr->y_--;
                citr++;citr++;
                citr->x_--;
                citr->y_++;
                citr++;
                citr->x_-=2;
                citr->y_+=2;
                dir_ = VERTICAL;
            }
            else {
                if (sitr_->x_ == 0 || CLEAR_BLOCK != board[sitr_->x_-1][sitr_->y_+1]) {
                    markAll();
                    return;
                }
                if (sitr_->x_+2 > max_x || CLEAR_BLOCK != board[sitr_->x_+1][sitr_->y_+1]) {
                    markAll();
                    return;
                }
                if (CLEAR_BLOCK != board[sitr_->x_+2][sitr_->y_+1]) {
                    markAll();
                    return;
                }
                blist::iterator citr = sitr_;
                citr->x_--;
                citr->y_++;
                citr++;citr++;
                citr->x_++;
                citr->y_--;
                citr++;
                citr->x_+=2;
                citr->y_-=2;
                dir_ = HORIZONAL;
            }
            markAll();
        }
    private:
        int offset_;
};
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
        virtual void rotate() {
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
        virtual void rotate() {
            unmarkAll();
            if (dir_ == HORIZONAL) {
                if (sitr_->y_-2 > 0 && CLEAR_BLOCK != board[sitr_->x_+1][sitr_->y_-2]){
                    markAll();
                    return;
                }
                if (sitr_->y_ + 1 > max_y || CLEAR_BLOCK != board[sitr_->x_+2][sitr_->y_+1]) {
                    markAll();
                    return;
                }
                blist::iterator citr = sitr_;
                citr->x_++;
                citr->y_-=2;
                citr++;
                citr->y_-=1;
                citr++;
                citr->x_++;
                citr++;
                citr->y_+=1;
                dir_ = VERTICAL;
            }
            else {
                if (sitr_->x_ == 0 || CLEAR_BLOCK != board[sitr_->x_-1][sitr_->y_+2]) {
                    markAll();
                    return;
                }
                if (sitr_->x_+2 > max_x || CLEAR_BLOCK != board[sitr_->x_+2][sitr_->y_+1]) {
                    markAll();
                    return;
                }
                blist::iterator citr = sitr_;
                citr->x_--;
                citr->y_+=2;
                citr--;
                citr->y_+=1;
                citr--;
                citr->x_--;
                citr--;
                citr->y_-=1;
                dir_ = HORIZONAL;
            }
            markAll();
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
        virtual void rotate() {
            unmarkAll();
            if (dir_ == HORIZONAL) {
                if (sitr_->y_ > 0 && CLEAR_BLOCK != board[sitr_->x_+1][sitr_->y_-1]){
                    markAll();
                    return;
                }
                if (sitr_->y_ + 2 > max_y || CLEAR_BLOCK != board[sitr_->x_+1][sitr_->y_+1]) {
                    markAll();
                    return;
                }
                if (CLEAR_BLOCK != board[sitr_->x_+1][sitr_->y_+2]) {
                    markAll();
                    return;
                }
                blist::iterator citr = sitr_;
                citr->x_+=2;
                citr->y_-=1;
                citr++;
                citr->x_+=1;
                citr++;
                citr->y_-=1;
                citr++;
                citr->x_-=1;
                dir_ = VERTICAL;
            }
            else {
                if (sitr_->x_ == 0 || CLEAR_BLOCK != board[sitr_->x_-1][sitr_->y_+1]) {
                    markAll();
                    return;
                }
                if (sitr_->x_+2 > max_x || CLEAR_BLOCK != board[sitr_->x_+1][sitr_->y_+1]) {
                    markAll();
                    return;
                }
                if (CLEAR_BLOCK != board[sitr_->x_+2][sitr_->y_+1]) {
                    markAll();
                    return;
                }
                blist::iterator citr = sitr_;
                citr->x_-=2;
                citr->y_+=1;
                citr++;
                citr->x_-=1;
                citr++;
                citr->y_+=1;
                citr++;
                citr->x_+=1;
                dir_ = HORIZONAL;
            }
            markAll();
        }
    private:
        int offset_;
};
class PieceSelector {
    public:
        PieceSelector(): currentPiece_(SQUARE) {}
        Piece * getNextPiece() {
            switch(currentPiece_) {
                case ZED:
                    currentPiece_ = LZED;
                    z_.construct();
                    return &z_;
                case LZED:
                    currentPiece_ = SQUARE;
                    lz_.construct();
                    return &lz_;
                case LOG:
                    currentPiece_ = ZED;
                    log_.construct();
                    return &log_;
                default:
                case SQUARE:
                    currentPiece_ = LOG;
                    square_.construct();
                    return &square_;
            }
        }
    private:
        enum {ZED, LZED, LOG, SQUARE} currentPiece_ ;
        ZPiece z_;
        LZPiece lz_;
        SquarePiece square_;
        LogPiece log_;
};
#include <iostream>
#include <fstream>
static void printBlocks(const char * name){
    std::ofstream myfile;
    myfile.open (name);
    std::for_each(blocks.begin(), blocks.end(), [&myfile](Block & b) {myfile << " " << b.x_ << "," << b.y_; });
    myfile << std::endl;
    for (int i=0; i < max_y; i++) {
        myfile << i << ":";
        for (int j=0; j < 10; j++) {
            char x[2];
            x[1] = '\0';
            x[0] = board[j][i];
            myfile << x;
            myfile.flush();
        }
        myfile << std::endl;
    }
    myfile.close();
}

static void shiftRowsDown(int row) {
    printBlocks("dump1");
    std::for_each(blocks.begin(), blocks.end(), [&row ](Block & b) {if (b.y_ == row ){  b.unmark();};});
    blocks.erase(std::remove_if(blocks.begin(), blocks.end(), [&row](Block & b)->bool { return b.y_ == row;}), blocks.end()  );
    printBlocks("dump2");
    std::for_each(blocks.begin(), blocks.end(), [&row ](Block & b) {if (b.y_ < row ){ b.unmark(); b.y_++; b.mark();};});
    std::for_each(blocks.begin(), blocks.end(), [&row ](Block & b) {b.mark();});
    printBlocks("dump3");
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
int main(int argc, char *argv[]) {
    int score = 0;
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
                case 'z': curPiecep->rotate();break;
                case 'c': curPiecep->rotate();break;
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
};

