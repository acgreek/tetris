class Player1: public Player {
	public:
		virtual ~Player1() {};
		bool move(bool kbhit, char c, GameBoard & tetrisGameBoard, std::shared_ptr<Piece>& curPiecep) {
			if (!kbhit) {
				return false;
			}
			switch (c){
				case 'w': curPiecep->up(tetrisGameBoard);break;
				case 's': curPiecep->down(tetrisGameBoard);break;
				case 'd': curPiecep->right(tetrisGameBoard);break;
				case 'z': curPiecep->rotateCounterClockwise(tetrisGameBoard);break;
				case 'c': curPiecep->rotateClockwise(tetrisGameBoard);break;
				case 'x':
						  while (!curPiecep->done_moving()) {
							  curPiecep->move(tetrisGameBoard);
						  }
			}
			return true;

		}
};
