#ifndef MOVABLE_HPP
#define MOVABLE_HPP
#include "gameboard.h"
class Movable {
    public:
        virtual void move(GameBoard & gb) = 0;
        virtual bool left(GameBoard & gb) = 0;
        virtual bool right(GameBoard & gb) = 0;
        virtual bool done_moving() = 0;
};
#endif
