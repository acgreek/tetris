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
		bool move(__attribute__((unused)) bool kbhit, char c, GameBoard & tetrisGameBoard, std::shared_ptr<Piece>& curOtherPiecep) {
			time_t now = time(NULL);
			if (last == now) {
				return false;
			}
			c = random() % 6;
			switch (c){
				case 0: curOtherPiecep->up(tetrisGameBoard);break;
				case 1: curOtherPiecep->down(tetrisGameBoard);break;
				case 2: curOtherPiecep->left(tetrisGameBoard);break;
				case 3: curOtherPiecep->rotateCounterClockwise(tetrisGameBoard);break;
				case 4: curOtherPiecep->rotateClockwise(tetrisGameBoard);break;
				case 5:
					 while (!curOtherPiecep->done_moving()) {
						curOtherPiecep->move(tetrisGameBoard);
					}
					break;
			}
			last=now;
			return true;
		}
	private:
		time_t last;
};
