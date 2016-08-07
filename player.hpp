#pragma once
class Player {
	public:
		virtual ~Player(){};
		virtual bool move(bool kbhit, char c, GameBoard & tetrisGameBoard, std::shared_ptr<Piece>& curPiecep)  =0;
};

