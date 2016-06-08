#include "block.hpp"
#ifndef GAMESCREEN_HPP
#define GAMESCREEN_HPP
class Window_interface  {
    public:
        Window_interface() {}
        virtual ~Window_interface() {}
        virtual void draw(Block & b,const char *symbol = "o", int drawoffsetx=0 , int drawoffsety=0) =0;
        virtual void text(int x, int y, const char * fmt, ...)  =0;
        virtual void refresh() =0;
        virtual void clear(void)  =0;
};

class GameScreen_interface {
    public:
        GameScreen_interface() {};
        virtual ~GameScreen_interface() {};
        virtual void getMaxyx(int &y, int &x) =0;
        virtual Window_interface & getWindow(int height, int width, int yoffset, int xoffset) = 0;
        virtual void refreshMain()  =0;
        virtual void doupdate() =0;
        virtual void clear(void) const  =0;
        virtual bool kbhit() = 0;
        virtual char lgetch() = 0;
};
#endif
