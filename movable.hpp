#ifndef MOVABLE_HPP
#define MOVABLE_HPP
#include "gameboard.h"
class Movable {
    public:
        virtual ~Movable(){  }
        virtual void down(GameBoard & gb) = 0;
        virtual void up(GameBoard & gb) = 0;
        virtual bool left(GameBoard & gb) = 0;
        virtual bool right(GameBoard & gb) = 0;
        virtual bool done_moving()const  = 0;
};
#endif
