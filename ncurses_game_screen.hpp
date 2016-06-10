#include "gamescreen.hpp"
namespace NCurses {
#include <ncurses.h>
#include "keyboard.h"

WINDOW *create_newwin(int height, int width, int starty, int startx) {
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

        Window() { }
        void set(WINDOW *w)   {
            w_.reset(w,delwin) ;
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
            NCurses::wclear(w_.get());
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
        std::shared_ptr<WINDOW> w_;
};
class GameScreen :public GameScreen_interface{
    public:
        GameScreen():  cursesSetup() {
            curs_set(FALSE);
        }
        virtual ~GameScreen() {

        }
        virtual void clear(void) const {
            NCurses::clear();
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
            NCurses::doupdate();
        }
        virtual bool kbhit() {
            return NCurses::kbhit();
        }
        virtual char lgetch() {

            return NCurses::lgetch();
        }
        virtual int getDelay() {
           return 3000;
        };
    private:
        CursesSetup cursesSetup;
        std::list<Window> windows;

};

}// namespace
