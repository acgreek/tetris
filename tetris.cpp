#include <ncurses.h>
#include <unistd.h>
#include <list>
#include <algorithm>
#include <string.h>
#include "keyboard.h"
#define DELAY 60000

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
    while(1) {
        clear();
        std::for_each(blocks.begin(), blocks.end(), [](Block & b) {b.draw();});
        refresh();
        usleep(DELAY);
        /*
        next_x = x + direction;
        if (next_x >= max_x || next_x < 0) {
            direction*= -1;
        } else {
            x+= direction;
        }
        */
        Block &b = blocks.back();
        if (b.done_moving()) {
            blocks.push_back(Block(0,0));
        }
        b.move();

    }
    endwin();
}
