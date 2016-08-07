#pragma once

#include<time.h>
/**
 * every second, player will randomly move piece
 */
class RandomPlayer: public Player{
	public:
		RandomPlayer() {
			last= time(NULL);
		}
		virtual ~RandomPlayer() {};
		bool move(__attribute__((unused)) bool kbhit, char c, GameBoard & tetrisGameBoard, std::shared_ptr<Piece>& curPiecep) {
			time_t now = time(NULL);
			if (last == now) {
				return false;
			}
			c = random() % 6;
			switch (c){
				case 0: curPiecep->up(tetrisGameBoard);break;
				case 1: curPiecep->down(tetrisGameBoard);break;
				case 2: curPiecep->left(tetrisGameBoard);break;
				case 3: curPiecep->rotateCounterClockwise(tetrisGameBoard);break;
				case 4: curPiecep->rotateClockwise(tetrisGameBoard);break;
				case 5: pushPieceDown (curPiecep,tetrisGameBoard); break;
			}
			last=now;
			return true;
		}
	private:
		time_t last;
};
