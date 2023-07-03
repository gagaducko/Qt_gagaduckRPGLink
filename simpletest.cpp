#include "simpletest.h"


QTEST_MAIN(SimpleTest)

SimpleTest::SimpleTest()
{
    game = new Game_Logic(1);
    setMap();
}


//
//先搞张空地图
//
void SimpleTest::setMap()
{
    for(int i = 0 ; i < ROWS + 2 ; i++){
        for(int j = 0 ; j < COLS + 2 ; j++){
            gamedata[i][j]=0;
        }
    }
    gamedata[0][0]=999;
}


//
//先测试no_corner的这个if
//先测试no_corner函数中相邻的这个if（包括竖着相邻和横着相邻两个，后续同理）
//如果能相连同时消除的话，那么就是对的
//
void SimpleTest::test_normal1()
{
    setMap();
    gamedata[1][1] = 3;
    gamedata[1][2] = gamedata[1][1];
    QVERIFY(game->isClearable(1,101,102) == true);   //可以相连
    QVERIFY(game->boxClear(1,101,102) == 1);         //可以消除
}

void SimpleTest::test_wrong1()
{
    setMap();
    gamedata[1][2] = 5;
    gamedata[2][1] = 5;
    gamedata[1][1] = 3;
    gamedata[2][2] = 6;                              //封死四周的路
    gamedata[1][3] = 6;
    gamedata[3][1] = 6;
    QVERIFY(game->isClearable(1,101,102) == true);   //可以相连
    QVERIFY(game->boxClear(1,101,102) == 1);         //不能消除——应该返回false
    QVERIFY(game->isClearable(1,101,201) == true);   //可以相连
    QVERIFY(game->boxClear(1,101,201) == 1);         //不能消除——应该返回false
}


//
//先测试no_corner的这个if
//再测试no_corner函数中不相邻的这个if
//如果能相连同时消除的话，那么就是对的
//
void SimpleTest::test_normal2()
{
    setMap();
    gamedata[1][1] = 1;
    gamedata[8][1] = 1;
    gamedata[1][8] = 1;

    //把除了连线外的路封死
    for(int i = 2 ; i < 12 ; i++) {
        gamedata[i][2]=6;
        gamedata[2][i]=6;
    }

    QVERIFY(game->isClearable(1,101,801) == true);
    QVERIFY(game->boxClear(1,101,801) == 1);
}

void SimpleTest::test_wrong2()
{
    setMap();
    gamedata[1][1] = 2;
    gamedata[8][1] = 1;
    gamedata[1][8] = 1;

    //把除了连线外的路封死
    for(int i = 2 ; i < 12 ; i++) {
        gamedata[i][2]=6;
        gamedata[2][i]=6;
    }
    QVERIFY(game->isClearable(1,101,801) == true);
    QVERIFY(game->boxClear(1,101,801) == 1);         //可以消除
    QVERIFY(game->isClearable(1,101,108) == true);
    QVERIFY(game->boxClear(1,101,108) == 1);         //可以消除
}


//
//测试one_corner的这个if
//如果能相连同时消除的话，那么就是对的
//
void SimpleTest::test_normal3()
{
    setMap();
    gamedata[1][1]=3;
    gamedata[2][2]=3;
    QVERIFY(game->isClearable(1,101,202) == true);
    QVERIFY(game->boxClear(1,101,202) == 1);         //可以消除
}

void SimpleTest::test_wrong3()
{
    setMap();
    gamedata[1][1]=3;
    gamedata[2][2]=3;
    gamedata[1][2]=6;
    gamedata[2][1]=6; //在normal3的基础上封住道路
    QVERIFY(game->isClearable(1,101,202) == true);
    QVERIFY(game->boxClear(1,101,202) == 1);         //可以消除
}


//
//测试two_corner的这个if
//如果能相连同时消除的话，那么就是对的
//
void SimpleTest::test_normal4()
{
    setMap();
    gamedata[1][1]=3;
    gamedata[10][10]=3;
    gamedata[5][5]=6; //封住对角线
    gamedata[1][10]=6;
    gamedata[10][1]=6;//封住能够一次到达的的角
    QVERIFY(game->isClearable(1,101,1010) == true);
    QVERIFY(game->boxClear(1,101,1010) == 1);         //可以消除
}

void SimpleTest::test_wrong4()
{
    setMap();
    gamedata[1][1]=3;
    gamedata[10][10]=5;
    gamedata[5][5]=6; //封住对角线
    gamedata[1][10]=6;
    gamedata[10][1]=6;//封住能够一次到达的的角
    QVERIFY(game->isClearable(1,101,1010) == true);
    QVERIFY(game->boxClear(1,101,1010) == 1);         //可以消除
}


//
//测试two_corner的这个if
//如果能相连同时消除的话，那么就是对的
//
void SimpleTest::test_normal5()
{
    setMap();
    gamedata[1][1]=3;
    gamedata[1][10]=3;
    gamedata[1][5]=6; //封住对角线
    QVERIFY(game->isClearable(1,101,110) == true);
    QVERIFY(game->boxClear(1,101,110) == 1);         //可以消除
}

void SimpleTest::test_wrong5()
{
    setMap();
    gamedata[1][1]=5;
    gamedata[1][10]=3;
    gamedata[1][5]=6; //封住对角线
    QVERIFY(game->isClearable(1,101,110) == true);
    QVERIFY(game->boxClear(1,101,110) == 1);         //可以消除
}


//
//测试两次选中了同一个的情况下
//会不会返回0
//
void SimpleTest::test_normal6()
{
    setMap();
    gamedata[1][1]=4;
    QVERIFY(game->boxClear(1,101,101) == 0);         //一样的
}


//
//搞张地图测试多种情况下的能相连
//
void SimpleTest::test_normal7()
{
    setMap();
    for(int i = 1; i < 11 ; i++){
        for(int j = 1 ; j < 11 ; j++){
            gamedata[i][j] = (i + j) % 5 + 1;
        }
    }
    QVERIFY(game->isClearable(1,101,102) == true);
    QVERIFY(game->isClearable(1,101,103) == true);
    QVERIFY(game->isClearable(1,101,109) == true);
    QVERIFY(game->isClearable(1,201,801) == true);
}


void SimpleTest::test_normal8()
{
    setMap();
    for(int i = 1; i < 11 ; i++){
        for(int j = 1 ; j < 11 ; j++){
            gamedata[i][j] = (i + j) % 5 + 1;
        }
    }
    QVERIFY(game->isClearable(1,305,102) == false);
    QVERIFY(game->isClearable(1,501,703) == false);
    QVERIFY(game->isClearable(1,803,409) == false);
    QVERIFY(game->isClearable(1,101,1010) == false);
}
