#ifndef Game_Logic_H
#define Game_Logic_H

#include <QPushButton>
#include <QVector>
#include <QKeyEvent>
#include <QWidget>
#include <QProgressBar>
#include <QPainter>
#include <QPalette>
#include <QFile>
#include <QFileInfo>
#include "player.h"
#include "box.h"

const int ROWS = 10;
const int COLS = 10;

extern int gamedata[ROWS+2][COLS+2];
extern bool is_show;

class Game_Logic
{
public:
    Game_Logic();
    Game_Logic(int array[],int score);
    Game_Logic(int array[],int score1,int score2);
    Game_Logic(int mode);
    void initGame();//初始化地图
    void initGame_double();//把地图替换一下double的道具
    void clearImage(int a,int b);//将被clear掉的地方设为0，可通行
    void playerDire(Player p);
    void playerDire_second(Player p);

    //移动相关函数
    int moveUp_num1();
    int moveUp_num2();
    int moveUp(int num);
    int moveDown_num1();
    int moveDown_num2();
    int moveDown(int num);
    int moveLeft_num1();
    int moveLeft_num2();
    int moveLeft(int num);
    int moveRight_num1();
    int moveRight_num2();
    int moveRight(int num);
    bool isWalkable(Player p,char direction);
    bool isWalkable_W(Player p);
    bool isWalkable_S(Player p);
    bool isWalkable_A(Player p);
    bool isWalkable_D(Player p);


    //方块消除相关函数
    void boxOn(int num,int boxx,int boxy);
    void numBox(int num,int num2);//返回目标方块
    void num_LastBox(int num,int num2);
    int boxClear(int mode,int last,int now);
    bool isClearable(int mode,int last,int now);//判断是否处于可以消除的位置
    bool isAble_Nocorner_Clear(int mode,int last,int now);//判断是否是不需要折线就可以消除的
    bool isAble_Onecorner_Clear(int mode,int last,int now);//判断是否是需要一条折线就可以消除的
    void isAble_Twocorner_Clear_Box1(int mode,int last,int mid_line);
    void isAble_Twocorner_Clear_Box2(int mode);
    void isAble_Twocorner_Clear_Box3(int mode,int last,int mid_line);
    void isAble_Twocorner_Clear_Box4(int mode);
    bool isAble_Twocorner_Clear(int mode,int last,int now);//判断是否是需要两条折线就可以消除的
    bool isAble_player_to(int num);//判断玩家能否到这里来
    bool isAble_continue();//判断是否还有可以消除的方块
    void scoreRenew(int player, int num);//分数更新

    //道具类方块函数
    void refreshLogic();//重排道具(48号道具)
    bool hintLogic();//暗示道具
    void flashLogic();//flash道具
    bool isClearable_hint(int last, int now);
    bool isAble_Nocorner_Clear_hint(int last,int now);//判断是否是不需要折线就可以消除的
    bool isAble_Onecorner_Clear_hint(int last,int now);//判断是否是需要一条折线就可以消除的
    bool isAble_Twocorner_Clear_hint(int last,int now);//判断是否是需要两条折线就可以消除的


    //相关数据
    //player
    Player *mary = new Player(1);
    Player *tom = new Player(2);

    //player对应的box
    Box *box1 = new Box();
    Box *box2 = new Box();

    int box_score;


    //direct用于寻找是否能到这个位置
    struct direct{
        int a;
        int b;
    };

    direct direction[4]={
        {-1,0},{0,1},{1,0},{0,-1}
    };

    bool seekPath(int x, int y, int m ,int p);//回溯法函数
    int mark[ROWS+2][COLS+2]={{0}};

    //道具类方块的对应数据
    bool needRefresh = false;//是否需要更新画面
    bool needPlusTime = false;//是否需要启用+1s道具
    bool needHint = false;//是否需要启用hint道具
    bool needFlash = false;//是否需要启动移动道具
    bool needFreeze = false;//是否需要启动冻结对方道具
    bool needDizzy = false;//是否需要让对方的操作上下左右颠倒
    int freezeMan;//需要冻结哪一个
    int dizzyMan;//需要颠倒哪一个
    bool canFlash;//所点击的位置能否flash
    int dataFlash = 9999;//dataFlash为点击的点
    int hintNum[2] = {0,0};//存入两个前后hint的坐标代数
    int flashReturn;
    int flashBeforetoNow[2];//到时候改一下内部两个数值所对应的图片
    int drawLineNum;


private:

};


#endif // Game_Logic_H
