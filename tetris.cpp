#include <ncurses.h>
#include <unistd.h>
#include <list>
#include <algorithm>
#include <string.h>
#include "keyboard.h"
int delay = 90000;

#define MAXBOARDH 1000
#define MAXBOARDW 1000

static char board[MAXBOARDW][MAXBOARDH];


static int max_y = 0, max_x = 0;

class Block {
    public:
        Block() :x_(0), y_(0){dy_ = 1;dx_ = 0;}
        Block(int x, int y) :x_(x), y_(y){dy_ = 1;dx_ =0;}
        void draw() {
            mvprintw(y_, x_, "o");
        }
        void move(){
            board[x_][y_] = ' ';
            int ny= y_, nx = x_;
            if (y_ == max_y-1) {
                dy_ = 0;
            }
            else {
               ny  = y_ + dy_;
            }
            if (board[nx][ny] != ' ') {
                dy_= 0;
                dx_= 0;
            }
            else {
                x_ = nx;
                y_ = ny;
            }
            board[x_][y_] = 'o';
        }
        bool left() {
            if (!done_moving() && x_ > 0 && board[x_-1][y_] == ' ') {
                board[x_][y_]=' ';
                x_--;
                board[x_][y_]='o';
                return true;
            };
            return false;
        }
        bool right() {
            if (!done_moving() && x_ < max_x&& board[x_+1][y_] == ' ') {
                board[x_][y_]=' ';
                x_++;
                board[x_][y_]='o';
                return true;
            };
            return false;
        }
        bool done_moving() {
            return dy_ == 0 && dx_ == 0;
        }
        int x_;
        int y_;
        int dx_;
        int dy_;
};

std::list<Block> blocks;



int main(int argc, char *argv[]) {
    initscr();
    noecho();
    curs_set(FALSE);
    memset(board, ' ', sizeof(board[0][0]) * MAXBOARDW* MAXBOARDH);
    getmaxyx(stdscr, max_y, max_x);
    blocks.push_back(Block(0,0));
    bool done= false;
    while(!done) {
        clear();
        Block &b = blocks.back();
        b.move();
        if (kbhit()) {
            char c = lgetch();
            switch (c){
                case 'q': done=true;break;
                case 'a': b.left();break;
                case 'd': b.right();break;
            }

        }
        std::for_each(blocks.begin(), blocks.end(), [](Block & b) {b.draw();});
        refresh();
        usleep(delay);
        if (b.done_moving()) {
            blocks.push_back(Block(0,0));
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
