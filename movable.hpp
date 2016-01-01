#ifndef MOVABLE_HPP
#define MOVABLE_HPP
class Movable {
    public:
        virtual void move() = 0;
        virtual bool left() = 0;
        virtual bool right() = 0;
        virtual bool done_moving() = 0;
};
#endif
