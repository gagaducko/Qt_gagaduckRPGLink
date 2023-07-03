#ifndef SIMPLETEST_H
#define SIMPLETEST_H

#include <QtTest/QtTest>
#include "game_logic.h"

class SimpleTest : public QObject
{
    Q_OBJECT
private slots:
    //能否相连的多种情况
    //相连后正常消除
    //相连后无法正常消除（两次连的一个或者两次连的不同）
    void test_normal1();      //正常情况
    void test_normal2();      //正常情况
    void test_normal3();      //正常情况
    void test_normal4();      //正常情况
    void test_normal5();      //正常情况
    void test_normal6();      //正常情况
    void test_normal7();      //正常情况
    void test_normal8();      //正常情况
    void test_wrong1();       //错误情况
    void test_wrong2();       //错误情况
    void test_wrong3();       //错误情况
    void test_wrong4();       //错误情况
    void test_wrong5();       //错误情况
public:
    SimpleTest();
    void setMap();

    Game_Logic *game;
};

#endif // SIMPLETEST_H
