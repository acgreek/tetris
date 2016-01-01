#ifndef LOGPIECE
#define LOGPIECE

class LogPiece :public Piece {
    public:
        LogPiece() {
            offset_ = 0;
        }
        virtual void construct() {
            dir_=HORIZONAL;
            blocks.push_back(Block(offset_ + 0,0));
            sitr_ = blocks.end();
            --sitr_;
            blocks.push_back(Block(offset_ + 1,0));
            blocks.push_back(Block(offset_ + 2,0));
            blocks.push_back(Block(offset_ + 3,0));
            litr_ = blocks.end();
            --litr_;
        }
        virtual void rotateCounterClockwise() {
            return rotateClockwise();
        }
        virtual void rotateClockwise() {
            unmarkAll();
            if (dir_ == HORIZONAL) {
                if (sitr_->y_ > 0 && CLEAR_BLOCK != board[sitr_->x_+1][sitr_->y_-1]){
                    markAll();
                    return;
                }
                if (sitr_->y_ + 2 > max_y || CLEAR_BLOCK != board[sitr_->x_+1][sitr_->y_+1]) {
                    markAll();
                    return;
                }
                if (CLEAR_BLOCK != board[sitr_->x_+1][sitr_->y_+2]) {
                    markAll();
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
                if (sitr_->x_ == 0 || CLEAR_BLOCK != board[sitr_->x_-1][sitr_->y_+1]) {
                    markAll();
                    return;
                }
                if (sitr_->x_+2 > max_x || CLEAR_BLOCK != board[sitr_->x_+1][sitr_->y_+1]) {
                    markAll();
                    return;
                }
                if (CLEAR_BLOCK != board[sitr_->x_+2][sitr_->y_+1]) {
                    markAll();
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
            markAll();
        }
    private:
        int offset_;
};
#endif
