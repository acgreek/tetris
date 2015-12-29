#include <ncurses.h>
#include <unistd.h>
#include <list>
#include <algorithm>
#include <string.h>
#include "keyboard.h"
int delay = 90000;

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
    private:
        int x_;
        int y_;
        int dx_;
        int dy_;
};

typedef std::list<Block> blist;
static blist blocks;

class Piece {
    public:
        Piece() {}
        virtual ~Piece() {}
        virtual void construct() = 0 ;

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
        blist::iterator sitr_;
        blist::iterator litr_;
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
    private:
        int offset_;

};

int main(int argc, char *argv[]) {
    initscr();
    noecho();
    curs_set(FALSE);
    memset(board, ' ', sizeof(board[0][0]) * MAXBOARDW* MAXBOARDH);
    getmaxyx(stdscr, max_y, max_x);
    //blocks.push_back(Block(0,0));
    LogPiece logPeace;

    logPeace.construct();
    bool done= false;
    while(!done) {
        clear();
        logPeace.move();
        if (kbhit()) {
            char c = lgetch();
            switch (c){
                case 'q': done=true;break;
                case 'a': logPeace.left();break;
                case 'd': logPeace.right();break;
            }
        }
        std::for_each(blocks.begin(), blocks.end(), [](Block & b) {b.draw();});
        refresh();
        usleep(delay);
        if (logPeace.done_moving()) {
            logPeace = LogPiece();
            logPeace.construct();
        }

    }
    endwin();
}




/*
        next_x = x + direction;
        if (next_x >= max_x || next_x < 0) {
            direction*= -1;
        } else {
            x+= direction;
        }
        */
