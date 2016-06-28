#include <unistd.h>
#include <algorithm>
#include <memory>

#include "gameboard.h"
#include "block.hpp"
#include "piece_selector.hpp"

typedef enum { LEFT, RIGHT } direction_t;

static void shiftRowsDown(GameBoard & gb,blist & blocks, int column,direction_t dir) {
    std::for_each(blocks.begin(), blocks.end(), [&column, &gb ](Block & b) {if (b.x_ == column ){  b.unmark(gb);};});
    blist::iterator new_end = std::remove_if(blocks.begin(), blocks.end(), [&column](Block & b)->bool { return b.x_ == column;});
    blocks.erase(new_end, blocks.end());
    if (LEFT == dir )
        std::for_each(blocks.begin(), blocks.end(), [&column, &gb ](Block & b) {if (b.x_ < column ){ b.unmark(gb); b.x_++; b.mark(gb);};});
    else
        std::for_each(blocks.begin(), blocks.end(), [&column, &gb ](Block & b) {if (b.x_ > column ){ b.unmark(gb); b.x_--; b.mark(gb);};});
    std::for_each(blocks.begin(), blocks.end(), [&column, &gb ](Block & b) {b.mark(gb);});
}

static int checkCompleteRows(GameBoard & gb, blist & blocks, const direction_t dir) {
    int rowsRemoved=0;
    int x=gb.maxx();
    while (x > 0) {
        int y=0;
        for (; y<= gb.maxy();y++) {
            if (gb.isClear(x,y))
                break;
        }
        if (gb.maxy() == y) {
            rowsRemoved++;
            shiftRowsDown(gb,blocks,x,dir);
            x--;
        } else {
            x--;
        }
    }
    return rowsRemoved;
}
static bool gameOver(std::shared_ptr<Piece> curPiecep, GameBoard & tetrisGameBoard) {
    return curPiecep->findIf([&](Block &b)->bool {return b.done_moving() && (b.getX() == 0 || b.getX() == tetrisGameBoard.maxx()-1);});
}

#include "gamescreen.hpp"

#define MIN(A,B) (A >B ? B :A)

void isDoneMoving(const direction_t dir, std::shared_ptr<Piece> & curPiecep,std::shared_ptr<Piece> & otherPiecep,std::shared_ptr<Piece> &nextPiecep,PieceSelector & pieceSelector, GameBoard & tetrisGameBoard, blist & blocks_,blist &nextblocks_,Window_interface & next_piece_win , int & score_, bool & done) {
    if (curPiecep->done_moving()) {
        if (gameOver(curPiecep, tetrisGameBoard)){
            score_ += 1000;
            done = true;
        }
        blist temp;
        otherPiecep->unmark(tetrisGameBoard);
        otherPiecep->litr_++;
        temp.splice(temp.begin(), blocks_, otherPiecep->sitr_, otherPiecep->litr_);
        switch (checkCompleteRows(tetrisGameBoard, blocks_, dir)) {
            case 4: score_ +=100; break;
            case 3: score_ +=50; break;
            case 2: score_ +=25; break;
            case 1: score_ +=10; break;
            default:
            case 0: score_ +=1; break;
        }
        blocks_.splice(blocks_.end(), temp, temp.begin(), temp.end());
        otherPiecep->litr_ = blocks_.end();
        --otherPiecep->litr_;
        otherPiecep->sitr_ = otherPiecep->litr_;
        --otherPiecep->sitr_;
        --otherPiecep->sitr_;
        --otherPiecep->sitr_;
        otherPiecep->mark(tetrisGameBoard);
        next_piece_win.clear();
        blocks_.splice(blocks_.begin(), nextblocks_);
        curPiecep = std::move(nextPiecep);
        nextPiecep.reset( pieceSelector.getNextPiece(nextblocks_));
        std::for_each(nextblocks_.begin(), nextblocks_.end(), [&](Block & b) {next_piece_win.draw(b, -pieceSelector.getXoffset() +1, 1);});
        int moveRight = rand() %11;
        for (int i = 0; i < moveRight; i++) {
            curPiecep->down(tetrisGameBoard);
        }
        curPiecep->markAll(tetrisGameBoard);
    }
}

class TetrisGame {
    public:
        TetrisGame(GameScreen_interface &gameScreen) : gameScreen_(gameScreen), delay_(gameScreen.getDelay()) {}
        bool handleKeyBoard(GameBoard & tetrisGameBoard, bool & done,std::shared_ptr<Piece>& curPiecep,std::shared_ptr<Piece>& curOtherPiecep) {
            if (!gameScreen_.kbhit()) {
                return false;
            }
            char c = gameScreen_.lgetch();
            switch (c){
                case 'q': done=true;break;
                case 'w': curPiecep->up(tetrisGameBoard);break;
                case 's': curPiecep->down(tetrisGameBoard);break;
                case 'd': curPiecep->right(tetrisGameBoard);break;
                case 'z': curPiecep->rotateCounterClockwise(tetrisGameBoard);break;
                case 'c': curPiecep->rotateClockwise(tetrisGameBoard);break;
                case 'x':
                          while (!curPiecep->done_moving()) {
                              curPiecep->move(tetrisGameBoard);
                          }
                          break;
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
        void addBlock(GameBoard &tetrisGameBoard,const int x,const  int y) {
            blocks_.push_back(Block(x,y ));
            blocks_.back().setDxDy(x,y);
            blocks_.back().mark(tetrisGameBoard);

        }
        void play() {
            int real_max_x, real_max_y;
            gameScreen_.getMaxyx(real_max_y, real_max_x);
            int centerX = real_max_x/2;
            int centerY = real_max_y/2;
            int offsetCenterX = centerX + TETRIS_DEFAULT_WIDTH/2 -10;
            int offsetCenterY = centerY + TETRIS_DEFAULT_WIDTH/2 -10;
            int max_x = real_max_x;
            int max_y = real_max_y;
            Window_interface & board_win = gameScreen_.getWindow(13,max_x,offsetCenterY,0);
            Window_interface & score1_win = gameScreen_.getWindow(3, 12, max_y-4, max_x/4-1);
            Window_interface & score2_win = gameScreen_.getWindow(3, 12, max_y-4, ((max_x/4) *3)-1);
            Window_interface & next_piece_win = gameScreen_.getWindow(5, 6, 2, offsetCenterX /2 +11);
            Window_interface & next_other_piece_win = gameScreen_.getWindow(5, 6, 2, offsetCenterX + offsetCenterX /2 + 11);
            max_y = max_y-6;
            GameBoard tetrisGameBoard(max_x-3, 11);
            bool done= false;
            int moveDownCount = 100;
            int currentCount = 1;
            PieceSelector  pieceSelector(1,0);
            PieceSelector  OtherPieceSelector(-1,0,max_x -3,0);
            std::shared_ptr<Piece> curPiecep(pieceSelector.getNextPiece(blocks_));
            std::shared_ptr<Piece> curOtherPiecep(OtherPieceSelector.getNextPiece(blocks_));
            int moveRight = rand() %8;
            for (int i = 0; i < moveRight; i++) {
                curPiecep->down(tetrisGameBoard);
                curOtherPiecep->down(tetrisGameBoard);
            }
            std::shared_ptr<Piece> nextPiecep(pieceSelector.getNextPiece(nextblocks_));
            std::shared_ptr<Piece> nextOtherPiecep(OtherPieceSelector.getNextPiece(otherNextblocks_));
            std::for_each(nextblocks_.begin(), nextblocks_.end(), [&](Block & b) {next_piece_win.draw(b, -pieceSelector.getXoffset() +1, 1);});
            std::for_each(otherNextblocks_.begin(), otherNextblocks_.end(), [&](Block & b) {next_other_piece_win.draw(b, -OtherPieceSelector.getXoffset() +1, 1);});

            for (int i=0; i < 10; i++)
                addBlock(tetrisGameBoard,centerX, i);
            for (int i=1; i < 11; i++)
                addBlock(tetrisGameBoard,centerX-1, i);
            for (int i=1; i < 11; i++)
                addBlock(tetrisGameBoard,centerX+1, i);
            std::for_each(blocks_.begin(), blocks_.end(), [&](Block & b) {board_win.draw(b,1,1);});
            while(!done) {
                bool needRedraw =false;
                if (currentCount % moveDownCount == 0 ) {
                    curPiecep->move(tetrisGameBoard);
                    curOtherPiecep->move(tetrisGameBoard);
                    needRedraw = true;
                }
                currentCount++;
                needRedraw |= handleKeyBoard(tetrisGameBoard,done,curPiecep,curOtherPiecep);
                usleep(delay_);
                if (needRedraw) {
                    gameScreen_.clear();
                    board_win.clear();
                    isDoneMoving(LEFT,curPiecep,curOtherPiecep, nextPiecep,pieceSelector, tetrisGameBoard, blocks_,nextblocks_,next_piece_win, scorePlayer1_,done);
                    isDoneMoving(RIGHT,curOtherPiecep,curPiecep, nextOtherPiecep,OtherPieceSelector, tetrisGameBoard, blocks_,otherNextblocks_,next_other_piece_win, scorePlayer2_,done);
                    std::for_each(blocks_.begin(), blocks_.end(), [&](Block & b) {board_win.draw(b,1,1);});
                    gameScreen_.refreshMain();
                    board_win.refresh();
                    score1_win.text(1, 1, "score: %d",scorePlayer1_);
                    score1_win.refresh();
                    score2_win.text(1, 1, "score: %d",scorePlayer2_);
                    score2_win.refresh();
                    next_piece_win.refresh();
                    next_other_piece_win.refresh();
                    gameScreen_.doupdate();
                }
            }
        }
        int getScorePlayer1() const {
            return scorePlayer1_;
        }
        int getScorePlayer2() const {
            return scorePlayer2_;
        }
    private:
        GameScreen_interface &gameScreen_;
        int delay_;
        blist blocks_;
        blist nextblocks_;
        blist otherNextblocks_;
        int scorePlayer1_ = 0;
        int scorePlayer2_ = 0;
};

#include "ncurses_game_screen.hpp"
#include <iostream>

int main() {
    /* initialize random seed: */
    srand (time(NULL));
    int p1, p2;
    {
        NCurses::GameScreen gameScreen;
        TetrisGame tetris(gameScreen);
        tetris.play();
        p1 = tetris.getScorePlayer1();
        p2 = tetris.getScorePlayer2();
    }
    std::cout << "final score player1: " << p1 <<  "  player 2: "<< p2<<  std::endl;
};

