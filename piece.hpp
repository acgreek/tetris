#ifndef PIECE_HPP
#define PIECE_HPP
class Piece {
    public:
        Piece():dir_(HORIZONAL) {}
        virtual ~Piece() {}
        virtual void construct(blist & blocks,int offset=0) = 0 ;

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
        void unmarkAll() {
            blist::iterator eitr = litr_;
            eitr++;
            std::for_each(sitr_, eitr, [](Block & b) {b.unmark();});
        }
        void markAll() {
            blist::iterator eitr = litr_;
            eitr++;
            std::for_each(sitr_, eitr, [](Block & b) {b.unmark();});

        }
        virtual void rotateClockwise() {
            const blist::iterator centerItr = getCenter();
            int cx = centerItr->x_;
            int cy = centerItr->y_;
            unmarkAll();
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
            if (canMove(cx-1,cy-1,cx+1,cy-1,modifications) &&
                    canMove(cx,cy-1,cx+1,cy,modifications) &&
                    canMove(cx+1,cy-1,cx+1,cy+1,modifications) &&
                    canMove(cx+1,cy,cx,cy+1,modifications) &&
                    canMove(cx+1,cy+1,cx-1,cy+1,modifications) &&
                    canMove(cx,cy+1,cx-1,cy,modifications) &&
                    canMove(cx-1,cy+1,cx-1,cy-1,modifications) &&
                    canMove(cx-1,cy,cx,cy-1,modifications)) {
                std::for_each(modifications.begin(),modifications.end(), [](Modification &m) {m.blockp->x_ = m.nx; m.blockp->y_ = m.ny; });
            }
            markAll();
            return;
        }
        virtual void rotateCounterClockwise() {
            const blist::iterator centerItr = getCenter();
            int cx = centerItr->x_;
            int cy = centerItr->y_;
            unmarkAll();
            std::vector<Modification> modifications;
            if (canRMove(cx-1,cy-1,cx+1,cy-1,modifications) &&
                    canRMove(cx,cy-1,cx+1,cy,modifications) &&
                    canRMove(cx+1,cy-1,cx+1,cy+1,modifications) &&
                    canRMove(cx+1,cy,cx,cy+1,modifications) &&
                    canRMove(cx+1,cy+1,cx-1,cy+1,modifications) &&
                    canRMove(cx,cy+1,cx-1,cy,modifications) &&
                    canRMove(cx-1,cy+1,cx-1,cy-1,modifications) &&
                    canRMove(cx-1,cy,cx,cy-1,modifications)) {
                std::for_each(modifications.begin(),modifications.end(), [](Modification &m) {m.blockp->x_ = m.nx; m.blockp->y_ = m.ny; });
            }
            markAll();
            return;
        }

    protected:
        struct Modification {
            Block * blockp;
            int nx;
            int ny;
        };
        bool canRMove(int ix,int iy, int dx, int dy,std::vector<Modification> &mods) {
            return canMove(dx,dy, ix,iy, mods);
        }
        bool canMove(int ix,int iy, int dx, int dy,std::vector<Modification> &mods) {
            blist::iterator eitr = litr_;
            eitr++;
            blist::iterator foundItr =std::find_if(sitr_, eitr, [&ix,&iy](Block &b)->bool {return b.x_ == ix && b.y_ == iy;})  ;
            if (eitr == foundItr)
                return true;
            if (CLEAR_BLOCK == board[dx][dy]) {
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
        blist::iterator sitr_;
        blist::iterator litr_;
        enum {HORIZONAL, VERTICAL,UHORIZONAL, RVERTICAL} dir_;

};

#endif
