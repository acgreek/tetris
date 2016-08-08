#ifndef PIECE_HPP
#define PIECE_HPP
class Piece {
    public:
        Piece():dir_(HORIZONAL) {}
        virtual ~Piece() {}
        virtual void construct(blist & blocks,int offset=0) = 0 ;

        void unmark(GameBoard &gb) {
            blist::iterator eitr = litr_;
            eitr++;
            std::for_each(sitr_, eitr, [&](Block & b) {b.unmark(gb);});
        }
        void mark(GameBoard &gb) {
            blist::iterator eitr = litr_;
            eitr++;
            std::for_each(sitr_, eitr, [&](Block & b) {b.mark(gb);});
        }

        virtual void move(GameBoard &gb) {
            blist::iterator eitr = litr_;
            eitr++;
            unmark(gb);
            if (eitr == std::find_if_not(sitr_, eitr, [&](Block &b) {return b.canMove(gb);})) {
                std::for_each(sitr_, eitr, [&](Block & b) {b.uncheckedMove();});
                hasMoved = true;
            }
            else {
                sitr_->stopMoving();
            }
            mark(gb);
        }
        virtual bool down(GameBoard &gb) {
            bool result = false;
            blist::iterator eitr = litr_;
            eitr++;
            std::for_each(sitr_, eitr, [&](Block & b) {b.unmark(gb);});
            if (eitr == std::find_if_not(sitr_, eitr, [&](Block &b) {return b.canMoveDown(gb);})) {
                std::for_each(sitr_, eitr, [&](Block & b) {b.uncheckedMoveDown();});
                result = true;
            }
            std::for_each(sitr_, eitr, [&](Block & b) {b.mark(gb);});
            return result;
        }
        virtual bool up(GameBoard &gb) {
            bool result = false;
            blist::iterator eitr = litr_;
            eitr++;
            std::for_each(sitr_, eitr, [&](Block & b) {b.unmark(gb);});
            if (eitr == std::find_if_not(sitr_, eitr, [&](Block &b) {return b.canMoveUp(gb);})) {
                std::for_each(sitr_, eitr, [&](Block & b) {b.uncheckedMoveUp();});
                result= true;
            }
            std::for_each(sitr_, eitr, [&](Block & b) {b.mark(gb);});
            return result;
        }
        virtual bool left(GameBoard &gb) {
            bool result = false;
            blist::iterator eitr = litr_;
            eitr++;
            std::for_each(sitr_, eitr, [&](Block & b) {b.unmark(gb);});
            if (eitr == std::find_if_not(sitr_, eitr, [&](Block &b) {return b.canMoveLeft(gb);})) {
                std::for_each(sitr_, eitr, [&](Block & b) {b.uncheckedMoveLeft();});
                result =true;
            }
            std::for_each(sitr_, eitr, [&](Block & b) {b.mark(gb);});
            return result;
        }

        virtual bool right(GameBoard &gb) {
            bool result = false;
            blist::iterator eitr = litr_;
            eitr++;
            std::for_each(sitr_, eitr, [&](Block & b) {b.unmark(gb);});
            if (eitr == std::find_if_not(sitr_, eitr, [&](Block &b) {return b.canMoveRight(gb);})) {
                std::for_each(sitr_, eitr, [&](Block & b) {b.uncheckedMoveRight();});
                result =true;
            }
            std::for_each(sitr_, eitr, [&](Block & b) {b.mark(gb);});
            return result;
        }
        bool done_moving() {
            bool result = findIf([](Block &b)->bool {return b.done_moving();});
            if (result)
                forEachBlock([](Block &b){ b.stopMoving(); });
            return result;

        }
        void unmarkAll(GameBoard &gb) {
            forEachBlock(std::bind(&Block::unmark, std::placeholders::_1, std::ref(gb)));
        }
        void markAll(GameBoard &gb) {
            forEachBlock(std::bind(&Block::mark, std::placeholders::_1, std::ref(gb)));
        }
        void forEachBlock(std::function<void(Block&)> func) {
            blist::iterator eitr = litr_;
            eitr++;
            std::for_each(sitr_, eitr, func);
        }
        bool findIf(std::function<bool(Block&)> func) {
            blist::iterator eitr = litr_;
            eitr++;
            return eitr != std::find_if(sitr_, eitr, func);
        }
        virtual void rotateClockwise(GameBoard &gb) {
            const blist::iterator centerItr = getCenter();
            int cx = centerItr->x_;
            int cy = centerItr->y_;
            unmarkAll(gb);
            /*
             * -1,-1 -> 1 -1
             *  0,-1 -> 1,0
             *  1, -1 - >1,1
             *  1,0  ->0,1
             *  1,1 -> -1,1
             *  0,1 -> -1,0
             *  -1,1 ->-1,-1
             *  -1,0 ->0,-1
            */
            std::vector<Modification> modifications;
            if (canMove(gb,cx-1,cy-1,cx+1,cy-1,modifications) &&
                    canMove(gb,cx,cy-1,cx+1,cy,modifications) &&
                    canMove(gb,cx+1,cy-1,cx+1,cy+1,modifications) &&
                    canMove(gb,cx+1,cy,cx,cy+1,modifications) &&
                    canMove(gb,cx+1,cy+1,cx-1,cy+1,modifications) &&
                    canMove(gb,cx,cy+1,cx-1,cy,modifications) &&
                    canMove(gb,cx-1,cy+1,cx-1,cy-1,modifications) &&
                    canMove(gb,cx-1,cy,cx,cy-1,modifications)) {
                std::for_each(modifications.begin(),modifications.end(), [](Modification &m) {m.blockp->x_ = m.nx; m.blockp->y_ = m.ny; });
            }
            markAll(gb);
            return;
        }
        virtual void rotateCounterClockwise(GameBoard & gb) {
            const blist::iterator centerItr = getCenter();
            int cx = centerItr->x_;
            int cy = centerItr->y_;
            unmarkAll(gb);
            std::vector<Modification> modifications;
            if (canRMove(gb,cx-1,cy-1,cx+1,cy-1,modifications) &&
                    canRMove(gb,cx,cy-1,cx+1,cy,modifications) &&
                    canRMove(gb,cx+1,cy-1,cx+1,cy+1,modifications) &&
                    canRMove(gb,cx+1,cy,cx,cy+1,modifications) &&
                    canRMove(gb,cx+1,cy+1,cx-1,cy+1,modifications) &&
                    canRMove(gb,cx,cy+1,cx-1,cy,modifications) &&
                    canRMove(gb,cx-1,cy+1,cx-1,cy-1,modifications) &&
                    canRMove(gb,cx-1,cy,cx,cy-1,modifications)) {
                std::for_each(modifications.begin(),modifications.end(), [](Modification &m) {m.blockp->x_ = m.nx; m.blockp->y_ = m.ny; });
            }
            markAll(gb);
            return;
        }

    protected:
        struct Modification {
            Block * blockp;
            int nx;
            int ny;
        };
        bool canRMove(GameBoard & gb, int ix,int iy, int dx, int dy,std::vector<Modification> &mods) const {
            return canMove(gb,dx,dy, ix,iy, mods);
        }
        bool canMove(GameBoard & gb, int ix,int iy, int dx, int dy,std::vector<Modification> &mods) const {
            if (dy < 0 || dy > gb.maxy()-1)
                return false;
            blist::iterator eitr = litr_;
            eitr++;
            blist::iterator foundItr =std::find_if(sitr_, eitr, [&ix,&iy](Block &b)->bool {return b.x_ == ix && b.y_ == iy;})  ;
            if (eitr == foundItr)
                return true;
            if (gb.isClear(dx,dy)) {
                Modification m;
                m.blockp = &*foundItr;
                m.nx = dx;
                m.ny = dy;
                mods.push_back(m);
                return true;
            }
            return false;
        }
        blist::iterator getCenter() {
            blist::iterator citr = sitr_;
            citr++;
            citr++;
            return citr;
        }

    public:
        blist::iterator sitr_;
        blist::iterator litr_;
        bool hasMoved =false;

    protected:
        enum {HORIZONAL, VERTICAL,UHORIZONAL, RVERTICAL} dir_;

};

#endif
