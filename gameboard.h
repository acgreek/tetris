#ifndef GAMEBOARD_HPP
#define GAMEBOARD_HPP
#include <vector>
#include <algorithm>

#define MAXBOARDH 1000
#define MAXBOARDW 1000

#define CLEAR_BLOCK ' '

class GameBoard {
    public:
        GameBoard(int max_x, int max_y) {
            board_.resize(max_y);
            std::for_each(board_.begin(), board_.end(), [&max_x] (std::vector<char> & row) {row.resize(max_x,CLEAR_BLOCK);});
        }
        bool isPopulated(int x, int y) const{
            return (CLEAR_BLOCK != board_[y][x] );
        }
        void populated(int x, int y, char what) {
            board_[y][x]= what;
        }
        void empty(int x, int y) {
            board_[y][x]= CLEAR_BLOCK;
        }
        size_t maxY() const {
            return board_.size();
        }
        size_t maxX() const {
            return board_[0].size();
        }
    private:
        std::vector<std::vector <char>> board_;
};

#endif
