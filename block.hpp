#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <list>

#include "movable.hpp"
#include "vectorable.hpp"

class Block : public Movable, public Vectorable {
    public:

        typedef enum {RED, BLUE, ORANGE, GREEN, PURPLE} color_t;

        static int getMaxColorNum() {
            return 4;
        }
        static color_t getEnumByNum(int n) {
            return (color_t)(n%5);
        }

        Block() :x_(0), y_(0), color_(RED){dy_ = 1;dx_ = 0;}
        Block(int x, int y, color_t color=RED ) :x_(x), y_(y), color_(color){dy_ = 1;dx_ =0;}

        void setColor(color_t c) {color_ =c ;}
        color_t getColor() {return color_;}


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
        void set(GameBoard & gb, int x, int y) {
            unmark(gb);
            x_ = x;
            y_ = y;
            mark(gb);
        }
        void unmark(GameBoard & gb) {
            gb.clear(x_,y_);
        }
        void mark(GameBoard & gb) {
            gb.set(x_,y_,'o');
        }
        bool canmove(GameBoard & gb) {
            if (y_ == gb.maxy()-1 || !gb.isClear(dx_ + x_,y_ + dy_)){
                return false;
            }
            return true;
        }
        void uncheckedMove() {
            x_ += dx_;
            y_ += dy_;
        }
        void move(GameBoard & gb){
            unmark(gb);
            if (canmove(gb)) {
                uncheckedMove();
            }else   {
                stopMoving();
            }
           mark(gb);
        }
        bool canMoveLeft(GameBoard & gb) {
            return (!done_moving() && x_ > 0 && gb.isClear(x_-1,y_));
        }
        void stopMoving() {
            dx_= dy_ = 0;
        }
        void uncheckedMoveLeft(GameBoard & gb) {
            unmark(gb);
            x_--;
            mark(gb);
        }
        bool left(GameBoard & gb) {
            if (canMoveLeft(gb)) {
                uncheckedMoveLeft(gb);
                return true;
            };
            return false;
        }
        bool canMoveRight(GameBoard & gb) {
            return (!done_moving() && x_ < gb.maxx()&& gb.isClear(x_+1,y_));
        }
        void uncheckedMoveRight(GameBoard & gb) {
            unmark(gb);
            x_++;
            mark(gb);
        }
        bool right(GameBoard & gb) {
            if (canMoveRight(gb)) {
                uncheckedMoveRight(gb);
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
        color_t color_;
};

typedef std::list<Block> blist;

#endif
