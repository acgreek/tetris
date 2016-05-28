
class Vectorable {
    public:
        virtual int getX() const = 0;
        virtual int getY() const = 0;
        virtual int getZ() const = 0;
        virtual int setX(int x)= 0;
        virtual int setY(int y)= 0;
        virtual int setZ(int z)= 0;
};
