#include "game_logic.h"
#include <QDebug>

int gamedata[ROWS+2][COLS+2] ={
        1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1
};
bool is_show=false;


//
//将num号玩家的box激活
//接收num——哪一个玩家、boxx\boxy，存入的坐标
void Game_Logic::boxOn(int num,int boxx,int boxy)
{
    if (num == 1) {
        box1->lastx_on = boxx;
        box1->lasty_on = boxy;
        box1->isBox_on();//num_onBox++
    } else if (num == 2) {
        box2->lastx_on = boxx;
        box2->lasty_on = boxy;
        box2->isBox_on();
    }
}


//
//设定现在的激活方块的num
//接受num_哪一个玩家，num2_激活方块的num
//
void Game_Logic::numBox(int num,int num2)
{
    if(num == 1) {
       box1->numofbox = num2;
    } else if (num == 2) {
        box2->numofbox = num2;
    }
}


//
//设定前一个激活方块的num
//参数及功能类似上一个
//
void Game_Logic::num_LastBox(int num,int num2)
{
    if(num == 1) {
       box1->numofbox_last = num2;
    } else if(num==2) {
        box2->numofbox_last = num2;
    }
}


//
//初始化game
//接受mode——是初始化单人模式还是双人模式
//
Game_Logic::Game_Logic(int mode)
{
    needRefresh = false;
    needPlusTime = false;
    needHint = false;
    needFlash = false;
    if(mode == 1) {
        initGame();
        playerDire(*mary);
    }
    if(mode == 2) {
        initGame();
        initGame_double();
        playerDire(*mary);
        playerDire_second(*tom);
    }
}


//
//saveload的初始化game
//array是gamedata的、score为mary的分数
//
Game_Logic::Game_Logic(int array[],int score)
{
    needRefresh = false;
    needPlusTime = false;
    needHint = false;
    needFlash = false;
    mary->score=score;
    int q = 0;
    for(int i = 0;i < ROWS + 2;i++) {
        for(int j = 0;j < COLS + 2;j++) {
            gamedata[i][j] = array[q];
            q++;
            if(gamedata[i][j] == 999) {
                mary->nowx = i;
                mary->nowy = j;
                mary->lastx = i;
                mary->lasty = j;
            }
        }
    }

}


//
//双人模式下的初始化game
//score2为2号玩家的分数，其他同上
//
Game_Logic::Game_Logic(int array[],int score1,int score2)
{
    needRefresh = false;
    needPlusTime = false;
    needHint = false;
    needFlash = false;
    mary->score = score1;
    tom->score = score2;
    int q = 0;
    for(int i = 0;i < ROWS + 2;i++) {
        for(int j = 0;j < COLS + 2;j++) {
            gamedata[i][j] = array[q];
            q++;
            if(gamedata[i][j] == 999) {
                mary->nowx = i;
                mary->nowy = j;
                mary->lastx = i;
                mary->lasty = j;
            }
            if(gamedata[i][j] == 666) {
                tom->nowx = i;
                tom->nowy = j;
                tom->lastx = i;
                tom->lasty = j;
            }
        }
    }
}


//
//设定player的位置，让player的所在地为999
//接受哪一个玩家
//
void Game_Logic::playerDire(Player p)
{
    gamedata[p.nowx][p.nowy] = 999;
}


//
//设定player2的位置，让player的所在地为666
//同上
//
void Game_Logic::playerDire_second(Player p)
{
    gamedata[p.nowx][p.nowy] = 666;
}


//
//重排道具
//重排
//
void Game_Logic::refreshLogic()
{
    //打乱现有元素的顺序
    srand((unsigned)time(nullptr));
    for(int i = 0;i < ROWS * COLS;i++) {
         int x1 = rand() % COLS+1;
         int x2 = rand() % COLS+1;
         int y1 = rand() % ROWS+1;
         int y2 = rand() % ROWS+1;
         //玩家在的位置不要重排，直接跳过，玩家的四周也不要变动
         if(gamedata[x1][y1] == 999 || gamedata[x2][y2] == 999
                 ||gamedata[x1+1][y1] == 999 || gamedata[x1-1][y1] == 999
                 ||gamedata[x1][y1+1] == 999 || gamedata[x1][y1-1] == 999
                 ||gamedata[x2+1][y2] == 999 || gamedata[x2-1][y2] == 999
                 ||gamedata[x2][y2+1] == 999 || gamedata[x2][y2-1] == 999
                 ||gamedata[x1][y1] == 666 || gamedata[x2][y2] == 666
                 ||gamedata[x1+1][y1] == 666 || gamedata[x1-1][y1] == 666
                 ||gamedata[x1][y1+1] == 666 || gamedata[x1][y1-1] == 666
                 ||gamedata[x2+1][y2] == 666 || gamedata[x2-1][y2] == 666
                 ||gamedata[x2][y2+1] == 666 || gamedata[x2][y2-1] == 666) {
             continue;
         } else {
             int temp = gamedata[x1][y1];
             gamedata[x1][y1] = gamedata[x2][y2];
             gamedata[x2][y2] = temp;
         }
    }
    needRefresh = true;
}


//
//初始化地图
//初始化地图
//
void Game_Logic::initGame()
{
    //先保障数组内元素成对存在
    int temp = 0;
    for(int i = 1;i< ROWS + 1;i++) {
        for(int j = 1;j < COLS + 1;j++) {
            gamedata[i][j] = temp/2 +1;
            temp++;
        }
    }
    //保障四周道路通畅
    for(int i = 0;i <= ROWS + 1;i++) {
        gamedata[0][i] = 0;
        gamedata[ROWS+1][i] = 0;
        gamedata[i][0] = 0;
        gamedata[i][COLS+1] = 0;
    }
   //再打乱元素的顺序
   srand((unsigned)time(nullptr));
   for(int i = 0;i< ROWS * COLS;i++) {
        int x1 = rand() % COLS + 1;
        int x2 = rand() % COLS + 1;
        int y1 = rand() % ROWS + 1;
        int y2 = rand() % ROWS + 1;
        int temp = gamedata[y1][x1];
        gamedata[y1][x1] = gamedata[y2][x2];
        gamedata[y2][x2] = temp;
   }
}


//
//把地图替换一下double的道具
//双人模式初始化使用
//
void Game_Logic::initGame_double()
{
    int change=0;
    for(int i = 1;i < ROWS + 1;i++) {
        for(int j = 1;j < COLS + 1;j++) {
            if(gamedata[i][j] == 49) {
                if(change == 0) {
                    change++;
                    gamedata[i][j] = 105;//冻结
                } else {
                    gamedata[i][j] = 106;//混乱
                }
            }
        }
    }
}


//
//将指定位置的值清零
//a,b为指定位置，将之设置为0可通行
//
void Game_Logic::clearImage(int a,int b)
{
    box_score = gamedata[a][b];
    gamedata[a][b] = 0;
}


//
//增加分数
//接受player——哪一个玩家和num——score
//
void Game_Logic::scoreRenew(int player,int num)
{
    int num1 = num/100;
    int num2 = num-num1*100;
    if(player == 1) {
        mary->score = mary->score + (box_score % 9 + 1); //不同方块的分数不一样
    }
    if(player==2) {
        tom->score = tom->score + (box_score % 9 + 1); //不同方块的分数不一样
    }
}


//
//向上走，接受num——哪一个玩家，后续同此
//0是不能走、1是可以走，2是激活一个方块，3是两个方块消除掉，4是两个方块不匹配，5是两次点了同一个方块
//
//为了保证函数在50行以内，拆成两个函数
int Game_Logic::moveUp_num1()
{
    if(mary->nowy == 0) {
        mary->lastx = mary->nowx;
        mary->lasty = mary->nowy;//让现在的坐标位置变成前一个坐标位置
        return 0;
    } else if (mary->nowy != 0 && isWalkable(*mary,'A')) {//可以行动
        clearImage(mary->nowx,mary->nowy);//把player1的现在的坐标点对应的数字清零
        mary->lastx = mary->nowx;
        mary->lasty = mary->nowy;//把现坐标存入前一个坐标
        mary->nowy--;//让列数减一
        playerDire(*mary);//将player现在的direction对应数字变为999
        if(needFreeze) {
            freezeMan = 2;
        }
        if(needDizzy) {
            dizzyMan = 2;
        }
        return 1;
    } else if (mary->nowy != 0 && (!isWalkable(*mary,'A'))) {//可以左移动但是出现了方块挡道
        if(needFreeze) {
            freezeMan = 2;
        }
        if(needDizzy) {
            dizzyMan = 2;
        }
        mary->lasty = mary->nowy;
        mary->lastx = mary->nowx;//先把现在的坐标变成原来过去式
        if(box1->num_onBox == 0) {//如果激活数量为0
            boxOn(1,mary->nowx,mary->nowy-1);//将player左边的一个方块的坐标存入box的激活坐标当中
            num_LastBox(1,box1->lastx_on * 100+box1->lasty_on);//将该方块位置改变形式存入激活坐标中去(此时激活数量为0，存入lastbox)
            return 2;
        } else if(box1->num_onBox == 1) {//如果激活数量为1
            boxOn(1,mary->nowx,mary->nowy-1);//激活坐标更新，现在num_on应该为2
            numBox(1,box1->lastx_on * 100+box1->lasty_on);//将第二个方块的坐标更新存入numBox中去
            if(boxClear(1,box1->numofbox_last,box1->numofbox) == 1) {//如果两个方块消掉了，就返回3
                box1->num_onBox = 0;//让方块的激活数量变为0
                scoreRenew(1,box1->numofbox);//增加分数
                return 3;
            } else if(boxClear(1,box1->numofbox_last,box1->numofbox) == 2) {//如果两个方块不匹配
                box1->num_onBox = 0;//让激活的方块的数量变为0
                return 4;
            } else if(boxClear(1,box1->numofbox_last,box1->numofbox) == 0) {//如果两次点了同一个方块
                box1->num_onBox = 0;//让激活的方块的数量变为0
                return 5;
            }
        }
    }
}

int Game_Logic::moveUp_num2()
{
    if(tom->nowy == 0) {
        tom->lastx = tom->nowx;
        tom->lasty = tom->nowy;//让现在的坐标位置变成前一个坐标位置
        return 0;
    } else if(tom->nowy != 0 && isWalkable(*tom,'A')){//可以行动
        if(needFreeze) {
            freezeMan = 1;
        }
        if(needDizzy) {
            dizzyMan = 1;
        }
        clearImage(tom->nowx,tom->nowy);//把player1的现在的坐标点对应的数字清零
        tom->lastx = tom->nowx;
        tom->lasty = tom->nowy;//把现坐标存入前一个坐标
        tom->nowy--;//让列数减一
        playerDire_second(*tom);//将player现在的direction对应数字变为666
        return 1;
    } else if(tom->nowy != 0 && (!isWalkable(*tom,'A'))) {//可以左移动但是出现了方块挡道
        if(needFreeze) {
            freezeMan = 1;
        }
        if(needDizzy) {
            dizzyMan = 1;
        }
        tom->lasty = tom->nowy;
        tom->lastx = tom->nowx;//先把现在的坐标变成原来过去式
        if(box2->num_onBox == 0){//如果激活数量为0
            boxOn(2,tom->nowx,tom->nowy-1);//将player左边的一个方块的坐标存入box的激活坐标当中
            num_LastBox(2,box2->lastx_on*100+box2->lasty_on);//将该方块位置改变形式存入激活坐标中去(此时激活数量为0，存入lastbox)
            return 2;
        } else if(box2->num_onBox == 1) {//如果激活数量为1
            boxOn(2,tom->nowx,tom->nowy-1);//激活坐标更新，现在num_on应该为2
            numBox(2,box2->lastx_on*100+box2->lasty_on);//将第二个方块的坐标更新存入numBox中去
            if(boxClear(2,box2->numofbox_last,box2->numofbox) == 1) {//如果两个方块消掉了，就返回3
                box2->num_onBox = 0;//让方块的激活数量变为0
                scoreRenew(2,box2->numofbox);//增加分数
                return 3;
            } else if(boxClear(2,box2->numofbox_last,box2->numofbox)==2) {//如果两个方块不匹配
                box2->num_onBox=0;//让激活的方块的数量变为0
                return 4;
            } else if(boxClear(2,box2->numofbox_last,box2->numofbox)==0) {//如果两次点了同一个方块
                box2->num_onBox=0;//让激活的方块的数量变为0
                return 5;
            }
        }
    }
}

int Game_Logic::moveUp(int num)
{
    if(isAble_continue() == false) {
        return 888;
    }
    //num==1一号玩家时
    if(num == 1) {
        moveUp_num1();
    } else if(num==2) {
        moveUp_num2();
    }
}


//
//向下转
//同上
//
//为了保证函数在50行以内，拆成两个函数
int Game_Logic::moveDown_num1()
{
    if(mary->nowy == 11) {
        mary->lastx = mary->nowx;
        mary->lasty = mary->nowy;//让现在的坐标位置变成前一个坐标位置
        return 0;
    } else if(mary->nowy != 11 && isWalkable(*mary,'D')) {
        if(needFreeze) {
            freezeMan = 2;
        }
        if(needDizzy) {
            dizzyMan = 2;
        }
        clearImage(mary->nowx,mary->nowy);//把player1的现在的坐标点对应的数字清零
        mary->lastx = mary->nowx;
        mary->lasty = mary->nowy;//把现坐标存入前一个坐标
        mary->nowy++;//让列数加1
        playerDire(*mary);//将player现在的direction对应数字变为999
        return 1;
    } else if(mary->nowy != 11 && (!isWalkable(*mary,'D'))) {
        if(needFreeze) {
            freezeMan = 2;
        }
        if(needDizzy) {
            dizzyMan = 2;
        }
        mary->lasty = mary->nowy;
        mary->lastx = mary->nowx;//先把现在的坐标变成原来过去式
        if(box1->num_onBox == 0) {
            boxOn(1,mary->nowx,mary->nowy+1);//将player右边的一个方块的坐标存入box的激活坐标当中
            num_LastBox(1,box1->lastx_on*100+box1->lasty_on);//将该方块位置改变形式存入激活坐标中去(此时激活数量为0，存入lastbox)
            return 2;
        } else if(box1->num_onBox == 1) {
            boxOn(1,mary->nowx,mary->nowy+1);//激活坐标更新，现在num_on应该为2
            numBox(1,box1->lastx_on*100+box1->lasty_on);//将第二个方块的坐标更新存入numBox中去
            if(boxClear(1,box1->numofbox_last,box1->numofbox) == 1) {
                box1->num_onBox = 0;//让方块的激活数量变为0
                scoreRenew(1,box1->numofbox);//增加分数
                return 3;
            } else if(boxClear(1,box1->numofbox_last,box1->numofbox) == 2) {
                box1->num_onBox = 0;//让激活的方块的数量变为0
                return 4;
            } else if(boxClear(1,box1->numofbox_last,box1->numofbox)==0) {
                box1->num_onBox = 0;//让激活的方块的数量变为0
                return 5;
            }
        }
    }
}

int Game_Logic::moveDown_num2()
{
    if(tom->nowy == 11) {
        tom->lastx = tom->nowx;
        tom->lasty = tom->nowy;//让现在的坐标位置变成前一个坐标位置
        return 0;
    } else if(tom->nowy != 11&&isWalkable(*tom,'D')) {
        if(needFreeze) {
            freezeMan = 1;
        }
        if(needDizzy) {
            dizzyMan = 1;
        }
        clearImage(tom->nowx,tom->nowy);//把player1的现在的坐标点对应的数字清零
        tom->lastx = tom->nowx;
        tom->lasty = tom->nowy;//把现坐标存入前一个坐标
        tom->nowy++;//让列数加1
        playerDire_second(*tom);//将player现在的direction对应数字变为999
        return 1;
    } else if(tom->nowy!=11&&(!isWalkable(*tom,'D'))) {
        if(needFreeze) {
            freezeMan = 1;
        }
        if(needDizzy) {
            dizzyMan = 1;
        }
        tom->lasty = tom->nowy;
        tom->lastx = tom->nowx;//先把现在的坐标变成原来过去式
        if(box2->num_onBox == 0) {
            boxOn(2,tom->nowx,tom->nowy + 1);//将player右边的一个方块的坐标存入box的激活坐标当中
            num_LastBox(2,box2->lastx_on*100+box2->lasty_on);//将该方块位置改变形式存入激活坐标中去(此时激活数量为0，存入lastbox)
            return 2;
        } else if(box2->num_onBox == 1) {
            boxOn(2,tom->nowx,tom->nowy+1);//激活坐标更新，现在num_on应该为2
            numBox(2,box2->lastx_on*100+box2->lasty_on);//将第二个方块的坐标更新存入numBox中去
            if(boxClear(2,box2->numofbox_last,box2->numofbox) == 1) {
                box2->num_onBox=0;//让方块的激活数量变为0
                scoreRenew(2,box2->numofbox);//增加分数
                return 3;
            } else if(boxClear(2,box2->numofbox_last,box2->numofbox) == 2) {
                box2->num_onBox = 0;//让激活的方块的数量变为0
                return 4;
            } else if(boxClear(2,box2->numofbox_last,box2->numofbox) == 0) {
                box2->num_onBox = 0;//让激活的方块的数量变为0
                return 5;
            }
        }
    }
}

int Game_Logic::moveDown(int num)
{
    if(isAble_continue() == false) {
        return 888;
    }
    if(num == 1) {
        moveDown_num1();
    } else if(num == 2) {
        moveDown_num2();
    }
}


//
//向左走
//同上
//为了保证函数在50行以内，拆成两个函数
//
int Game_Logic::moveLeft_num1()
{
    if(mary->nowx == 0) {
        mary->lastx = mary->nowx;
        mary->lasty = mary->nowy;//让现在的坐标位置变成前一个坐标位置
        return 0;
    }
    else if(mary->nowx != 0 && isWalkable(*mary,'W')) {
        if(needFreeze){
            freezeMan = 2;
        }
        if(needDizzy) {
            dizzyMan = 2;
        }
        clearImage(mary->nowx,mary->nowy);//把player1的现在的坐标点对应的数字清零
        mary->lastx = mary->nowx;
        mary->lasty = mary->nowy;//把现坐标存入前一个坐标
        mary->nowx--;//让行数减1
        playerDire(*mary);//将player现在的direction对应数字变为999
        return 1;
    } else if(mary->nowx!=0&&(!isWalkable(*mary,'W'))) {
        if(needFreeze) {
            freezeMan = 2;
        }
        if(needDizzy) {
            dizzyMan = 2;
        }
        mary->lasty = mary->nowy;
        mary->lastx = mary->nowx;//先把现在的坐标变成原来过去式
        if(box1->num_onBox == 0) {
            boxOn(1,mary->nowx-1,mary->nowy);//将player上边的一个方块的坐标存入box的激活坐标当中
            num_LastBox(1,box1->lastx_on*100+box1->lasty_on);//将该方块位置改变形式存入激活坐标中去(此时激活数量为0，存入lastbox)
            return 2;
        } else if(box1->num_onBox == 1) {
            boxOn(1,mary->nowx-1,mary->nowy);//激活坐标更新，现在num_on应该为2
            numBox(1,box1->lastx_on*100+box1->lasty_on);//将第二个方块的坐标更新存入numBox中去
            if(boxClear(1,box1->numofbox_last,box1->numofbox) == 1) {
                box1->num_onBox = 0;//让方块的激活数量变为0
                scoreRenew(1,box1->numofbox);//增加分数
                return 3;
            } else if(boxClear(1,box1->numofbox_last,box1->numofbox) == 2) {
                box1->num_onBox = 0;//让激活的方块的数量变为0
                return 4;
            } else if(boxClear(1,box1->numofbox_last,box1->numofbox) == 0) {
                box1->num_onBox = 0;//让激活的方块的数量变为0
                return 5;
            }
        }
    }
}

int Game_Logic::moveLeft_num2()
{
    if(tom->nowx == 0) {
        tom->lastx = tom->nowx;
        tom->lasty = tom->nowy;//让现在的坐标位置变成前一个坐标位置
        return 0;
    } else if(tom->nowx != 0 && isWalkable(*tom,'W')) {
        if(needFreeze) {
            freezeMan = 1;
        }
        if(needDizzy) {
            dizzyMan = 1;
        }
        clearImage(tom->nowx,tom->nowy);//把player1的现在的坐标点对应的数字清零
        tom->lastx = tom->nowx;
        tom->lasty = tom->nowy;//把现坐标存入前一个坐标
        tom->nowx--;//让行数减1
        playerDire_second(*tom);//将player现在的direction对应数字变为999
        return 1;
    } else if(tom->nowx != 0 && (!isWalkable(*tom,'W'))) {
        if(needFreeze) {
            freezeMan = 1;
        }
        if(needDizzy) {
            dizzyMan = 1;
        }
        tom->lasty = tom->nowy;
        tom->lastx = tom->nowx;//先把现在的坐标变成原来过去式
        if(box2->num_onBox == 0) {
            boxOn(2,tom->nowx-1,tom->nowy);//将player上边的一个方块的坐标存入box的激活坐标当中
            num_LastBox(2,box2->lastx_on*100+box2->lasty_on);//将该方块位置改变形式存入激活坐标中去(此时激活数量为0，存入lastbox)
            return 2;
        } else if(box2->num_onBox == 1){
            boxOn(2,tom->nowx-1,tom->nowy);//激活坐标更新，现在num_on应该为2
            numBox(2,box2->lastx_on*100+box2->lasty_on);//将第二个方块的坐标更新存入numBox中去
            if(boxClear(2,box2->numofbox_last,box2->numofbox) == 1) {
                box2->num_onBox = 0;//让方块的激活数量变为0
                scoreRenew(2,box2->numofbox);//增加分数
                return 3;
            } else if(boxClear(2,box2->numofbox_last,box2->numofbox) == 2) {
                box2->num_onBox = 0;//让激活的方块的数量变为0
                return 4;
            } else if(boxClear(2,box2->numofbox_last,box2->numofbox) == 0) {
                box2->num_onBox = 0;//让激活的方块的数量变为0
                return 5;
            }
        }
    }
}

int Game_Logic::moveLeft(int num)
{
    if(isAble_continue() == false) {
        return 888;
    }
    if(num == 1) {
        moveLeft_num1();
    } else if(num == 2) {
        moveLeft_num2();
    }
}


//
//向右边走
//同上
//为了保证函数在50行以内，拆成两个函数
//
int Game_Logic::moveRight_num1()
{
    if(mary->nowx == 11) {
        mary->lastx = mary->nowx;
        mary->lasty = mary->nowy;//让现在的坐标位置变成前一个坐标位置
        return 0;
    } else if(mary->nowx != 11 && isWalkable(*mary,'S')) {
        if(needFreeze) {
            freezeMan = 2;
        }
        if(needDizzy) {
            dizzyMan = 2;
        }
        clearImage(mary->nowx,mary->nowy);//把player1的现在的坐标点对应的数字清零
        mary->lastx = mary->nowx;
        mary->lasty = mary->nowy;//把现坐标存入前一个坐标
        mary->nowx++;//让行数加1
        playerDire(*mary);//将player现在的direction对应数字变为999
        return 1;
    } else if(mary->nowx != 11 && (!isWalkable(*mary,'S'))) {
        if(needFreeze) {
            freezeMan = 2;
        }
        if(needDizzy) {
            dizzyMan = 2;
        }
        mary->lasty = mary->nowy;
        mary->lastx = mary->nowx;//先把现在的坐标变成原来过去式
        if(box1->num_onBox == 0) {
            boxOn(1,mary->nowx+1,mary->nowy);//将player右边的一个方块的坐标存入box的激活坐标当中
            num_LastBox(1,box1->lastx_on*100+box1->lasty_on);//将该方块位置改变形式存入激活坐标中去(此时激活数量为0，存入lastbox)
            return 2;
        } else if(box1->num_onBox == 1) {
            boxOn(1,mary->nowx+1,mary->nowy);//激活坐标更新，现在num_on应该为2
            numBox(1,box1->lastx_on*100+box1->lasty_on);//将第二个方块的坐标更新存入numBox中去
            if(boxClear(1,box1->numofbox_last,box1->numofbox) == 1) {
                box1->num_onBox=0;//让方块的激活数量变为0
                scoreRenew(1,box1->numofbox);//增加分数
                return 3;
            } else if(boxClear(1,box1->numofbox_last,box1->numofbox) == 2){
                box1->num_onBox = 0;//让激活的方块的数量变为0
                return 4;
            } else if(boxClear(1,box1->numofbox_last,box1->numofbox) == 0) {
                box1->num_onBox = 0;//让激活的方块的数量变为0
                return 5;
            }
        }
    }
}

int Game_Logic::moveRight_num2()
{
    if(tom->nowx == 11) {
        tom->lastx = tom->nowx;
        tom->lasty = tom->nowy;//让现在的坐标位置变成前一个坐标位置
        return 0;
    } else if(tom->nowx != 11 && isWalkable(*tom,'S')) {
        if(needFreeze) {
            freezeMan = 1;
        }
        if(needDizzy) {
            dizzyMan = 1;
        }
        clearImage(tom->nowx,tom->nowy);//把player1的现在的坐标点对应的数字清零
        tom->lastx = tom->nowx;
        tom->lasty = tom->nowy;//把现坐标存入前一个坐标
        tom->nowx++;//让行数加1
        playerDire_second(*tom);//将player现在的direction对应数字变为999
        return 1;
    } else if(tom->nowx != 11 && (!isWalkable(*tom,'S'))) {
        if(needFreeze) {
            freezeMan = 1;
        }
        if(needDizzy) {
            dizzyMan = 1;
        }
        tom->lasty = tom->nowy;
        tom->lastx = tom->nowx;//先把现在的坐标变成原来过去式
        if(box2->num_onBox == 0) {
            boxOn(2,tom->nowx+1,tom->nowy);//将player右边的一个方块的坐标存入box的激活坐标当中
            num_LastBox(2,box2->lastx_on*100+box2->lasty_on);//将该方块位置改变形式存入激活坐标中去(此时激活数量为0，存入lastbox)
            return 2;
        } else if(box2->num_onBox == 1) {
            boxOn(2,tom->nowx+1,tom->nowy);//激活坐标更新，现在num_on应该为2
            numBox(2,box2->lastx_on*100+box2->lasty_on);//将第二个方块的坐标更新存入numBox中去
            if(boxClear(2,box2->numofbox_last,box2->numofbox) == 1) {
                box2->num_onBox = 0;//让方块的激活数量变为0
                scoreRenew(2,box2->numofbox);//增加分数
                return 3;
            } else if(boxClear(2,box2->numofbox_last,box2->numofbox) == 2) {
                box2->num_onBox = 0;//让激活的方块的数量变为0
                return 4;
            } else if(boxClear(2,box2->numofbox_last,box2->numofbox) == 0) {
                box2->num_onBox = 0;//让激活的方块的数量变为0
                return 5;
            }
        }
    }
}

int Game_Logic::moveRight(int num)
{
    if(isAble_continue() == false) {
        return 888;
    }
    if(num == 1) {
        moveRight_num1();
    } else if(num == 2) {
        moveRight_num2();
    }
}


//
//判断player能否运动
//如果说是重排道具等道具的话，那么也要返回1，1是可以走，0是不能走
//为了保证在五十行内，分成四个函数
//
bool Game_Logic::isWalkable_W(Player p)
{
    if(gamedata[p.nowx-1][p.nowy] == 48){
        refreshLogic();
        return 1;
    } else if(gamedata[p.nowx-1][p.nowy] == 47) {
        needPlusTime = true;
        return 1;
    } else if(gamedata[p.nowx-1][p.nowy] == 49){
        needFlash = true;
        flashLogic();
        return 1;
    } else if(gamedata[p.nowx-1][p.nowy] == 50) {
        hintLogic();
        needHint=true;
        return 1;
    } else if(gamedata[p.nowx-1][p.nowy] == 105) {
        needFreeze = true;
        return 1;
    } else if(gamedata[p.nowx-1][p.nowy] == 106) {
        needDizzy = true;
        return 1;
    } else if(gamedata[p.nowx-1][p.nowy] != 0 && gamedata[p.nowx-1][p.nowy] != 666 && gamedata[p.nowx-1][p.nowy] != 999) {
        return 0;
    } else {
        return 1;
    }
}

bool Game_Logic::isWalkable_S(Player p)
{
    if(gamedata[p.nowx+1][p.nowy] == 48) {
        refreshLogic();
        return 1;
    } else if(gamedata[p.nowx+1][p.nowy] == 47) {
        needPlusTime = true;
        return 1;
    } else if(gamedata[p.nowx+1][p.nowy] == 49) {
        needFlash = true;
        flashLogic();
        return 1;
    } else if(gamedata[p.nowx+1][p.nowy] == 50) {
        hintLogic();
        needHint = true;
        return 1;
    } else if(gamedata[p.nowx+1][p.nowy] == 105) {
        needFreeze = true;
        return 1;
    } else if(gamedata[p.nowx+1][p.nowy] == 106) {
        needDizzy = true;
        return 1;
    } else if(gamedata[p.nowx+1][p.nowy] != 0 && gamedata[p.nowx+1][p.nowy] != 666 && gamedata[p.nowx+1][p.nowy] != 999) {
        return 0;
    } else {
        return 1;
    }
}

bool Game_Logic::isWalkable_A(Player p)
{
    if(gamedata[p.nowx][p.nowy-1] == 48) {
        refreshLogic();
        return 1;
    }  else if(gamedata[p.nowx][p.nowy-1] == 47) {
        needPlusTime = true;
        return 1;
    } else if(gamedata[p.nowx][p.nowy-1] == 49) {
        needFlash = true;
        flashLogic();
        return 1;
    } else if(gamedata[p.nowx][p.nowy-1] == 50) {
        hintLogic();
        needHint = true;
        return 1;
    } else if(gamedata[p.nowx][p.nowy-1] == 105) {
        needFreeze = true;
        return 1;
    } else if(gamedata[p.nowx][p.nowy-1] == 106) {
        needDizzy = true;
        return 1;
    } else if(gamedata[p.nowx][p.nowy-1] != 0 && gamedata[p.nowx][p.nowy-1] != 666 && gamedata[p.nowx][p.nowy-1] != 999) {
        return 0;
    } else {
        return 1;
    }
}

bool Game_Logic::isWalkable_D(Player p)
{
    if(gamedata[p.nowx][p.nowy+1] == 48) {
        refreshLogic();
        return 1;
    } else if(gamedata[p.nowx][p.nowy+1] == 47) {
        needPlusTime = true;
        return 1;
    } else if(gamedata[p.nowx][p.nowy+1] == 49) {
        needFlash = true;
        flashLogic();
        return 1;
    } else if(gamedata[p.nowx][p.nowy+1] == 50) {
        hintLogic();
        needHint = true;
        return 1;
    } else if(gamedata[p.nowx][p.nowy+1] == 105) {
        needFreeze = true;
        return 1;
    } else if(gamedata[p.nowx][p.nowy+1] == 106) {
        needDizzy = true;
        return 1;
    } else if(gamedata[p.nowx][p.nowy+1] != 0 && gamedata[p.nowx][p.nowy+1] != 666 && gamedata[p.nowx][p.nowy+1] != 999) {
        return 0;
    } else {
        return 1;
    }
}

bool Game_Logic::isWalkable(Player p,char direction)
{
    if(direction == 'W') {
        bool isWalkable;
        isWalkable = isWalkable_W(p);
        return isWalkable;
    }
    if(direction == 'A') {
        bool isWalkable;
        isWalkable = isWalkable_A(p);
        return isWalkable;
    }
    if(direction == 'D') {
        bool isWalkable;
        isWalkable = isWalkable_D(p);
        return isWalkable;
    }
    if(direction == 'S') {
        bool isWalkable;
        isWalkable = isWalkable_S(p);
        return isWalkable;
    }
}


//
//boxClear
//两次激活相同返回零，可以消除返回1，不可以消除返回2
//
int Game_Logic::boxClear(int mode,int last,int now)
{
    int num1 = last/100;
    int num2 = last-num1*100;
    int num3 = now/100;
    int num4 = now-num3*100;
    if((num1 == num3) && (num2 == num4)) {
        return 0;//如果两次激活相同，那么就返回0
    } else if(((gamedata[num1][num2] % 9 + 1) == (gamedata[num3][num4] % 9 + 1))
            && (isClearable(mode,last,now) == true)) {
        if(mode == 1) {
            box1->numofbox_now_toDraw = gamedata[num3][num4];
            clearImage(num1,num2);
            clearImage(num3,num4);
            return 1;//如果两次激活对应方块形态相同并且是可以清理的，那么就清除方块返回1
        } else if(mode == 2) {
            box2->numofbox_now_toDraw = gamedata[num3][num4];
            clearImage(num1,num2);
            clearImage(num3,num4);
            return 1;//如果两次激活对应方块形态相同并且是可以清理的，那么就清除方块返回1
        }
    } else {
        return 2;//不相同就返回2
    }
}


//
//判断是否是不需要折线就可以消除的
//能消除返回true,不能返回false
//
bool Game_Logic::isAble_Nocorner_Clear(int mode,int last,int now)
{
    int last1 = last/100;
    int last2 = last-last1*100;
    int now1 = now/100;
    int now2 = now-now1*100;
    if(last1 == now1) {
        //如果说是相邻的，那么是可以消去的
        if(last2-now2 == 1 || now2-last2 == 1) {
            if(mode == 1) {
                box1->Line_Nocorner[0][0] = last;
                box1->Line_Nocorner[0][1] = now;
                return true;
            } else if(mode == 2) {
                box2->Line_Nocorner[0][0] = last;
                box2->Line_Nocorner[0][1] = now;
                return true;
            }
        } else {
            //找到要查询的初始位置和结束位置
            int begin = last2 < now2 ? last2 : now2;
            int final = last2 + now2 - begin;
            for(int i = begin + 1;i <= final - 1;i++) {
                if(gamedata[last1][i] != 0 && gamedata[last1][i] != 999 && gamedata[last1][i] != 666) {
                    return false;//那么中间就是有阻碍的，过不去
                }
            }
            if(mode == 1) {
                box1->Line_Nocorner[0][0] = last;
                box1->Line_Nocorner[0][1] = now;
                return true;
            } else if(mode == 2) {
                box2->Line_Nocorner[0][0] = last;
                box2->Line_Nocorner[0][1] = now;
                return true;
            }
        }
    } else if(last2 == now2) {
        if(last1-now1 == 1 || now1-last1 == 1) {
            if(mode == 1) {
                box1->Line_Nocorner[0][0] = last;
                box1->Line_Nocorner[0][1] = now;
                return true;
            } else if(mode==2) {
                box2->Line_Nocorner[0][0] = last;
                box2->Line_Nocorner[0][1] = now;
                return true;
            }
        } else {
            //找到要查询的初始位置和结束位置
            int begin = last1 < now1 ? last1 : now1;
            int final = last1 + now1 - begin;
            for(int i = begin + 1;i <= final - 1;i++) {
                if(gamedata[i][last2] != 0 && gamedata[i][last2] != 999 && gamedata[i][last2] != 666) {
                    return false;//那么中间就是有阻碍的，过不去
                }
            }
            if(mode == 1) {
                box1->Line_Nocorner[0][0] = last;
                box1->Line_Nocorner[0][1] = now;
                return true;
            } else if(mode == 2) {
                box2->Line_Nocorner[0][0] = last;
                box2->Line_Nocorner[0][1] = now;
                return true;
            }
        }
    }
    return false;//如果说两个点不在一个直线上，那么就不可能不要折线就消除掉
}


//
//判断是否是不需要折线就可以消除的_hint用
//类似于上一个函数，但是不需要存用来画线的点
//
bool Game_Logic::isAble_Nocorner_Clear_hint(int last,int now)
{
    int last1 = last / 100;
    int last2 = last - last1 * 100;
    int now1 = now / 100;
    int now2 = now - now1 * 100;
    if(last1 == now1) {
        //如果说是相邻的，那么是可以消去的
        if(last2 - now2 == 1 || now2 - last2 == 1) {
            return true;
        } else {
            //找到要查询的初始位置和结束位置
            int begin = last2 < now2 ? last2 : now2;
            int final = last2 + now2 - begin;
            for(int i = begin + 1 ; i <= final - 1 ; i++) {
                if(gamedata[last1][i] != 0 && gamedata[last1][i] != 999 && gamedata[last1][i] != 666) {
                    return false;//那么中间就是有阻碍的，过不去
                }
            }
            return true;
        }
    } else if(last2 == now2) {
        if(last1 - now1 == 1 || now1 - last1 == 1) {
            return true;
        } else {
            //找到要查询的初始位置和结束位置
            int begin = last1 < now1 ? last1 : now1;
            int final = last1 + now1 - begin;
            for(int i = begin + 1;i <= final - 1;i++) {
                if(gamedata[i][last2] != 0 && gamedata[i][last2] != 999 && gamedata[i][last2] != 666) {
                    return false;//那么中间就是有阻碍的，过不去
                }
            }
            return true;
        }
    }
    return false;//如果说两个点不在一个直线上，那么就不可能不要折线就消除掉
}


//
//判断是否是需要一条折线就可以消除的
//接受last和now两个点，判断能都折线一次消除
//
bool Game_Logic::isAble_Onecorner_Clear_hint(int last,int now)
{
    int last1 = last / 100;
    int last2 = last - last1 * 100;
    int now1 = now / 100;
    int now2=now-now1*100;
    if(last1 == now1 || last2 == now2) {
        return false;//如果说两个横纵坐标有相同的，那么就不可能一条折线就可以消除
    }
    //如果说一条折线可以消除的话，那么，last1,now2或者在last2,now1的情况下发生曲折
    if(gamedata[last1][now2] == 0 || gamedata[last1][now2] == 999 || gamedata[last1][now2] == 666) {
        int mid = last1 * 100 + now2;//确定中间点位的代数
        if(isAble_Nocorner_Clear_hint(last,mid)
                &&isAble_Nocorner_Clear_hint(mid,now)) {
            return true;
        }
    }
    if(gamedata[now1][last2] == 0 || gamedata[now1][last2] == 999 || gamedata[now1][last2] == 666) {
        int mid = now1 * 100 + last2;//确定中间点位的代数
        if(isAble_Nocorner_Clear_hint(last,mid)
                &&isAble_Nocorner_Clear_hint(mid,now)) {
            return true;
        }
    }
    return false;
}


//
//判断是否是需要两条折线就可以消除的
//接受last和now两个坐标点，返回能否折线两次消除
//
bool Game_Logic::isAble_Twocorner_Clear_hint(int last,int now)
{
    int last1 = last / 100;
    int last2 = last - last1 * 100;
    int now1 = now / 100;
    int now2 = now - now1 * 100;
    //如果说折两次线可以消除的话，那么一定会有一个点和其中一个点呈现noCorner,而和另外一个点呈现oneCorner
    //先假设这个点的x与第一个点的x相同，那么这个点的y需要在一个能和第二个点形成一次折线的位置上
    for(int i = 0 ;i <= ROWS + 1 ; i ++) {
        if(i != last1 && i != now1 //先是这个特殊的折点和两个点不在一个x轴上
                && (gamedata[i][last2] == 0 || gamedata[i][last2] == 999 || gamedata[i][last2] == 666)
                && (gamedata[i][now2] == 0 || gamedata[i][now2] == 999 || gamedata[i][now2] == 666)//加上这个i关于的两个点都是等于0或者有玩家在的
                && isAble_Onecorner_Clear_hint(i*100+last2,now)//可以和第二个点有一个折线链接
                && isAble_Nocorner_Clear_hint(last,i*100+last2)) {
            return true;
        }
        if(i != last1 && i != now1
                && (gamedata[i][last2] == 0 || gamedata[i][last2] == 999 || gamedata[i][last2] == 666)
                && (gamedata[i][now2] == 0 || gamedata[i][now2] == 999 || gamedata[i][now2] == 666)//加上这个i关于的两个点都是等于0或者有玩家在的
                && isAble_Onecorner_Clear_hint(last,i*100+now2)
                && isAble_Nocorner_Clear_hint(i*100+now2,now)) {
            return true;
        }
    }
    for(int i = 0 ; i <= COLS + 1 ; i ++) {
        if(i != last2 && i != now2
                && (gamedata[last1][i] == 0 || gamedata[last1][i] == 999 || gamedata[last1][i] == 666)
                && (gamedata[now1][i] == 0 || gamedata[now1][i] == 999 || gamedata[now1][i] == 666)
                && isAble_Onecorner_Clear_hint(last1 * 100 + i,now)
                && isAble_Nocorner_Clear_hint(last,last1 * 100 + i)) {
            return true;
        }
        if(i != last2 && i != now2
                && (gamedata[last1][i] == 0 || gamedata[last1][i] == 999 || gamedata[last1][i] == 666)
                && (gamedata[now1][i] == 0 || gamedata[now1][i] == 999 || gamedata[now1][i] == 666)
                && isAble_Onecorner_Clear_hint(last,now1 * 100 + i)
                && isAble_Nocorner_Clear_hint(now1 * 100 + i,now)) {
            return true;
        }
    }
    return false;
}


//
//判断是否是需要一条折线就可以消除的
//接受mode_哪个玩家last和now前后点位，返回能否折线折一次消除
//
bool Game_Logic::isAble_Onecorner_Clear(int mode,int last,int now)
{
    int last1 = last / 100;
    int last2 = last - last1 * 100;
    int now1 = now / 100;
    int now2 = now - now1 * 100;
    if(last1 == now1 || last2 == now2) {
        return false;//如果说两个横纵坐标有相同的，那么就不可能一条折线就可以消除
    }
    //如果说一条折线可以消除的话，那么，last1,now2或者在last2,now1的情况下发生曲折
    if(gamedata[last1][now2] == 0 || gamedata[last1][now2] == 999 || gamedata[last1][now2] == 666) {
        int mid = last1 * 100 + now2;//确定中间点位的代数
        if(isAble_Nocorner_Clear(mode,last,mid)
                &&isAble_Nocorner_Clear(mode,mid,now)) {
            if(mode == 1) {
                box1->Line_Onecorner[0][0] = last;
                box1->Line_Onecorner[0][1] = mid;
                box1->Line_Onecorner[1][0] = mid;
                box1->Line_Onecorner[1][1] = now;
                return true;
            } else if(mode == 2) {
                box2->Line_Onecorner[0][0] = last;
                box2->Line_Onecorner[0][1] = mid;
                box2->Line_Onecorner[1][0] = mid;
                box2->Line_Onecorner[1][1] = now;
                return true;
            }
        }
    }
    if(gamedata[now1][last2]==0||gamedata[now1][last2]==999||gamedata[now1][last2]==666)
    {
        int mid=now1*100+last2;//确定中间点位的代数
        if(isAble_Nocorner_Clear(mode,last,mid)
                &&isAble_Nocorner_Clear(mode,mid,now)) {
            if(mode == 1) {
                box1->Line_Onecorner[0][0] = last;
                box1->Line_Onecorner[0][1] = mid;
                box1->Line_Onecorner[1][0] = mid;
                box1->Line_Onecorner[1][1] = now;
                return true;
            } else if(mode == 2) {
                box2->Line_Onecorner[0][0] = last;
                box2->Line_Onecorner[0][1] = mid;
                box2->Line_Onecorner[1][0] = mid;
                box2->Line_Onecorner[1][1] = now;
                return true;
            }
        }
    }
    return false;
}


//
//判断是否是需要两条折线就可以消除的，接受参数mode-一号还是二号玩家，last和now上一个与下一个点的代数
//能折线两次消除就是true
//为了保证函数小于五十行，分拆几个下来
//
void Game_Logic::isAble_Twocorner_Clear_Box1(int mode,int last,int mid_line)
{
    if(mode == 1) {
        box1->Line_Twocorner[0][0] = last;
        box1->Line_Twocorner[0][1] = mid_line;
        box1->Line_Twocorner[1][0] = box1->Line_Onecorner[0][0];
        box1->Line_Twocorner[1][1] = box1->Line_Onecorner[0][1];
        box1->Line_Twocorner[2][0] = box1->Line_Onecorner[1][0];
        box1->Line_Twocorner[2][1] = box1->Line_Onecorner[1][1];
    } else if(mode == 2) {
        box2->Line_Twocorner[0][0] = last;
        box2->Line_Twocorner[0][1] = mid_line;
        box2->Line_Twocorner[1][0] = box2->Line_Onecorner[0][0];
        box2->Line_Twocorner[1][1] = box2->Line_Onecorner[0][1];
        box2->Line_Twocorner[2][0] = box2->Line_Onecorner[1][0];
        box2->Line_Twocorner[2][1] = box2->Line_Onecorner[1][1];
    }
}

void Game_Logic::isAble_Twocorner_Clear_Box2(int mode)
{
    if(mode == 1) {
        box1->Line_Twocorner[0][0] = box1->Line_Onecorner[0][0];
        box1->Line_Twocorner[0][1] = box1->Line_Onecorner[0][1];
        box1->Line_Twocorner[1][0] = box1->Line_Onecorner[1][0];
        box1->Line_Twocorner[1][1] = box1->Line_Onecorner[1][1];
        box1->Line_Twocorner[2][0] = box1->Line_Nocorner[0][0];
        box1->Line_Twocorner[2][1] = box1->Line_Nocorner[0][1];
    } else if(mode == 2) {
        box2->Line_Twocorner[0][0] = box2->Line_Onecorner[0][0];
        box2->Line_Twocorner[0][1] = box2->Line_Onecorner[0][1];
        box2->Line_Twocorner[1][0] = box2->Line_Onecorner[1][0];
        box2->Line_Twocorner[1][1] = box2->Line_Onecorner[1][1];
        box2->Line_Twocorner[2][0] = box2->Line_Nocorner[0][0];
        box2->Line_Twocorner[2][1] = box2->Line_Nocorner[0][1];
    }
}

void Game_Logic::isAble_Twocorner_Clear_Box3(int mode,int last,int mid_line)
{
    if(mode == 1) {
        box1->Line_Twocorner[0][0] = last;
        box1->Line_Twocorner[0][1] = mid_line;
        box1->Line_Twocorner[1][0] = box1->Line_Onecorner[0][0];
        box1->Line_Twocorner[1][1] = box1->Line_Onecorner[0][1];
        box1->Line_Twocorner[2][0] = box1->Line_Onecorner[1][0];
        box1->Line_Twocorner[2][1] = box1->Line_Onecorner[1][1];
    } else if(mode == 2) {
        box2->Line_Twocorner[0][0] = last;
        box2->Line_Twocorner[0][1] = mid_line;
        box2->Line_Twocorner[1][0] = box2->Line_Onecorner[0][0];
        box2->Line_Twocorner[1][1] = box2->Line_Onecorner[0][1];
        box2->Line_Twocorner[2][0] = box2->Line_Onecorner[1][0];
        box2->Line_Twocorner[2][1] = box2->Line_Onecorner[1][1];
    }
}

void Game_Logic::isAble_Twocorner_Clear_Box4(int mode)
{
    if(mode == 1) {
        box1->Line_Twocorner[0][0] = box1->Line_Onecorner[0][0];
        box1->Line_Twocorner[0][1] = box1->Line_Onecorner[0][1];
        box1->Line_Twocorner[1][0] = box1->Line_Onecorner[1][0];
        box1->Line_Twocorner[1][1] = box1->Line_Onecorner[1][1];
        box1->Line_Twocorner[2][0] = box1->Line_Nocorner[0][0];
        box1->Line_Twocorner[2][1] = box1->Line_Nocorner[0][1];
        box1->Line_Twocorner[3][0] = box1->Line_Onecorner[2][0];
        box1->Line_Twocorner[3][1] = box1->Line_Onecorner[2][1];//三根线有了，现在需要两个点
        box1->Line_Twocorner[3][2] = box1->Line_Nocorner[1][0];
        box1->Line_Twocorner[3][3] = box1->Line_Onecorner[2][2];//第一个拐点找到了
    } else if(mode == 2) {
        box2->Line_Twocorner[0][0] = box2->Line_Onecorner[0][0];
        box2->Line_Twocorner[0][1] = box2->Line_Onecorner[0][1];
        box2->Line_Twocorner[1][0] = box2->Line_Onecorner[1][0];
        box2->Line_Twocorner[1][1] = box2->Line_Onecorner[1][1];
        box2->Line_Twocorner[2][0] = box2->Line_Nocorner[0][0];
        box2->Line_Twocorner[2][1] = box2->Line_Nocorner[0][1];
        box2->Line_Twocorner[3][0] = box2->Line_Onecorner[2][0];
        box2->Line_Twocorner[3][1] = box2->Line_Onecorner[2][1];//三根线有了，现在需要两个点
        box2->Line_Twocorner[3][2] = box2->Line_Nocorner[1][0];
        box2->Line_Twocorner[3][3] = box2->Line_Onecorner[2][2];//第一个拐点找到了
    }
}

bool Game_Logic::isAble_Twocorner_Clear(int mode,int last,int now)
{
    int last1 = last/100;
    int last2 = last-last1*100;
    int now1 = now/100;
    int now2 = now-now1*100;
    //如果说折两次线可以消除的话，那么一定会有一个点和其中一个点呈现noCorner,而和另外一个点呈现oneCorner
    //先假设这个点的x与第一个点的x相同，那么这个点的y需要在一个能和第二个点形成一次折线的位置上
    for(int i=0;i<=ROWS+1;i++) {
        if(i != last1 && i != now1 //先是这个特殊的折点和两个点不在一个x轴上
                && (gamedata[i][last2] == 0 || gamedata[i][last2] == 999 || gamedata[i][last2] == 666)
                && (gamedata[i][now2] == 0 || gamedata[i][now2] == 999 || gamedata[i][now2] == 666)//加上这个i关于的两个点都是等于0或者有玩家在的
                && isAble_Onecorner_Clear(mode,i*100+last2,now)//可以和第二个点有一个折线链接
                && isAble_Nocorner_Clear(mode,last,i*100+last2)) {//在加上这个点可以和第一个点无折线链接
            int mid_line = i * 100 + last2;
            isAble_Twocorner_Clear_Box1(mode,last,mid_line);
            return true;
        }
        if(i != last1 && i != now1
                && (gamedata[i][last2] == 0|| gamedata[i][last2] == 999 || gamedata[i][last2] == 666)
                && (gamedata[i][now2] == 0 || gamedata[i][now2] == 999 || gamedata[i][now2] == 666)//加上这个i关于的两个点都是等于0或者有玩家在的
                && isAble_Onecorner_Clear(mode,last,i*100+now2)
                && isAble_Nocorner_Clear(mode,i*100+now2,now)) {//先corner，再直线
            int mid_line = i * 100 + now2;
            isAble_Twocorner_Clear_Box2(mode);
            return true;
        }
    }
    for(int i=0;i<=COLS+1;i++) {//换个方向，同上
        if(i!=last2 && i!=now2
                && (gamedata[last1][i] == 0 || gamedata[last1][i] == 999 || gamedata[last1][i] == 666)
                && (gamedata[now1][i] == 0 || gamedata[now1][i] == 999 || gamedata[now1][i] == 666)
                && isAble_Onecorner_Clear(mode,last1 * 100 + i,now)
                && isAble_Nocorner_Clear(mode,last,last1 * 100 + i)) {//先直线再corner
            int mid_line = last1 * 100 + i;
            isAble_Twocorner_Clear_Box3(mode,last,mid_line);
            return true;
        }
        if(i != last2 && i != now2
                && (gamedata[last1][i] == 0 || gamedata[last1][i]==999 || gamedata[last1][i] == 666)
                && (gamedata[now1][i] == 0 || gamedata[now1][i] == 999 || gamedata[now1][i] == 666)
                && isAble_Onecorner_Clear(mode,last,now1*100+i)
                && isAble_Nocorner_Clear(mode,now1*100+i,now)) {
            int mid_line = now1 * 100 + i;
            isAble_Twocorner_Clear_Box4(mode);
            return true;
        }
    }
    return false;
}


//
//判断是否还有可以消除的方块
//返回真假，不能消除返回假
//
bool Game_Logic::isAble_continue()
{
    //先找到玩家的位置
    //然后遍历四周不全是方块的位置，看这个位置有没有对应的方块，这里用类似于Hint的函数来确定
    int i = mary->nowx;
    int j = mary->nowy;
    int num_player = 100 * i + j;
    for(int m = 0; m < ROWS + 1; m ++) {
        for(int n = 0 ; n < COLS; n ++) {
            int numBox1 = m * 100 + n;
            if(isAble_player_to(numBox1)
                    && ((gamedata[m][n] >= 47 && gamedata[m][n] <= 50)
                    ||(gamedata[m][n] == 105)
                    ||(gamedata[m][n] == 106))) {
                return true;//如果说玩家能到的某个地方有道具，那么一定是可以继续游戏的
            }
            if((gamedata[m][n] > 0
                    && gamedata[m][n] < 47)
                    && (isAble_player_to(numBox1))) {//如果这个方块和player能相连
                    for(int k = 0 ; k < ROWS + 1; k ++) {
                        for(int l = 0;l < COLS + 1; l ++) {
                            int numBox2 = k * 100 + l;
                            if((isClearable(1,numBox1,numBox2))
                                    && (gamedata[k][l] > 0 && gamedata[k][l] < 47)
                                    && (gamedata[m][n] % 9 + 1 == gamedata[k][l] % 9 + 1)) {//如果说这个方块能和上一个相连并且相同的话，那就是可以相连的
                                return true;
                            }
                        }
                    }
            }
        }
    }
    return false;
}


//
//回溯法求玩家能否到mp处的函数函数，接受现在的坐标xy和目标点的mp
//返回真假
//
bool Game_Logic::seekPath(int x, int y, int m ,int p)
{
    int i, g, h; // g, h记录位置信息
    if(x == m && y == p) {// m和p是出口坐标
        return true;
    }
    // 循环遍历(x, y)的4个方向
    for(i = 0; i < 4; i++) {
        g = x + direction[i].a;
        h = y + direction[i].b;
        if(g < 0 || h < 0 || g > 11 || h > 11) {
            continue;
        }
        // 找下一位置寻找通向出口的路径
        if((gamedata[g][h] == 0||gamedata[g][h]==999||gamedata[g][h]==666) && mark[g][h] == 0) { // 如果这个位置是没有东西得且未被访问过
            mark[g][h] = 1; // 标记为已访问过
            if(seekPath(g, h, m, p)) { // 递归试探
                return true;
            }
        } else if(gamedata[g][h] > 0 && gamedata[g][h] < 47 && mark[g][h] == 0 && g == m && h == p) {// 回溯，换一个方向再试探通向出口的路径
            return true;
        }
    }
    return false; // 找不到通向出口的路径
}


//
//判断玩家能否到这里来，接受目标点的位置，返回是否能到那里去
//返回真假
//
bool Game_Logic::isAble_player_to(int num)
{
    int i = mary->nowx;
    int j = mary->nowy;
    //先找到玩家的位置
    int num1 = num / 100;
    int num2 = num - num1 * 100;
    //目标点的位置
    for(int p = 0 ; p < ROWS + 2 ; p ++){
        for(int q = 0 ;q < COLS + 2 ;q ++) {
            mark[p][q]=0;
        }
    }//先初始化一下mark，方便后续
    if(seekPath(i,j,num1,num2) == true) {
        return true;
    } else {
        return false;
    }
}


//
//flash道具：只能单人模式下使用
//dataFlash为点击的点，在无法用的时候要保持在9999
//
void Game_Logic::flashLogic()
{
    if(dataFlash == 9999) {
        canFlash = false;
        return;
    }
    int flash1 = dataFlash / 100;
    int flash2 = dataFlash - flash1 * 100;
    if(gamedata[flash1][flash2] == 0) {
        flashBeforetoNow[0] = mary->nowx * 100+mary->nowy;
        clearImage(mary->nowx,mary->nowy);
        mary->nowx = flash1;
        mary->nowy = flash2;//再将原有人物内的坐标改一下
        playerDire(*mary);//将角色所在的方块清零，角色不在的方块安放上角色
        flashBeforetoNow[1] = dataFlash;
        canFlash = true;
        dataFlash = 9999;
        return;
    } else if(gamedata[flash1][flash2] > 46 && gamedata[flash1][flash2] <= 50) {//如果点击的位置是道具且四周为空的话，那就停留在一旁，但是不激活道具
        int finalx,finaly;//放置的坐标
        //由于最边边上那一圈一定是空的，那么可以不用考虑边界问题
        for(int i = 0;i < 4;i++) {
            finalx = flash1+direction[i].a;
            finaly = flash2+direction[i].b;
            if(gamedata[finalx][finaly] == 0) {
                clearImage(mary->nowx,mary->nowy);
                flashBeforetoNow[0] = mary->nowx*100 + mary->nowy;
                mary->nowx = finalx;
                mary->nowy = finaly;//再将原有人物内的坐标改一下
                playerDire(*mary);//将角色所在的方块清零，角色不在的方块安放上角色
                flashBeforetoNow[1] = mary->nowx*100+mary->nowy;
                canFlash = true;
                dataFlash = 9999;
                if(box1->num_onBox != 0) {
                    flashReturn = 5;
                }
                return;
            }
        }
        dataFlash = 9999;
        canFlash = false;
        return;
    } else {//如果说点击的就是方块的话，那么是需要激活他们的
        int finalx,finaly;//放置的坐标
        //由于最边边上那一圈一定是空的，那么可以不用考虑边界问题
        for(int i = 0;i < 4;i++) {
            finalx = flash1+direction[i].a;
            finaly = flash2+direction[i].b;
            if(gamedata[finalx][finaly] == 0 || gamedata[finalx][finaly] == 999) {
                clearImage(mary->nowx,mary->nowy);
                flashBeforetoNow[0] = mary->nowx*100+mary->nowy;
                mary->nowx = finalx;
                mary->nowy = finaly;//再将原有人物内的坐标改一下
                playerDire(*mary);//将角色所在的方块清零，角色不在的方块安放上角色
                flashBeforetoNow[1] = finalx * 100 + finaly;
                //激活该方块
                if(box1->num_onBox == 0) {
                    boxOn(1,flash1,flash2);//将方块的坐标存入box的激活坐标当中
                    num_LastBox(1,box1->lastx_on*100+box1->lasty_on);//将该方块位置改变形式存入激活坐标中去(此时激活数量为0，存入lastbox)
                    flashReturn = 2;
                    dataFlash = 9999;
                    canFlash = true;
                    return;
                } else if(box1->num_onBox == 1) {
                    boxOn(1,flash1,flash2);//激活坐标更新，现在num_on应该为2
                    numBox(1,box1->lastx_on*100+box1->lasty_on);//将第二个方块的坐标更新存入numBox中去
                    if(boxClear(1,box1->numofbox_last,box1->numofbox) == 1) {
                        box1->num_onBox = 0;//让方块的激活数量变为0
                        scoreRenew(1,box1->numofbox);//增加分数
                        flashReturn = 3;//返回3
                        dataFlash = 9999;
                        canFlash = true;
                        return;
                    } else if(boxClear(1,box1->numofbox_last,box1->numofbox) == 2) {
                        box1->num_onBox = 0;//让激活的方块的数量变为0
                        flashReturn = 4;
                        canFlash = true;
                        dataFlash = 9999;
                        return;
                    } else if(boxClear(1,box1->numofbox_last,box1->numofbox) == 0) {
                        box1->num_onBox = 0;//让激活的方块的数量变为0
                        flashReturn = 5;
                        canFlash = true;
                        dataFlash = 9999;
                        return;
                    }
                    canFlash = false;
                    dataFlash = 9999;
                    return;
                }
            }
        }
        canFlash = false;
        dataFlash = 9999;
        return;
    }
}


//
//暗示道具
//返回真假
//
bool Game_Logic::hintLogic()
{
    int i = mary->nowx;
    int j = mary->nowy;
    int num_player = i * 100 + j;
    //先找到玩家的位置
    for(int m = 1;m < ROWS + 1;m++) {
        for(int n = 1;n < COLS + 1;n++) {
            int numBox1 = m * 100 + n;
            if((gamedata[m][n] > 0
                    && gamedata[m][n] < 47)
                    && (isAble_player_to(numBox1))) {
                //就存入hint——num1
                hintNum[0] = numBox1;
                //然后遍历其后面的方块里面有没有相同的并且能与player无转动或者一转动相连
                for(int k = 1;k < ROWS + 1;k++) {
                    for(int l = 1;l < COLS + 1;l++) {
                        int numBox2 = k * 100 + l;
                        if((isClearable_hint(numBox1,numBox2))
                                && (gamedata[k][l] > 0 && gamedata[k][l] < 47)
                                && (gamedata[m][n] % 9 + 1 == gamedata[k][l] % 9 + 1)
                                && (k != m && l != n)
                                && (isAble_player_to(numBox2))) {
                            hintNum[1] = numBox2;
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}


//
//hint专用的clearable，接受上一个点的代数last和这一个点的代数last，返回能不能消除
//类似下一个函数
//
bool Game_Logic::isClearable_hint(int last, int now)
{
    if(isAble_Nocorner_Clear_hint(last,now) == true) {
        return true;
    } else if(isAble_Onecorner_Clear_hint(last,now) == true) {
        return true;
    } else if(isAble_Twocorner_Clear_hint(last,now) == true) {
        return true;
    } else {
        return false;
    }
}


//
//判断是否处于可以消除的位置，接受上一个点的代数last和这一个点的代数last，返回能不能消除
//同时需要画几条线
//
bool Game_Logic::isClearable(int mode,int last,int now)
{
    if(isAble_Nocorner_Clear(mode,last,now) == true) {
        drawLineNum = 1;
        return true;
    } else if(isAble_Onecorner_Clear(mode,last,now) == true) {
        drawLineNum = 2;
        return true;
    } else if(isAble_Twocorner_Clear(mode,last,now) == true) {
        drawLineNum = 3;
        return true;
    } else {
        drawLineNum = 0;
        return false;
    }
}
