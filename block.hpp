#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "vectorable.hpp"

class Block : public Movable, public Vectorable {
    public:
        Block() :x_(0), y_(0){dy_ = 1;dx_ = 0;}
        Block(int x, int y) :x_(x), y_(y){dy_ = 1;dx_ =0;}
        int getX() const {
            return x_;
        }
        int getY() const {
            return y_;
        }
        int getZ() const  {
            return 0;
        }
        int setX(int x){
            return x_ = x;
        }
        int setY(int y){
            return y_ = y;
        }
        int setZ(__attribute__((unused))int z){
            return 0;
        }
        void set(int x, int y) {
            unmark();
            x_ = x;
            y_ = y;
            mark();
        }
        void draw(WINDOW * win, const char *symbol="o",  int drawoffsetx =0,int  drawoffsety= 0 ) {
            if (x_ >=0)
                mvwprintw(win, y_ + drawoffsety, x_+drawoffsetx, symbol);
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

#endif
