class SquarePiece :public Piece {
    public:
        SquarePiece() {
        }
        virtual void construct(blist & blocks,int offset = 0) {
            dir_=HORIZONAL;
            blocks.push_back(Block(offset + 0,0));
            sitr_ = blocks.end();
            --sitr_;
            blocks.push_back(Block(offset + 1,0));
            blocks.push_back(Block(offset + 0,1));
            blocks.push_back(Block(offset + 1,1));
            litr_ = blocks.end();
            --litr_;
        }
        virtual void rotateClockwise() {
        }
        virtual void rotateCounterClockwise() {
        }
};
class LZPiece :public Piece {
    public:
        LZPiece() {
            offset_ = 0;
        }
        virtual void construct(blist & blocks,int offset=0) {
            dir_=HORIZONAL;
            blocks.push_back(Block(offset + 0,1));
            sitr_ = blocks.end();
            --sitr_;
            blocks.push_back(Block(offset + 1,1));
            blocks.push_back(Block(offset + 1,0));
            blocks.push_back(Block(offset + 2,0));
            litr_ = blocks.end();
            --litr_;
        }
    private:
        int offset_;
};
class LEl:public Piece {
    public:
        LEl() {
            offset_ = 0;
        }
        virtual void construct(blist & blocks,int offset = 0) {
            dir_=HORIZONAL;
            blocks.push_back(Block(offset + 0,0));
            sitr_ = blocks.end();
            --sitr_;
            blocks.push_back(Block(offset + 0,1));
            blocks.push_back(Block(offset + 1,1));
            blocks.push_back(Block(offset + 2,1));
            litr_ = blocks.end();
            --litr_;
        }
    private:
        int offset_;
};
class El:public Piece {
    public:
        El() {
            offset_ = 0;
        }
        virtual void construct(blist & blocks,int offset = 0) {
            dir_=HORIZONAL;
            blocks.push_back(Block(offset + 0,1));
            sitr_ = blocks.end();
            --sitr_;
            blocks.push_back(Block(offset + 0,0));
            blocks.push_back(Block(offset + 1,0));
            blocks.push_back(Block(offset + 2,0));
            litr_ = blocks.end();
            --litr_;
        }
    private:
        int offset_;
};
class Pyramid :public Piece {
    public:
        Pyramid() { }
        virtual void construct(blist & blocks,int offset = 0 ) {
            dir_=HORIZONAL;
            blocks.push_back(Block(offset + 0,1));
            sitr_ = blocks.end();
            --sitr_;
            blocks.push_back(Block(offset + 1,0));
            blocks.push_back(Block(offset + 1,1));
            blocks.push_back(Block(offset + 2,1));
            litr_ = blocks.end();
            --litr_;
        }
};
class ZPiece :public Piece {
    public:
        ZPiece() { }
        virtual void construct(blist & blocks,int offset=0) {
            dir_=HORIZONAL;
            blocks.push_back(Block(offset + 0,0));
            sitr_ = blocks.end();
            --sitr_;
            blocks.push_back(Block(offset + 1,0));
            blocks.push_back(Block(offset + 1,1));
            blocks.push_back(Block(offset + 2,1));
            litr_ = blocks.end();
            --litr_;
        }
};
