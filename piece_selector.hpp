
class PieceSelector {
    public:
        PieceSelector(): currentPiece_(SQUARE) {}
        void drawNextPiece(WINDOW * win) {
           mvwprintw(win, 1,1, "next piece") ;
        }
        Piece * getNextPiece() {
            piece_type nextPiece = ( piece_type) (rand() % (LEL + 1));
            switch(currentPiece_) {
                case ZED:
                    currentPiece_ = nextPiece;
                    z_.construct();
                    return &z_;
                case LZED:
                    currentPiece_ = nextPiece;
                    lz_.construct();
                    return &lz_;
                case LOG:
                    currentPiece_ = nextPiece;
                    log_.construct();
                    return &log_;
                case PYRAMID:
                    currentPiece_ = nextPiece;
                    pyramid_.construct();
                    return &pyramid_;
                case EL:
                    currentPiece_ = nextPiece;
                    el_.construct();
                    return &el_;
                case LEL:
                    currentPiece_ = nextPiece;
                    lel_.construct();
                    return &lel_;
                default:
                case SQUARE:
                    currentPiece_ = nextPiece;
                    square_.construct();
                    return &square_;
            }
        }
    private:
        enum piece_type {ZED, LZED, LOG, SQUARE, PYRAMID, EL, LEL} currentPiece_ ;
        ZPiece z_;
        LZPiece lz_;
        SquarePiece square_;
        LogPiece log_;
        Pyramid pyramid_;
        El el_;
        LEl lel_;
};

