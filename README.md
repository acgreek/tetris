# build
* cmake .
* make


# wartetris
two player tetris or one player two hand variant. You have one game board that is horizontal. One player moves from the left to the right and the other player moves from the right to the left. To win, one of the players has to get one of there pieces to the start position of the other player or other player has to run out of room to spawn new pieces .
I built it on linux using ncurses, it should work in linux in cygwin as well.

## keys
q - quit

### left player:
* w - up
* s - down
* d - right
* z - rotate counter clockwise
* c - rotate clockwise
* x - move right until hit something

### right player:
* i - up
* k - down 
* j - left 
* m - rotate counter clockwise
* . - rotate clockwise
* , - move left until hit something

# tetris
ncurses Tetris clone

## keys
* q     - quit
* a     - left
* d     - right
* q     - quit
* z     - rotate counter clockwise
* c     - rotate clockwise
* space - drop until hit something
