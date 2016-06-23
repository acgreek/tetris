#include <unistd.h>
#include <algorithm>
#include <memory>

#include "gameboard.h"
#include "block.hpp"
#include "piece_selector.hpp"

static void shiftRowsDown(GameBoard & gb,blist & blocks, int row) {
    std::for_each(blocks.begin(), blocks.end(), [&row, &gb ](Block & b) {if (b.y_ == row ){  b.unmark(gb);};});
    blocks.erase(std::remove_if(blocks.begin(), blocks.end(), [&row](Block & b)->bool { return b.y_ == row;}), blocks.end()  );
    std::for_each(blocks.begin(), blocks.end(), [&row, &gb ](Block & b) {if (b.y_ < row ){ b.unmark(gb); b.y_++; b.mark(gb);};});
    std::for_each(blocks.begin(), blocks.end(), [&row, &gb ](Block & b) {b.mark(gb);});
}

static int checkCompleteRows(GameBoard & gb, blist & blocks) {
    int rowsRemoved=0;
    int i=gb.maxy();
    while (i > 0) {
        int j=0;
        for (; j<= gb.maxx();j++) {
            if (gb.isClear(j,i))
                break;
        }
        if (gb.maxx() +1 ==j) {
             rowsRemoved++;
             shiftRowsDown(gb,blocks, i);
        } else {
            i--;
        }
    }
    return rowsRemoved;
}
static bool gameOver(std::shared_ptr<Piece> curPiecep) {
    return curPiecep->findIf([](Block &b)->bool {return b.getY() == 0;});
}

#include "gamescreen.hpp"

#define MIN(A,B) (A >B ? B :A)

class TetrisGame {
    public:
        TetrisGame(GameScreen_interface &gameScreen) : gameScreen_(gameScreen), delay_(gameScreen.getDelay()) {}
        bool handleKeyBoard(GameBoard & tetrisGameBoard, bool & done,std::shared_ptr<Piece>& curPiecep) {
            if (!gameScreen_.kbhit()) {
                return false;
            }
            char c = gameScreen_.lgetch();
            switch (c){
                case 'q': done=true;break;
                case 'a': curPiecep->left(tetrisGameBoard);break;
                case 'd': curPiecep->right(tetrisGameBoard);break;
                case 'z': curPiecep->rotateCounterClockwise(tetrisGameBoard);break;
                case 'c': curPiecep->rotateClockwise(tetrisGameBoard);break;
                case 's': curPiecep->move(tetrisGameBoard);break;
                case ' ':
                          while (!curPiecep->done_moving()) {
                              curPiecep->move(tetrisGameBoard);
                          }
                          break;
            }
            return true;

        }
        void play() {
            int real_max_x, real_max_y;
            gameScreen_.getMaxyx(real_max_y, real_max_x);
            int centerX = real_max_x/2;
            int offsetCenterX = centerX + TETRIS_DEFAULT_WIDTH/2 -10;
            int max_x = MIN(real_max_x,TETRIS_DEFAULT_WIDTH);
            int max_y = real_max_y;
            Window_interface & board_win = gameScreen_.getWindow(max_y-4,12,0, offsetCenterX-1);
            Window_interface & score_win = gameScreen_.getWindow(3, 12, max_y-4, offsetCenterX-1);
            Window_interface & next_piece_win = gameScreen_.getWindow(5, 6, 2, offsetCenterX + 11);
            max_y = max_y-6;
            GameBoard tetrisGameBoard(max_x, max_y);
            bool done= false;
            int moveDownCount = 100;
            int currentCount = 1;
            PieceSelector  pieceSelector;
            std::shared_ptr<Piece> curPiecep(pieceSelector.getNextPiece(blocks_));
            int moveRight = rand() %(TETRIS_DEFAULT_WIDTH -1);
            for (int i = 0; i < moveRight; i++) {
                curPiecep->right(tetrisGameBoard);
            }
            std::shared_ptr<Piece> nextPiecep(pieceSelector.getNextPiece(nextblocks_));
            std::for_each(nextblocks_.begin(), nextblocks_.end(), [&](Block & b) {next_piece_win.draw(b, 1, 1);});
            while(!done) {
                bool needRedraw =false;
                if (currentCount % moveDownCount == 0 ) {
                    curPiecep->move(tetrisGameBoard);
                    needRedraw = true;
                }
                currentCount++;
                needRedraw |= handleKeyBoard(tetrisGameBoard,done,curPiecep);
                usleep(delay_);
                if (needRedraw) {
                    gameScreen_.clear();
                    board_win.clear();
                    if (curPiecep->done_moving()) {
                        if (gameOver(curPiecep))
                            done = true;
                        switch (checkCompleteRows(tetrisGameBoard, blocks_)) {
                            case 4: score_ +=100; break;
                            case 3: score_ +=50; break;
                            case 2: score_ +=25; break;
                            case 1: score_ +=10; break;
                            default:
                            case 0: score_ +=1; break;
                        }
                        int moveRight = rand() %8;
                        for (int i = 0; i < moveRight; i++) {
                            nextPiecep->right(tetrisGameBoard);
                        }
                        next_piece_win.clear();
                        blocks_.splice(blocks_.begin(), nextblocks_);
                        curPiecep = std::move(nextPiecep);
                        nextPiecep.reset( pieceSelector.getNextPiece(nextblocks_));
                        std::for_each(nextblocks_.begin(), nextblocks_.end(), [&](Block & b) {next_piece_win.draw(b, 1, 1);});
                        curPiecep->markAll(tetrisGameBoard);
                    }
                    std::for_each(blocks_.begin(), blocks_.end(), [&](Block & b) {board_win.draw(b,1,1);});
                    gameScreen_.refreshMain();
                    board_win.refresh();
                    score_win.text(1, 1, "score: %d",score_);
                    score_win.refresh();
                    next_piece_win.refresh();
                    gameScreen_.doupdate();
                }
            }
        }
        int getScore() {
            return score_;
        }
    private:
        GameScreen_interface &gameScreen_;
        int delay_;
        blist blocks_;
        blist nextblocks_;
        int score_ = 0;
};

#include "ncurses_game_screen.hpp"
#include <iostream>

int main() {
    /* initialize random seed: */
    srand (time(NULL));
    NCurses::GameScreen gameScreen;
    TetrisGame tetris(gameScreen);
    tetris.play();
    std::cout << "final score:" <<  tetris.getScore() << std::endl;
};

