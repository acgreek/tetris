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
            max_x_ = max_x;
            max_y_ = max_y;
            memset(board, CLEAR_BLOCK, sizeof(board[0][0]) * MAXBOARDW* MAXBOARDH);

        }
        int maxx() {return max_x_;}
        int maxy() {return max_y_;}
        void clear(int x, int y) {
            board[x][y] = CLEAR_BLOCK;
        }
        bool isClear(int x, int y) {
            return board[x][y] == CLEAR_BLOCK;
        }
        void set(int x, int y, char what) {
            board[x][y] = what;
        }
    private:
        char board[MAXBOARDW][MAXBOARDH];
        int  max_y_ = 0, max_x_ = 0;

};

#endif
