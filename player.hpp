#pragma once

class Player {
	public:
		virtual ~Player(){};
		virtual bool move(bool kbhit, char c, GameBoard & tetrisGameBoard, std::shared_ptr<Piece>& curPiecep)  =0;
    protected:
        static void pushPieceDown(Piece * curPiecep,GameBoard & tetrisGameBoard) {
            while (!curPiecep->done_moving()) {
                curPiecep->move(tetrisGameBoard);
            }
        }
};

class KeyBoardPlayer: public Player {
	public:
        KeyBoardPlayer(char up, char down,char left, char right,char rcc,char rc, char drop) {
            setKey(up, [] (Piece *pp, GameBoard &gb) {pp->up(gb);});
            setKey(down, [] (Piece *pp, GameBoard &gb) {pp->down(gb);});
            setKey(left, [] (Piece *pp, GameBoard &gb) {pp->left(gb);});
            setKey(right, [] (Piece *pp, GameBoard &gb) {pp->right(gb);});
            setKey(rcc, [] (Piece *pp, GameBoard &gb) {pp->rotateCounterClockwise(gb);});
            setKey(rc, [] (Piece *pp, GameBoard &gb) {pp->rotateClockwise(gb);});
            setKey(drop, [] (Piece *pp, GameBoard &gb) {pushPieceDown(pp, gb);});
        };
		virtual ~KeyBoardPlayer() {};
		bool move(bool kbhit, char c, GameBoard & tetrisGameBoard, std::shared_ptr<Piece>& curPiecep) {
			if (!kbhit) {
				return false;
			}
            if (key_map_.count(c))
                key_map_[c](curPiecep.get(), tetrisGameBoard);
			return true;

		}
    private:
        void setKey(char key, std::function<void (Piece *, GameBoard &)> func) {
            if (key == 0)
                return ;
            key_map_[key] =func;

        }
        std::unordered_map<char,std::function<void (Piece *, GameBoard &)> >key_map_;
};

