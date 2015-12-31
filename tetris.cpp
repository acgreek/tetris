#include <ncurses.h>
#include <unistd.h>
#include <list>
#include <algorithm>
#include <string.h>
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
        void draw() {
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
            board[x_][y_] = 'o';
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
    protected:
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
        }
    private:
        int offset_;
};
class PieceSelector {
    public:
        PieceSelector(): currentPiece_(SQUARE) {}
        Piece & getNextPiece() {
            switch(currentPiece_) {
                case ZED:
                    currentPiece_ = LZED;
                    z_.construct();
                    return z_;
                case LZED:
                    currentPiece_ = LZED;
                    lz_.construct();
                    return lz_;
                case SQUARE:
                    currentPiece_ = LOG;
                    log_.construct();
                    return log_;
                default:
                case LOG:
                    currentPiece_ = ZED;
                    square_.construct();
                    return square_;
            }
        }
    private:
        enum {ZED, LZED, LOG, SQUARE} currentPiece_ ;
        ZPiece z_;
        LZPiece lz_;
        SquarePiece square_;
        LogPiece log_;
};

int main(int argc, char *argv[]) {
    initscr();
    noecho();
    curs_set(FALSE);
    memset(board, ' ', sizeof(board[0][0]) * MAXBOARDW* MAXBOARDH);
    getmaxyx(stdscr, max_y, max_x);

    bool done= false;
    int moveDownCount = 100;
    int currentCount = 1;
    PieceSelector  pieceSelector;
    Piece & curPiece =pieceSelector.getNextPiece();
    while(!done) {
        bool needRedraw =false;
        clear();
        if (currentCount % moveDownCount == 0 ) {
            curPiece.move();
            needRedraw = true;
        }
        currentCount++;
        if (kbhit()) {
            char c = lgetch();
            needRedraw = true;
            switch (c){
                case 'q': done=true;break;
                case 'a': curPiece.left();break;
                case 'd': curPiece.right();break;
                case 'z': curPiece.rotate();break;
                case 'c': curPiece.rotate();break;
                case 's': curPiece.move();break;
            }
        }
        usleep(delay);
        if (needRedraw) {
            std::for_each(blocks.begin(), blocks.end(), [](Block & b) {b.draw();});
            wnoutrefresh(stdscr);
            doupdate();
            if (curPiece.done_moving()) {
                curPiece =pieceSelector.getNextPiece();
            }
        }
    }
    endwin();
};

