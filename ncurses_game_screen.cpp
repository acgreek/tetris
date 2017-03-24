#include "gamescreen.hpp"
#include "keyboard.h"
#include <ncurses.h>
#include <memory>


static WINDOW *create_newwin(int height, int width, int starty, int startx) {
    auto *local_win = newwin(height, width, starty, startx);
    box(local_win, 0 , 0);/* 0, 0 gives default characters
                           * for the vertical and horizontal
                           *                   * lines*/
    wrefresh(local_win);/* Show that box */
    return local_win;
}

class CursesSetup {
    public:
        CursesSetup() {
            initscr();
            if(has_colors() == FALSE)
            {
                endwin();
                printf("Your terminal does not support color\n");
                exit(1);
            }
            start_color();
            init_pair(1, COLOR_RED, COLOR_RED);
            init_pair(2, COLOR_BLUE, COLOR_BLUE);
            init_pair(3, COLOR_GREEN, COLOR_GREEN);
            init_pair(4, COLOR_WHITE, COLOR_WHITE);
            init_pair(5, COLOR_WHITE, COLOR_BLACK);
            wbkgd(stdscr, COLOR_PAIR(2));
            noecho();

        }
        ~CursesSetup (){
            endwin();
        }
};

class Window : public Window_interface {
    public:

        Window() :w_(NULL,[](WINDOW *w) {w=w;}) { }
        void set(WINDOW *w) {  
					w_  =  std::unique_ptr<WINDOW,void (*)(WINDOW * w)>(w,[](WINDOW *w) {delwin(w);}) ;
        }
        void setColor(int color) {
            if (currentColor_ != color) {
                wattron(get(),COLOR_PAIR(color));
                currentColor_ =color;
            }
        }
        void refresh() {
            setColor (5);
            box(get(), 0 , 0);
            wnoutrefresh(get());
        }
        void draw(Block & b, int drawoffsetx=0 , int drawoffsety=0) {

            switch (b.getColor()){
                case Block::RED: setColor (1);break;
                case Block::BLUE: setColor (2);break;
                case Block::GREEN: setColor (3);break;
                case Block::WHITE: setColor (4);break;
                case Block::MAX_COLOR: abort(); break;
            }
            char str[2] = "A";
            str[0] = 'A' + b.getColor();
            if (b.getX() >=0)
                mvwprintw(get(), b.getY() + drawoffsety, b.getX()+drawoffsetx, str);
        }
        virtual void erase(Block & b, int drawoffsetx=0 , int drawoffsety=0) {
            if (b.getX() >=0)
                mvwprintw(get(), b.getY() + drawoffsety, b.getX()+drawoffsetx, " ");

        }
        void clear(void) {
            wclear(w_.get());
        }
        void text(int x, int y, const char * fmt, ...) {
            setColor (5);
            va_list argp;
            va_start(argp, fmt);
            wmove(w_.get(),y, x);
            vw_printw(w_.get(), fmt,argp);
            va_end(argp);
        }
    private:
        int currentColor_ = 0;
        WINDOW * get() { return w_.get(); }
        std::unique_ptr<WINDOW, void (*)(WINDOW * w)> w_;
};

class GameScreen_impl {
    public:
        GameScreen_impl():  cursesSetup() {
            curs_set(FALSE);
        }
        virtual ~GameScreen_impl() {

        }
        virtual void clear(void) const {
            ::clear();
        }
        virtual void getMaxyx(int &y, int &x) {
            getmaxyx(stdscr, y, x);
        }
        virtual Window_interface & getWindow(int height, int width, int yoffset, int xoffset){
            windows.push_back(Window());
            windows.back().set(create_newwin(height,width,yoffset,xoffset));
            return windows.back();
        }
        virtual void refreshMain() {
            wnoutrefresh(stdscr);

        };
        virtual void doupdate(){
            ::doupdate();
        }
        virtual bool kbhit() {
            return ::kbhit();
        }
        virtual char lgetch() {

            return ::lgetch();
        }
        virtual int getDelay() {
           return 3000;
        };
    private:
        CursesSetup cursesSetup;
        std::list<Window> windows;
};


GameScreen::GameScreen() { gameScreen = new GameScreen_impl;  };
GameScreen::~GameScreen() { delete gameScreen;}
void GameScreen::getMaxyx(int &y, int &x) { gameScreen->getMaxyx(y,x);}
Window_interface & GameScreen::getWindow(int height, int width, int yoffset, int xoffset) {return gameScreen->getWindow (height, width, yoffset, xoffset);}
void GameScreen::refreshMain()  {gameScreen->refreshMain();}
void GameScreen::doupdate() {gameScreen->doupdate();}
void GameScreen::clear(void) const {gameScreen->clear();} 
bool GameScreen::kbhit() {return gameScreen->kbhit();}
char GameScreen::lgetch() {return gameScreen->lgetch();}
int GameScreen::getDelay() {return gameScreen->getDelay();}
