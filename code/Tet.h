#ifndef TET_H
#define TET_H
#pragma execution_character_set("utf-8")
#include <string>
class Single_Block
{
private:
    int y;
    int x;
    bool shape;
public:
    Single_Block();
    Single_Block(int, int);

    int cy();
    int cx();

    int my(int);
    int mx(int);

    void setyx(int, int);
    void movyx(int, int);
    void coreBset(int, int);

    void mleft();
    void mright();
    void mdown();
    void mup();
};
class Tetris
{
private:
    Single_Block mblock[4];
    Single_Block mshad[4];
    int mrot, mcolor, packnum = 0;
    char mname, pack[7] = {'T','L','J','S','Z','O','I'},pre_pack[7];
    int color[20][10];
    int lives = 1;
public:
    //For Static:
    void initialize();
    void game_over();

    bool is_empty(int y, int x);
    int scan(int y,int x);
    int line();
    int line_erase();
    //For Moving:
    void rand7pack();
    void spawn(char n);

    bool can_trans(int);
    bool can_sink(bool is_shad);
    bool can_rot(int, int, int);
    bool can_shad(int);

    void move();
    bool trans(int);
    void sink();
    void rotate();

    void shad();
    void display();
};
#endif // TET_H
