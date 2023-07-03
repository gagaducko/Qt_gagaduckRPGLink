#ifndef Mode_Single_H
#define Mode_Single_H

#include <QMainWindow>
#include <QSound>
#include <QTimer>
#include <QtDebug>
#include <QPushButton>
#include <QMessageBox>
#include <QLabel>
#include <cstring>
#include "game_logic.h"


namespace Ui {
class Mode_Single;
}

class Mode_Single : public QMainWindow
{
    Q_OBJECT

public:
    Mode_Single(QWidget *parent = nullptr);
    Mode_Single(int array[],int score,int time,QWidget *parent = nullptr);
    ~Mode_Single();

    //方块激活及消除相关的函数
    void gameBox_on(int mode,int num1,int num2);                            //激活
    void gameBox_hint_on(int num1,int num2);                                //激活
    void gameBox_dowm(int num1,int num2);                                   //变为空
    void gameBox_back(int num1,int num2);                                   //回到未激活状态
    void gameBox_how_to_do(int numofplayer,int num);                        //看看下一步要搞啥
    void gameBox_how_to_do_Player1(int num);                                //看看下一步要搞啥
    void gameBox_how_to_do_Player2(int num);                                //看看下一步要搞啥
    void gameBox_draw_hide(int num1,int num2,int num3,int num4);            //看看需要隐藏哪些图片
    void gameBox_draw_show();

    //道具相关函数
    void refreshGame();                                                     //重新更新画面
    void refreshButton();                                                   //重排道具
    void plusThirtys();                                                     //+1s道具
    void useFlash();                                                        //flash道具
    void useHint();                                                         //hint道具
    void useFreeze();                                                       //冻结3s
    void useDizzy();                                                        //混乱十秒

    //游戏结束相关函数
    void show_Gameover_time();                                              //游戏结束弹窗
    void show_Gameover_Nobox_toclear();                                     //没有方块可以消除了
    void show_Gameover_allclear();                                          //全都消除了游戏结束
    void changeShow();

    //绘画相关的函数
    void drawGame();
    void drawLines();                                                       //画出所有的线
    void drawLines_down();                                                  //把画了的线全部撤销回去
    void drawFlash_after();                                                 //flash的变化

    //保存相关函数
    void saveLoad();

    //keypress以及EVENTFILTER
    bool eventFilter(QObject *watched, QEvent *event);
    void keyPressEvent(QKeyEvent *ev);
    void pressEvent_1(QKeyEvent *ev);
    void pressEvent_2(QKeyEvent *ev);
    void pressEvent_3(QKeyEvent *ev);
    void pressEvent_4(QKeyEvent *ev);

    QSound *sound = new QSound(":/Sounds/BGM_game.wav", this);

public slots:

    void on_actionabout_clicked(bool checked);
    void on_actionexit_clicked(bool checked);
    void on_actionsave_clicked(bool checked);
    void on_actionout_clicked(bool checked);
    void on_actionstop_clicked(bool checked);
    void box_Clicked();
    void process();                                                         //计时器
    void hint_process();                                                    //hint的计时器
    void flash_process();                                                   //flash的计时器
    void freeze_process();                                                  //freeze的计时器
    void dizzy_process();                                                   //dizzy的计时器

public:
    Game_Logic *game;
    Ui::Mode_Single *ui;
    QPushButton *buttons[ROWS + 2][COLS + 2];
    int prog_num;                               //进度条显示的数值
    int hint_num = 20;                          //用于hint刷新的数值
    bool changed;                               //用于flash的判断
    bool is_stoped = false;                     //用于是否暂停
    bool isHint_on = false;                     //hint道具是否是开启的
    bool can_go = true;                         //能否运动
    bool can_go2 = true;
    bool dizzy_1 = false;
    bool dizzy_2 = false;
    bool can_onprocess = true;
    int messageBox_go = 0;
    int draw_line_man;                          //需要画哪一条线的line
    QTimer *myTimer;                            //计时器对象
    QTimer *hintTimer;                          //hint计时器对象
    QTimer *freezeTimer;                        //freeze计时器对象
    QTimer *dizzyTimer;                         //dizzy计时器对象
    QProgressBar *pProgressBar;                 //进度条
    QLabel *label_1;                            //label1:显示倒计时
    QLabel *label_2;                            //label2:显示玩家图片
    QLabel *label_3;                            //label3:显示玩家的分数


private:
    bool isFlash_on = false;                    //flash道具是否是开启的
    int flash_num = 50;                         //用于flash刷新的数值
    QTimer *flashTimer;                         //flash计时器对象
};

#endif // Mode_Single_H
