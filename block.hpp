#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <list>

#include "movable.hpp"
#include "vectorable.hpp"

class Block : public Movable, public Vectorable {
    public:

        typedef enum {RED, BLUE, GREEN,WHITE, MAX_COLOR} color_t;

        static int getMaxColorNum() {
            return 4;
        }
        static color_t getEnumByNum(int n) {
            return (color_t)(n%MAX_COLOR);
        }

        Block() :x_(0), y_(0), color_(RED){dy_ = 1;dx_ = 0;}
        Block(int x, int y, color_t color=RED ) :x_(x), y_(y), color_(color){dy_ = 1;dx_ =0;}
        Block(int x, int y, int dx, int dy, color_t color=RED ) :x_(x), y_(y), color_(color),dy_(dy),dx_(dx){}

        Block(Block && other ) {
            this->x_ = other.x_;
            this->y_ = other.y_;
            this->dy_ = other.dy_;
            this->dx_ = other.dx_;
            this->color_ = other.color_;
        }
        Block& operator=(const Block& other) {
            this->x_ = other.x_;
            this->y_ = other.y_;
            this->dy_ = other.dy_;
            this->dx_ = other.dx_;
            this->color_ = other.color_;
            return *this;
        }

        void setColor(color_t c) {color_ =c ;}
        color_t getColor() {return color_;}

        void setDxDy(int dx, int dy) {dx_ =dx; dy_ =dy;}

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
        bool canMove(GameBoard & gb) {
            if ((1 == dy_ &&  y_ == gb.maxy()-1)  ||
                (-1 == dy_ &&  y_ == 0) ||
                (1 == dx_ &&  x_ == gb.maxx()-1)  ||
                (-1 == dx_ &&  x_ == 0) ||
                !gb.isClear(dx_ + x_,y_ + dy_)){
                return false;
            }
            return true;
        }
        void move(GameBoard & gb){
            unmark(gb);
            if (canMove(gb)) {
                uncheckedMove();
            }else   {
                stopMoving();
            }
           mark(gb);
        }

        void down(GameBoard & gb){
            unmark(gb);
            if (canMoveDown(gb)) {
                uncheckedMoveDown();
            }else   {
                if (1 == dy_)
                    stopMoving();
            }
           mark(gb);
        }
        void up(GameBoard & gb){
            unmark(gb);
            if (canMoveUp(gb)) {
                uncheckedMoveUp();
            }else   {
                if (-1 == dy_)
                    stopMoving();
            }
           mark(gb);
        }
        bool canMoveUp(GameBoard & gb) {
            return (!done_moving() && y_ > 0 && gb.isClear(x_,y_-1));
        }
        bool canMoveDown(GameBoard & gb) {
            if (y_ == gb.maxy()-1 || !gb.isClear(x_,y_+1)){
                return false;
            }
            return true;
        }
        bool canMoveLeft(GameBoard & gb) {
            return (!done_moving() && x_ > 0 && gb.isClear(x_-1,y_));
        }
        void stopMoving() {
            dx_= dy_ = 0;
        }
        void uncheckedMoveLeft() {
            x_--;
        }
        void uncheckedMove() {
            x_+= dx_;
            y_+= dy_;
        }
        bool left(GameBoard & gb) {
            if (canMoveLeft(gb)) {
                uncheckedMoveLeft();
                return true;
            } else {
                if (-1 == dx_)
                    stopMoving();
            }
            return false;
        }
        bool canMoveRight(GameBoard & gb) {
            return (!done_moving() && x_ < gb.maxx()&& gb.isClear(x_+1,y_));
        }
        void uncheckedMoveRight() {
            x_++;
        }
        void uncheckedMoveDown() {
            y_ += 1;
        }

        void uncheckedMoveUp() {
            y_ -= 1;
        }
        bool right(GameBoard & gb) {
            if (canMoveRight(gb)) {
                uncheckedMoveRight();
                return true;
            } else {
                if (1 == dx_)
                    stopMoving();
            }
            return false;
        }
        bool done_moving() {
            return dy_ == 0 && dx_ == 0;
        }
        int x_;
        int y_;
        int getDy() {
         return dy_;
        }
        int getDx() {
         return dx_;
        }
    private:
        color_t color_;
        int dy_;
        int dx_;
};

typedef std::list<Block> blist;

#endif
