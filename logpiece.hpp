#ifndef LOGPIECE
#define LOGPIECE

class LogPiece :public Piece {
    public:
        LogPiece() { }
        virtual void construct(blist & blocks,int offset=0) {
            dir_=HORIZONAL;
            blocks.push_back(Block(offset + 0,0));
            sitr_ = blocks.end();
            --sitr_;
            blocks.push_back(Block(offset + 1,0));
            blocks.push_back(Block(offset + 2,0));
            blocks.push_back(Block(offset + 3,0));
            litr_ = blocks.end();
            --litr_;
        }
        virtual void rotateCounterClockwise(GameBoard & gb) {
            return rotateClockwise(gb);
        }
        virtual void rotateClockwise(GameBoard & gb) {
            unmarkAll(gb);
            if (dir_ == HORIZONAL) {
                if (sitr_->y_ > 0 && !gb.isClear(sitr_->x_+1,sitr_->y_-1)){
                    markAll(gb);
                    return;
                }
                if (sitr_->y_ + 2 > gb.maxy() || !gb.isClear(sitr_->x_+1,sitr_->y_+1)) {
                    markAll(gb);
                    return;
                }
                if (!gb.isClear(sitr_->x_+1,sitr_->y_+2)) {
                    markAll(gb);
                    return;
                }
                blist::iterator citr = sitr_;
                citr->x_++;
                citr->y_--;
                citr++;citr++;
                citr->x_--;
                citr->y_++;
                citr++;
                citr->x_-=2;
                citr->y_+=2;
                dir_ = VERTICAL;
            }
            else {
                if (sitr_->x_ == 0 || !gb.isClear(sitr_->x_-1,sitr_->y_+1)) {
                    markAll(gb);
                    return;
                }
                if (sitr_->x_+2 > gb.maxx() || !gb.isClear(sitr_->x_+1,sitr_->y_+1)) {
                    markAll(gb);
                    return;
                }
                if (!gb.isClear(sitr_->x_+2,sitr_->y_+1)) {
                    markAll(gb);
                    return;
                }
                blist::iterator citr = sitr_;
                citr->x_--;
                citr->y_++;
                citr++;citr++;
                citr->x_++;
                citr->y_--;
                citr++;
                citr->x_+=2;
                citr->y_-=2;
                dir_ = HORIZONAL;
            }
            markAll(gb);
        }
};
#endif
