#ifndef PIECE_SELECTOR_HPP
#define PIECE_SELECTOR_HPP

#include "piece.hpp"
#include "logpiece.hpp"
#include "pieces.hpp"
class PieceSelector {
    public:
        PieceSelector(int dx, int dy, int xoffset= 0, int yoffset=0) :dx_(dx), dy_(dy), xoffset_(xoffset), yoffset_(yoffset)
        {
               currentPiece_ = ( piece_type) (rand() % (LEL + 1));
        }
        PieceSelector():dx_(0), dy_(1), currentPiece_(SQUARE) {}
        Piece * getNextPiece(blist & blocks) {
            Piece * piece = getNextPieceNoColor(blocks);
            int cnum =rand() % (Block::getMaxColorNum()+1);
            Block::color_t color = Block::getEnumByNum(cnum);
            piece->forEachBlock([&](Block & b) {b.setColor(color);b.setDxDy(dx_,dy_); b.setX(b.getX() + xoffset_); b.setY(b.getY() + yoffset_);});
            return piece;
        }
        int getXoffset() {
            return xoffset_;
        }
    private:
        Piece * getNextPieceNoColor(blist & blocks) {
            piece_type nextPiece = ( piece_type) (rand() % (LEL + 1));
            switch(currentPiece_) {
                case ZED:
                    currentPiece_ = nextPiece;
                    z_.construct(blocks);
                    return new ZPiece(z_);
                case LZED:
                    currentPiece_ = nextPiece;
                    lz_.construct(blocks);
                    return new LZPiece(lz_);
                case LOG:
                    currentPiece_ = nextPiece;
                    log_.construct(blocks);
                    return new LogPiece(log_);
                case PYRAMID:
                    currentPiece_ = nextPiece;
                    pyramid_.construct(blocks);
                    return new Pyramid(pyramid_);
                case EL:
                    currentPiece_ = nextPiece;
                    el_.construct(blocks);
                    return new El(el_);
                case LEL:
                    currentPiece_ = nextPiece;
                    lel_.construct(blocks);
                    return new LEl(lel_);
                default:
                case SQUARE:
                    currentPiece_ = nextPiece;
                    square_.construct(blocks);
                    return new SquarePiece(square_);
            }
        }
        int dx_;
        int dy_;
        int xoffset_;
        int yoffset_;
        enum piece_type {ZED, LZED, LOG, SQUARE, PYRAMID, EL, LEL} currentPiece_ ;
        ZPiece z_;
        LZPiece lz_;
        SquarePiece square_;
        LogPiece log_;
        Pyramid pyramid_;
        El el_;
        LEl lel_;
};
#endif
