#pragma once

class Player2 : public Player{
	public:
		virtual ~Player2() {};
		bool move(bool kbhit, char c, GameBoard & tetrisGameBoard, std::shared_ptr<Piece>& curPiecep) {
			if (!kbhit) {
				return false;
			}
			switch (c){
				case 'i': curPiecep->up(tetrisGameBoard);break;
				case 'k': curPiecep->down(tetrisGameBoard);break;
				case 'j': curPiecep->left(tetrisGameBoard);break;
				case 'm': curPiecep->rotateCounterClockwise(tetrisGameBoard);break;
				case '.': curPiecep->rotateClockwise(tetrisGameBoard);break;
				case ',': pushPieceDown (curPiecep,tetrisGameBoard); break;
			}
			return true;
		}
};
