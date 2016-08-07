#pragma once
class Player2 : public Player{
	public:
		virtual ~Player2() {};
		bool move(bool kbhit, char c, GameBoard & tetrisGameBoard, std::shared_ptr<Piece>& curOtherPiecep) {
			if (!kbhit) {
				return false;
			}
			switch (c){
				case 'i': curOtherPiecep->up(tetrisGameBoard);break;
				case 'k': curOtherPiecep->down(tetrisGameBoard);break;
				case 'j': curOtherPiecep->left(tetrisGameBoard);break;
				case 'm': curOtherPiecep->rotateCounterClockwise(tetrisGameBoard);break;
				case '.': curOtherPiecep->rotateClockwise(tetrisGameBoard);break;
				case ',':
						  while (!curOtherPiecep->done_moving()) {
							  curOtherPiecep->move(tetrisGameBoard);
						  }
						  break;
			}
			return true;
		}
};

