#include "block.hpp"
#ifndef GAMESCREEN_HPP
#define GAMESCREEN_HPP
class Window_interface  {
    public:
        Window_interface() {}
        virtual ~Window_interface() {}
        virtual void draw(Block & b, int drawoffsetx=0 , int drawoffsety=0) =0;
        virtual void erase(Block & b, int drawoffsetx=0 , int drawoffsety=0) =0;
        virtual void text(int x, int y, const char * fmt, ...)  =0;
        virtual void refresh() =0;
        virtual void clear(void)  =0;
};

class GameScreen_impl;


class GameScreen {
    public:
        GameScreen();
        ~GameScreen();
        void getMaxyx(int &y, int &x);
        Window_interface & getWindow(int height, int width, int yoffset, int xoffset);
        void refreshMain();
        void doupdate();
        void clear(void) const;
        bool kbhit();
        char lgetch();
        int getDelay();
    private:
			GameScreen_impl *gameScreen;
};

#endif
