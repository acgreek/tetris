#pragma once

class Player {
	public:
		virtual ~Player(){};
		virtual bool move(bool kbhit, char c, GameBoard & tetrisGameBoard, std::shared_ptr<Piece>& curPiecep)  =0;
    protected:
        static void pushPieceDown(std::shared_ptr<Piece>& curPiecep,GameBoard & tetrisGameBoard) {
            while (!curPiecep->done_moving()) {
                curPiecep->move(tetrisGameBoard);
            }

        }
};
