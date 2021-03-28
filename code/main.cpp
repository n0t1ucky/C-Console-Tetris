#include <QCoreApplication>
#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "Tet.h"
#include <time.h>
#include <string>
#include <QTextCodec>
#include <QString>
#define DISP_L 0
#define DISP_R 9
#define DISP_T 0
#define DISP_D 19
using namespace std;
double Gscore = 0;//全局分数
int lines_cleared = 0;
int level = 1;//与速度相关
string space = " ";//1空格
string ipcheat;
HANDLE hOutput;
COORD coord = { 0,0 };
DWORD bytes = 0;
DWORD dwLast;
DWORD dwCurrent;
DWORD dwInterval = 5;
void go();
void menu(string code, int c);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //SetConsoleTitle("TETRIS");
    //test(2);
    menu("0",0);
    return a.exec();
}

void go()
{
    Tetris T;
    T.initialize();
    T.move();
}
void opSpc(int n)
{
    while(n--)
        cout << space;
}
void main_menu()
{

}

//***********************class Single_Block***********************
Single_Block::Single_Block()//test
{
    y = DISP_R / 2;
    x = 0;
}

Single_Block::Single_Block(int y, int x)//
{
    this->y = y;
    this->x = x;
}

int Single_Block::cy()
{
    return y;
}
int Single_Block::cx()
{
    return x;
}

int Single_Block::my(int m)
{
    y += m;
    return y;
}
int Single_Block::mx(int m)
{
    x += m;
    return x;
}
void Single_Block::movyx(int dy, int dx)
{
    my(dy);
    mx(dx);
}
void Single_Block::setyx(int y, int x)
{
    this->y = y;
    this->x = x;
}
void Single_Block::coreBset(int x, int y)//
{
    this->y = ((DISP_R - DISP_L) / 2) + x;
    this->x = y;
}


void Single_Block::mleft()
{
    if (x > DISP_L)
        --x;
}
void Single_Block::mright()
{
    if (x < DISP_R)
        ++x;
}
void Single_Block::mdown()
{
    if (y < DISP_D)
        ++y;
}
void Single_Block::mup()
{
    if (y > DISP_T)
        --y;
}
//Tetris
void Tetris::initialize()
{
    memset(color, 0x00, 800);
    spawn(pack[packnum]);
}
void Tetris::game_over()
{
    Gscore = 0;
    lines_cleared = 0;
    level = 1;
    memset(color, 0x40, 800);
    display();
    Sleep(2000);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x4C);
    cout << "Game Over." << endl;
    initialize();
    system("cls");
    menu("0", 0);
}
void Tetris::rand7pack()
{
    //俄罗斯方块机制, 7个方块不重样

}

//定义方块形状
void Tetris::spawn(char n)//test:T shape
{
    if (color[1][4] != 0x00)
    {
        game_over();
    }
    int coy, cox;
    switch (n)
    {
    case 'T':
    {
        coy = 1, cox = 4;
        mblock[0].setyx(coy, cox);
        mblock[1].setyx(coy - 1, cox);
        mblock[2].setyx(coy, cox - 1);
        mblock[3].setyx(coy, cox + 1);
        mrot = 0;
        mname = 'T';
        mcolor = 0x0D;
    }break;
    case 'L':
    {
        coy = 1, cox = 4;
        mblock[0].setyx(coy, cox);
        mblock[1].setyx(coy - 1, cox + 1);
        mblock[2].setyx(coy, cox - 1);
        mblock[3].setyx(coy, cox + 1);
        mrot = 0;
        mname = 'L';
        mcolor = 0x0F;
    }break;
    case 'J':
    {
        coy = 1, cox = 4;
        mblock[0].setyx(coy, cox);
        mblock[1].setyx(coy - 1, cox - 1);
        mblock[2].setyx(coy, cox - 1);
        mblock[3].setyx(coy, cox + 1);
        mrot = 0;
        mname = 'J';
        mcolor = 0x09;
    }break;
    case 'O':
    {
        coy = 1, cox = 4;
        mblock[0].setyx(coy, cox);
        mblock[1].setyx(coy - 1, cox);
        mblock[2].setyx(coy - 1, cox + 1);
        mblock[3].setyx(coy, cox + 1);
        mrot = 0;
        mname = 'O';
        mcolor = 0x0E;
    }break;
    case 'S':
    {
        coy = 1, cox = 4;
        mblock[0].setyx(coy, cox);
        mblock[1].setyx(coy - 1, cox);
        mblock[2].setyx(coy, cox - 1);
        mblock[3].setyx(coy - 1, cox + 1);
        mrot = 0;
        mname = 'S';
        mcolor = 0x0A;
    }break;
    case 'Z':
    {
        coy = 1, cox = 4;
        mblock[0].setyx(coy, cox);
        mblock[1].setyx(coy - 1, cox);
        mblock[2].setyx(coy - 1, cox - 1);
        mblock[3].setyx(coy, cox + 1);
        mrot = 0;
        mname = 'Z';
        mcolor = 0x0C;
    }break;
    case 'I':
    {
        coy = 0, cox = 5;
        mblock[0].setyx(coy, cox);
        mblock[1].setyx(coy, cox - 2);
        mblock[2].setyx(coy, cox - 1);
        mblock[3].setyx(coy, cox + 1);
        mrot = 0;
        mname = 'I';
        mcolor = 0x0B;
    }break;
    default:
        break;
    }
    //TODO:如果顶格，则gameover:底色0x40
}
//CANs
bool Tetris::is_empty(int y, int x)//判断是否空
{
    if (color[y][x] <= 0x00)
        return true;
    else
        return false;
}
bool Tetris::can_trans(int direction)
{
    //return true;
    int by[4], bx[4];
    for (int i = 0; i < 4; ++i)
    {
        by[i] = mblock[i].cy();
        bx[i] = mblock[i].cx();
    }
    switch (direction)
    {
    case 75://左是否为空
    {
        for (int i = 0; i < 4; ++i)
        {
            if (!is_empty(by[i], bx[i] - 1) || bx[i] <= DISP_L)
                return false;
        }
        return true;
    }break;
    case 77://右是否为空
    {
        for (int i = 0; i < 4; ++i)
        {
            if (!is_empty(by[i], bx[i] + 1) || bx[i] >= DISP_R)
                return false;
        }
        return true;
    }break;
    case 80://下是否为空
    {
        for (int i = 0; i < 4; ++i)
        {
            if (!is_empty(by[i] + 1, bx[i]) || by[i] >= DISP_D)
                return false;
        }
        return true;
    }break;
    case 119:
    {
        for (int i = 0; i < 4; ++i)
        {
            if (!is_empty(by[i] - 1, bx[i]) || by[i] <= DISP_T)
                return false;
        }
        return true;
    }break;
    default:
    {
        for (int i = 0; i < 4; ++i)
        {
            if (!is_empty(by[i] + 1, bx[i]) || by[i] >= DISP_D)
                return false;
        }
        return true;
    }break;
        return false;
        break;
    }
}
bool Tetris::can_shad(int direction)
{
    //return true;
    int by[4], bx[4];
    for (int i = 0; i < 4; ++i)
    {
        by[i] = mshad[i].cy();
        bx[i] = mshad[i].cx();
    }
    switch (direction)
    {
    case 75://左是否为空
    {
        for (int i = 0; i < 4; ++i)
        {
            if (!is_empty(by[i], bx[i] - 1) || bx[i] <= DISP_L)
                return false;
        }
        return true;
    }break;
    case 77://右是否为空
    {
        for (int i = 0; i < 4; ++i)
        {
            if (!is_empty(by[i], bx[i] + 1) || bx[i] >= DISP_R)
                return false;
        }
        return true;
    }break;
    case 80://下是否为空
    {
        for (int i = 0; i < 4; ++i)
        {
            if (!is_empty(by[i] + 1, bx[i]) || by[i] >= DISP_D)
                return false;
        }
        return true;
    }break;
    case 119:
    {
        for (int i = 0; i < 4; ++i)
        {
            if (!is_empty(by[i] - 1, bx[i]) || by[i] <= DISP_T)
                return false;
        }
        return true;
    }break;
    default:
        return false;
        break;
    }
}
bool Tetris::can_sink( bool is_shad)
{
    int by[4], bx[4];
    //return true;
    if (is_shad)
        for (int i = 0; i < 4; ++i)
        {
            by[i] = mblock[i].cy();
            bx[i] = mblock[i].cx();
            if (!is_empty(by[i] + 1, bx[i]) || by[i] >= DISP_D)
                return false;
        }
    else
        for (int i = 0; i < 4; ++i)
        {
            by[i] = mshad[i].cy();
            bx[i] = mshad[i].cx();
            if (!is_empty(by[i] + 1, bx[i]) || by[i] >= DISP_D)
                return false;
        }
    return true;
}
bool Tetris::can_rot(int i, int y, int x)
{
    y += mblock[i].cy();
    x += mblock[i].cx();
    bool St = is_empty(y, x);
    bool L = x >= DISP_L, R = x <= DISP_R;
    bool U = y >= DISP_T, D = y <= DISP_D;
    //TODO:判断固定方块
    //if (S.empty(y, x) && (y < DISP_D) && (y > DISP_T) && (x < DISP_R) && (x > DISP_L))
    if (St&&L&&R&&U&&D)
        return true;
    else if (!L)
    {
        return trans(77);
    }
    else if (!R)
    {
        return trans(75);
    }
    else if (!D)
    {
        return trans(119);
    }
    else
        return false;
}
//MOVEs
void Tetris::rotate()
{
    switch (mname)
    {
    case 'T':
    {
        switch (mrot)
        {
        case 0:
        {
            if (can_rot(2, 1, 1))
            {
                ++mrot;
                mblock[2].movyx(1, 1);
            }
            else
                break;
        }break;
        case 1:
        {
            if (can_rot(1, 1, -1))
            {
                ++mrot;
                mblock[1].movyx(1, -1);
            }
            else
                break;

        }break;
        case 2:
        {
            if (can_rot(3, -1, -1))
            {
                ++mrot;
                mblock[3].movyx(-1, -1);
            }
            else
                break;
        }break;
        case 3:
        {
            if (can_rot(1, -1, 1) && can_rot(2, -1, -1) && can_rot(3, 1, 1))
            {
                mrot = 0;
                mblock[1].movyx(-1, 1);
                mblock[2].movyx(-1, -1);
                mblock[3].movyx(1, 1);
            }
            else
                break;
        }break;
        default:
            break;
        }
    }break;
    case 'L':
    {
        switch (mrot)
        {
        case 0:
        {
            if (can_rot(1, 2, 0) && can_rot(2, -1, 1) && can_rot(3, 1, -1))
            {
                ++mrot;
                mblock[1].movyx(2, 0);
                mblock[2].movyx(-1, 1);
                mblock[3].movyx(1, -1);
            }
            else
                break;
        }break;
        case 1:
        {
            if (can_rot(1, -1, 0) && can_rot(2, 1, -1) && can_rot(3, 0, -1))
            {
                ++mrot;
                mblock[1].movyx(-1, 0);
                mblock[2].movyx(1, -1);
                mblock[3].movyx(0, -1);
            }
            else
                break;

        }break;
        case 2:
        {
            if (can_rot(1, -1, -1) && can_rot(2, -1, 0) && can_rot(3, 0, 1))
            {
                ++mrot;
                mblock[1].movyx(-1, -1);
                mblock[2].movyx(-1, 0);
                mblock[3].movyx(0, 1);
            }
            else
                break;
        }break;
        case 3:
        {
            if (can_rot(1, 0, 1) && can_rot(2, 1, 0) && can_rot(3, -1, 1))
            {
                mrot = 0;
                mblock[1].movyx(0, 1);
                mblock[2].movyx(1, 0);
                mblock[3].movyx(-1, 1);
            }
            else
                break;
        }break;
        default:
            break;
        }
    }break;
    case 'J':
    {
        switch (mrot)
        {
        case 0:
        {
            if (can_rot(1, 0, 2) && can_rot(2, -1, 1) && can_rot(3, 1, -1))
            {
                ++mrot;
                mblock[1].movyx(0, 2);
                mblock[2].movyx(-1, 1);
                mblock[3].movyx(1, -1);
            }
            else
                break;
        }break;
        case 1:
        {
            if (can_rot(1, 1, 0) && can_rot(2, 1, -1) && can_rot(3, 0, 1))
            {
                ++mrot;
                mblock[1].movyx(1, 0);
                mblock[2].movyx(1, -1);
                mblock[3].movyx(0, 1);
            }
            else
                break;

        }break;
        case 2:
        {
            if (can_rot(1, -1, -1) && can_rot(2, 1, 0) && can_rot(3, 0, -1))
            {
                ++mrot;
                mblock[1].movyx(-1, -1);
                mblock[2].movyx(1, 0);
                mblock[3].movyx(0, -1);
            }
            else
                break;
        }break;
        case 3:
        {
            if (can_rot(1, 0, -1) && can_rot(2, -1, 0) && can_rot(3, -1, 1))
            {
                mrot = 0;
                mblock[1].movyx(0, -1);
                mblock[2].movyx(-1, 0);
                mblock[3].movyx(-1, 1);
            }
            else
                break;
        }break;
        default:
            break;
        }
    }break;
    case 'S':
    {
        switch (mrot)
        {
        case 0:
        {
            if (can_rot(2, 0, 2) && can_rot(3, 2, 0))
            {
                ++mrot;
                mblock[2].movyx(0, 2);
                mblock[3].movyx(2, 0);
            }
            else
                break;
        }break;
        case 1:
        {
            if (can_rot(1, 2, 0) && can_rot(3, 0, -2))
            {
                ++mrot;
                mblock[1].movyx(2, 0);
                mblock[3].movyx(0, -2);
            }
            else
                break;

        }break;
        case 2:
        {
            if (can_rot(2, 0, -2) && can_rot(3, -2, 0))
            {
                ++mrot;
                mblock[2].movyx(0, -2);
                mblock[3].movyx(-2, 0);
            }
            else
                break;
        }break;
        case 3:
        {
            if (can_rot(1, -2, 0) && can_rot(3, 0, 2))
            {
                mrot = 0;
                mblock[1].movyx(-2, 0);
                mblock[3].movyx(0, 2);
            }
            else
                break;
        }break;
        default:
            break;
        }
    }break;
    case 'Z':
    {
        switch (mrot)
        {
        case 0:
        {
            if (can_rot(1, 2, 0) && can_rot(2, 0, 2))
            {
                ++mrot;
                mblock[1].movyx(2, 0);
                mblock[2].movyx(0, 2);
            }
            else
                break;
        }break;
        case 1:
        {
            if (can_rot(2, 2, 0) && can_rot(3, 0, -2))
            {
                ++mrot;
                mblock[2].movyx(2, 0);
                mblock[3].movyx(0, -2);
            }
            else
                break;

        }break;
        case 2:
        {
            if (can_rot(1, -2, 0) && can_rot(2, 0, -2))
            {
                ++mrot;
                mblock[1].movyx(-2, 0);
                mblock[2].movyx(0, -2);
            }
            else
                break;
        }break;
        case 3:
        {
            if (can_rot(2, -2, 0) && can_rot(3, 0, 2))
            {
                mrot = 0;
                mblock[2].movyx(-2, 0);
                mblock[3].movyx(0, 2);
            }
            else
                break;
        }break;
        default:
            break;
        }
    }break;
    case 'O':break;
    case 'I':
    {
        switch (mrot)
        {
        case 0:
        {
            if (can_rot(1, 2, 2) && can_rot(2, 1, 1) && can_rot(3, -1, -1))
            {
                ++mrot;
                mblock[1].movyx(2, 2);
                mblock[2].movyx(1, 1);
                mblock[3].movyx(-1, -1);
            }
            else
                break;
        }break;
        case 1:
        {
            if (can_rot(1, -2, 2) && can_rot(2, -1, 1) && can_rot(3, 1, -1))
            {
                ++mrot;
                mblock[1].movyx(-2, 2);
                mblock[2].movyx(-1, 1);
                mblock[3].movyx(1, -1);
            }
            else
                break;

        }break;
        case 2:
        {
            if (can_rot(1, -2, -2) && can_rot(2, -1, -1) && can_rot(3, 1, 1))
            {
                ++mrot;
                mblock[1].movyx(-2, -2);
                mblock[2].movyx(-1, -1);
                mblock[3].movyx(1, 1);
            }
            else
                break;
        }break;
        case 3:
        {
            if (can_rot(1, 2, -2) && can_rot(2, 1, -1) && can_rot(3, -1, 1))
            {
                mrot = 0;
                mblock[1].movyx(2, -2);
                mblock[2].movyx(1, -1);
                mblock[3].movyx(-1, 1);
            }
            else
                break;
        }break;
        default:
            break;
        }
    }break;

    default:
        break;
    }

}
void Tetris::move()
{
    int t = 0;
    int key = 0x00;
    while (1)
    {
        dwLast = GetTickCount();
        display();
        dwCurrent = GetTickCount();
        if (dwCurrent - dwLast < dwInterval)
            continue;
        while (_kbhit())
        {
            key = _getch();
            int times_of_tr = 0;//运动次数
            switch (key)
            {
            case 0xE0:
            {
                trans(key = _getch());
                display();
            }break;
            case 0x20:
            {
                Gscore += 10 * (20 - mblock[0].cy());//下沉每格+10分
                sink();
                display();
            }break;
            case 0x1B:
            {
                opSpc(18);
                cout << "////PAUSE////";
                getchar();
                display();

            }break;
            case 0x2F:
            {
                opSpc(18);
                cout << ">/";
                cin >> ipcheat;
                display();
            }break;
            default:
                break;
            }
        }
        ++t;
        if (t*dwInterval == 100)
        {
            t = 0;
            if (can_trans(80))
                trans(-1);
            else
                sink();
        }
        dwLast = dwCurrent;
        line();
    }

    /*
    */
    //TODO自动下落


}
bool Tetris::trans(int d)//d为方向
{
    int count = 0;//沉底后可运动15次
    switch (d)
    {
    case 72:rotate(); break;
    case 75:
    {
        if (can_trans(75))
        {
            for (int i = 0; i < 4; ++i)
                mblock[i].mleft();//左
            return true;
        }
        else break;
    }break;
    case 77:
    {
        if (can_trans(77))
        {
            for (int i = 0; i < 4; ++i)
                mblock[i].mright();//右
            return true;
        }
        else break;
    }break;
    case 80://手动下落
    {
        if (can_trans(80))
        {
            for (int i = 0; i < 4; ++i)
                mblock[i].mdown();//下
            ++Gscore;//手动操作下落 1分/格
            return true;
        }
        else break;
    }break;
    case 119:
    {
        if (can_trans(119))
        {
            for (int i = 0; i < 4; ++i)
                mblock[i].mup();//上
            return true;
        }
        else break;
    }break;
    case -1://自动下落
    {
        if (can_trans(80))
        {
            for (int i = 0; i < 4; ++i)
                mblock[i].mdown();//下
            return true;
        }
        else break;
    }break;

    default:break;
    }
    return false;
}
void Tetris::sink()
{
    while (can_trans(80))
    {
        for (int i = 0; i < 4; ++i)
            mblock[i].mdown();
    }
    for (int i = 0; i < 4; ++i)
    {
        color[mblock[i].cy()][mblock[i].cx()] = mcolor;
    }
    if (packnum < 6)
        ++packnum;
    else
    {
        packnum = 0;
        //rand7pack();
    }
    line();
    //spawn('O');
    spawn(pack[packnum]);//spawn another
}
//Paint Shadow
void Tetris::shad()
{
    for (int i = 0; i < 4; ++i)
    {
        mshad[i].setyx(mblock[i].cy(), mblock[i].cx());
    }
    while (can_shad(80))
    {
        for (int i = 0; i < 4; ++i)
            mshad[i].mdown();
    }
}
//Clear A row
int Tetris::line()
{
    int r = 3;
    int count, row[4], cleared = 0;//最多清理4行
    for (int i = 0; i < 20; ++i)
    {
        count = 0;
        for (int j = 0; j < 10; ++j)
            if (color[i][j] != 0x00)
                ++count;
        if (count == 10)
        {
            row[r--] = i;//消除行的行号
            for (int j = 0; j < 10; ++j)
                color[i][j] += 0xF0;
            ++cleared;
        }
    }
    if(cleared == 0)
        return cleared;
    display();//显示变白
    Sleep(30);
    for (int i = 19; i >= 0; --i)
    {
        count = 0, r = 0;
        for(;r < 4; ++r)
        {
            if (row[r] > i)
                ++count;
            else if (row[r] == i)
                continue;
        }
        if (count == 0)
            continue;
        else
        {
            for (int j = 0; j < 10; ++j)
            {
                if (i - count >= 0)
                    color[i + count][j] = color[i][j];
            }
            for (int j = 0; j < 10; ++j)
                color[i][j] = 0x00;
        }
    }
    Gscore += 1000 * cleared;
    lines_cleared += cleared;
    //TODO:加速效果
    return cleared;
}
//Display
int Tetris::scan(int y, int x)
{
    for (int i = 0; i < 4; ++i)
    {
        if (mblock[i].cy() == y && mblock[i].cx() == x)
            return mcolor;
    }
    for (int i = 0; i < 4; ++i)
    {
        if (mshad[i].cy() == y && mshad[i].cx() == x)
            return mcolor - 0x08;
    }
    return color[y][x];
}
void Tetris::display()
{

    hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(hOutput, &cci);
    cci.bVisible = false;
    SetConsoleCursorInfo(hOutput, &cci);
    SetConsoleCursorPosition(hOutput, coord);

    shad();

    int c[20][10];
    for (int i = 0; i < 20; ++i)
        for (int j = 0; j < 10; ++j)
            c[i][j] = scan(i, j);
    for (int i = 0; i < 20; ++i)
        for (int j = 0; j < 10; ++j)
        {
            if (c[i][j] == 0x00)
                c[i][j] = scan(i, j);
        }
    opSpc(18);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
    cout << "SCORE:" << Gscore << endl;
    for (int i = 0; i < 20; ++i)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x00);
        opSpc(18);
        for (int j = 0; j < 10; ++j)
        {
            if (c[i][j] == 0x00)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x08);
                cout << "[]";
            }
            else
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c[i][j]);
                cout << "##";
            }
        }
        cout << endl;
    }
    //opSpc(60);
}
void menu(string code, int c)
{
    hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(hOutput, &cci);
    cci.bVisible = false;
    SetConsoleCursorInfo(hOutput, &cci);
    //SetConsoleCursorPosition(hOutput, coord);

    string menu[3] = { "Start","About","Exit" };
    int select = 0, key = 0;
    while (1)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
        cout << "Tetris Console alpha 0.9" << endl;
        for (int i = 0; i < 3; ++i)
        {
            //system("cls");
            if (i == select)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x8f);
                cout << menu[i];
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
                cout << endl;
            }
            else
                cout << menu[i] << endl;
        }
        if ((key = _getch()) == 0xE0)
        {
            switch (_getch())
            {
            case 72:
            {
                if (select > 0)
                    --select;
            }; break;
            case 75:
            {
                //左
            }break;
            case 77:
            {
                //右
            }break;
            case 80:
            {
                if (select < 2)
                    ++select;
                //下
            }break;

            default:
                break;
            }
        }
        else if (key == 0x0D && select == 0)
            go();
        else if (key == 0x0D && select == 1)
        {
            system("cls");
            cout << "Tetris Console alpha 0.9\nCode by: Naught_IX\nLast Update:2021/03/28\n\nBack to menu in 5s.";
            Sleep(5000);
            system("cls");
        }
        else if (key == 0x0D && select == 2)
        {
            int i = 3;
            while (i)
            {
                system("cls");
                cout << "Exit......" << i-- << endl;
                Sleep(1000);
            }
            exit(0);
        }
        system("cls");
    }
}
