CXXFLAGS =-ggdb3 -Wall -std=c++11
LDLIBS=	-lncurses
all: tetris

clean: 
	rm -rf tetris

.PHONY: clean 


