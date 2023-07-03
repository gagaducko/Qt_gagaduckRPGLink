#include "Mode_Double.h"
#include "ui_Mode_Double.h"


//
//初始化
//
//
Mode_Double::Mode_Double(QWidget *parent) :
    Mode_Single(parent),
    ui(new Ui::Mode_Double)
{
    ui->setupUi(this);
    ui->centralwidget->installEventFilter(this);
    game = new Game_Logic(2);
    //设置图标和标题与背景音乐
    setWindowTitle("Qlink连连看");
    setWindowIcon(QIcon(":/Images/icon.ico"));
    setFixedSize(780,1000);
    sound->play();
    sound->setLoops(100);
    this->grabKeyboard();
    //绘制初始地图
    drawGame();
    //设定玩家1的游戏分数
    label_2 = new QLabel(this);
    label_2->move(50,830);
    label_2->resize(100,100);
    QPixmap pixmap(":/Images/score.png");
    label_2->setPixmap(pixmap);
    label_2->setFixedSize(100,100);
    label_3 = new QLabel(this);
    label_3->move(170,830);
    label_3->resize(100,100);
    label_3->setNum(game->mary->score);
    label_3->setStyleSheet("QLabel{font: bold;  font-size:70px;}");
    //设定计时器
    pProgressBar = new QProgressBar(this);
    pProgressBar->setOrientation(Qt::Horizontal);                // 水平方向
    pProgressBar->move(290,850);                                  //位置
    pProgressBar->resize(230,45);                                //设置控件大小
    myTimer = new QTimer(this);
    myTimer->start(1000);					                     //设置每个时间为1000ms，启动计时器
    connect(myTimer, SIGNAL(timeout()), this, SLOT(process()));  //将超时信号和自定义的槽函数连接
    prog_num = 300;                                              //一共三百秒的游戏时间
    label_1 = new QLabel(this);
    label_1->move(525,850);
    label_1->resize(50,45);
    label_1->setNum(prog_num);
    //设定游戏分数
    label_4 = new QLabel(this);
    label_4->move(580,830);
    label_4->resize(100,100);
    label_4->setPixmap(pixmap);
    label_4->setFixedSize(100,100);
    label_5 = new QLabel(this);
    label_5->move(680,830);
    label_5->resize(100,100);
    label_5->setNum(game->tom->score);
    label_5->setStyleSheet("QLabel{font: bold;  font-size:70px;}");
    //绑定按键和函数
    connect(ui->actionexit,&QAction::triggered,this,&Mode_Double::on_actionexit_clicked);
    connect(ui->actionsave,&QAction::triggered,this,&Mode_Double::on_actionsave_clicked);
    connect(ui->actionabout,&QAction::triggered,this,&Mode_Double::on_actionabout_clicked);
    connect(ui->actionout,&QAction::triggered,this,&Mode_Double::on_actionout_clicked);
    connect(ui->actionstop,&QAction::triggered,this,&Mode_Double::on_actionstop_clicked);
}


//
//初始化，array是gamedata\score1与score2是分数，time是剩下的时间
//
//
Mode_Double::Mode_Double(int array[],int score1,int score2,int time,QWidget *parent):
    Mode_Single(parent),
    ui(new Ui::Mode_Double)
{
    ui->setupUi(this);
    ui->centralwidget->installEventFilter(this);
    game = new Game_Logic(array,score1,score2);
    //设置图标和标题与背景音乐
    setWindowTitle("Qlink连连看");
    setWindowIcon(QIcon(":/Images/icon.ico"));
    setFixedSize(780,1000);
    sound->play();
    sound->setLoops(100);
    this->grabKeyboard();
    //绘制初始地图
    drawGame();
    //设定玩家1的游戏分数
    label_2 = new QLabel(this);
    label_2->move(50,830);
    label_2->resize(100,100);
    QPixmap pixmap(":/Images/score.png");
    label_2->setPixmap(pixmap);
    label_2->setFixedSize(100,100);
    label_3 = new QLabel(this);
    label_3->move(170,830);
    label_3->resize(100,100);
    label_3->setNum(game->mary->score);
    label_3->setStyleSheet("QLabel{font: bold;  font-size:70px;}");
    //设定计时器
    pProgressBar = new QProgressBar(this);
    pProgressBar->setOrientation(Qt::Horizontal);                // 水平方向
    pProgressBar->move(290,850);                                  //位置
    pProgressBar->resize(230,45);                                //设置控件大小
    myTimer = new QTimer(this);
    myTimer->start(1000);					                     //设置每个时间为1000ms，启动计时器
    connect(myTimer, SIGNAL(timeout()), this, SLOT(process()));  //将超时信号和自定义的槽函数连接
    prog_num = time;                                              //一共三百秒的游戏时间
    label_1 = new QLabel(this);
    label_1->move(525,850);
    label_1->resize(50,45);
    label_1->setNum(prog_num);
    //设定游戏分数
    label_4 = new QLabel(this);
    label_4->move(580,830);
    label_4->resize(100,100);
    label_4->setPixmap(pixmap);
    label_4->setFixedSize(100,100);
    label_5 = new QLabel(this);
    label_5->move(680,830);
    label_5->resize(100,100);
    label_5->setNum(game->tom->score);
    label_5->setStyleSheet("QLabel{font: bold;  font-size:70px;}");
    //绑定按键和函数
    connect(ui->actionexit,&QAction::triggered,this,&Mode_Double::on_actionexit_clicked);
    connect(ui->actionsave,&QAction::triggered,this,&Mode_Double::on_actionsave_clicked);
    connect(ui->actionabout,&QAction::triggered,this,&Mode_Double::on_actionabout_clicked);
    connect(ui->actionout,&QAction::triggered,this,&Mode_Double::on_actionout_clicked);
    connect(ui->actionstop,&QAction::triggered,this,&Mode_Double::on_actionstop_clicked);
}


//
//关于选项
//
//
void Mode_Double::on_actionabout_clicked(bool checked)
{
    QMessageBox messageBox(QMessageBox::NoIcon,
                               "多人游戏", "A方通过WASD四键进行操作！\nB方通过↑↓←→四键进行操作！",
                               QMessageBox::Yes, this);
        int result=messageBox.exec();

        switch (result)
        {
        case QMessageBox::Yes:
            qDebug()<<"Yes";
            break;
        default:
            break;
        }
}


//
//计时器
//
//
void Mode_Double::process()
{
    if(can_onprocess == true) {
        prog_num --;                             //进度条数值自减
        if(prog_num == 0) {
            myTimer->stop();
            can_onprocess = false;
            show_Gameover_time();
            qDebug()<<"game is over,time is out!";
        }
        if(game->isAble_continue() == false) {
            can_onprocess = false;
            myTimer->stop();
            int flag = 0;
            for(int i = 0; i < ROWS + 2 ; i ++) {
                for(int j = 0; j < COLS + 2 ; j ++) {
                    if(gamedata[i][j] > 0 && gamedata[i][j] < 47) {
                        flag = 1;
                        return;
                    }
                }
                if(flag == 1) {
                    myTimer->stop();
                    show_Gameover_Nobox_toclear();
                } else if(flag == 0) {
                    myTimer->stop();
                    show_Gameover_allclear();
                } else {
                    return;
                }
            }
        }
        int value = prog_num/3;
        if(value > 100)
        {
            value = 100;
        }
        pProgressBar->setValue(value);        //设置进度条数值
        label_1->setNum(prog_num);            //设置剩余时间
        label_3->setNum(game->mary->score);   //设置现在的score
        label_5->setNum(game->tom->score);    //同上
    } else {
        qDebug()<<"it is down!!!!!!!!!!";
        return;
    }
}


//
//saveLoad
//
//
void Mode_Double::saveLoad()
{
    QFile file("saveLoad.txt");
    qDebug()<<"it have get the things";
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << file.errorString();
    }
    int a = 2;
    QTextStream in(&file);
    in << a << " ";
        for(int i = 0 ; i < ROWS + 2 ; i ++) {
            for(int j = 0 ; j < COLS + 2 ; j ++) {
                in << gamedata[i][j] << " ";
            }
        }//写入现在的gamedate
        in << game->mary->score << " ";//写入游戏分数
        in << game->tom->score << " ";
        in << prog_num << " ";//写入游戏剩余的时间
        file.close();
        QMessageBox messageBox(QMessageBox::NoIcon,
                                   "保存", "保存成功！！！",
                                   QMessageBox::Yes, this);
            int result=messageBox.exec();
            switch (result) {
            case QMessageBox::Yes:
                break;
            default:
                break;
            }
}


//
//将线回归本源
//
//
void Mode_Double::drawLines_down()
{
    gameBox_draw_show();
}


//
//eventfilter过滤事件
//
//
bool Mode_Double::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == this->ui->centralwidget && event->type() == QEvent::Paint)
    {
        drawLines();
    }
    return QWidget::eventFilter(watched,event);
}


//
//看看需要隐藏哪些图片
//
//
void Mode_Double::gameBox_draw_hide(int num1,int num2,int num3,int num4)
{
    if(num1 == num3) {
        if(num2 - num4 == 1 || num4 - num2 == 1) {
            return;
        } else {
            if(num2 > num4) {
                for(int i = num4 + 1 ; i < num2 ; i ++) {
                    if(gamedata[num1][i] != 999 && gamedata[num1][i] != 666)
                        buttons[num1][i]->setVisible(false);
                }
                return;
            } else {
                for(int i = num2 + 1 ; i < num4 ; i ++) {
                    if(gamedata[num1][i] != 999 && gamedata[num1][i] != 666)
                        buttons[num1][i]->setVisible(false);
                }
                return;
            }
        }
    }
    if(num2 == num4) {
        if(num1 - num3 == 1 || num3 - num1 == 1) {
            return;
        } else {
            if(num1 > num3) {
                for(int i = num3 + 1 ; i < num1 ; i ++) {
                    if(gamedata[i][num2] != 999 && gamedata[i][num2] != 666)
                        buttons[i][num2]->setVisible(false);
                }
                return;
            } else {
                for(int i = num1 + 1 ; i < num3 ; i ++) {
                    if(gamedata[i][num2] != 999 && gamedata[i][num2] != 666)
                        buttons[i][num2]->setVisible(false);
                }
                return;
            }
        }
    }
}


//
//看看需要显示哪些图片
//
//
void Mode_Double::gameBox_draw_show()
{
    for(int i = 0 ; i < ROWS + 2 ; i ++) {
        for(int j = 0 ; j < COLS + 2 ; j ++) {
            buttons[i][j]->setVisible(true);
        }
    }
}


//
//画出所有的线
//
//
void Mode_Double::drawLines()
{
    QPainter p(this->ui->centralwidget);
    QPen pen;
    QColor color(255,215,0);
    pen.setColor(color);
    pen.setWidth(9);
    p.setPen(pen);

    switch (game->drawLineNum) {
        case 0:
        {
            break;
        }
        case 1:
        {
            int num1,num2,num3,num4;
            if(draw_line_man == 1) {
                num1 = game->box1->Line_Nocorner[0][0] / 100;
                num2 = game->box1->Line_Nocorner[0][0] - num1 * 100;
                num3 = game->box1->Line_Nocorner[0][1] / 100;
                num4 = game->box1->Line_Nocorner[0][1] - 100 * num3;
            } else {
                num1 = game->box2->Line_Nocorner[0][0] / 100;
                num2 = game->box2->Line_Nocorner[0][0] - num1 * 100;
                num3 = game->box2->Line_Nocorner[0][1] / 100;
                num4 = game->box2->Line_Nocorner[0][1] - 100 * num3;
            }
            p.drawLine(num1 * 65 + 32.5,num2 * 65 + 32.5,num3 * 65 + 32.5,num4 * 65 + 32.5);
            gameBox_draw_hide(num1,num2,num3,num4);
            break;
        }
        case 2:
        {
            int num1,num2,num3,num4,num5,num6,num7,num8;
            if(draw_line_man == 1) {
                num1 = game->box1->Line_Onecorner[0][0] / 100;
                num2 = game->box1->Line_Onecorner[0][0] - num1 * 100;
                num3 = game->box1->Line_Onecorner[0][1] / 100;
                num4 = game->box1->Line_Onecorner[0][1] - 100 * num3;
                num5 = game->box1->Line_Onecorner[1][0] / 100;
                num6 = game->box1->Line_Onecorner[1][0] - num5 * 100;
                num7 = game->box1->Line_Onecorner[1][1] / 100;
                num8 = game->box1->Line_Onecorner[1][1] - 100 * num7;
            } else {
                num1 = game->box2->Line_Onecorner[0][0] / 100;
                num2 = game->box2->Line_Onecorner[0][0] - num1 * 100;
                num3 = game->box2->Line_Onecorner[0][1] / 100;
                num4 = game->box2->Line_Onecorner[0][1] - 100 * num3;
                num5 = game->box2->Line_Onecorner[1][0] / 100;
                num6 = game->box2->Line_Onecorner[1][0] - num5 * 100;
                num7 = game->box2->Line_Onecorner[1][1] / 100;
                num8 = game->box2->Line_Onecorner[1][1] - 100 * num7;
            }
            p.drawLine(num1 * 65 + 32.5,num2 * 65 + 32.5,num3 * 65 + 32.5,num4 * 65 + 32.5);
            p.drawLine(num5 * 65 + 32.5,num6 * 65 + 32.5,num7 * 65 + 32.5,num8 * 65 + 32.5);
            gameBox_draw_hide(num1,num2,num3,num4);
            gameBox_draw_hide(num5,num6,num7,num8);
            if(gamedata[num3][num4] != 999 && gamedata[num3][num4] != 666)
                buttons[num3][num4]->hide();
            break;
        }
        case 3:
        {
            int num1,num2,num3,num4,num5,num6,num7,num8,num9,num10,num11,num12;
            if(draw_line_man == 1) {
                num1 = game->box1->Line_Twocorner[0][0] / 100;
                num2 = game->box1->Line_Twocorner[0][0] - num1 * 100;
                num3 = game->box1->Line_Twocorner[0][1] / 100;
                num4 = game->box1->Line_Twocorner[0][1] - 100 * num3;
                num5 = game->box1->Line_Twocorner[1][0] / 100;
                num6 = game->box1->Line_Twocorner[1][0] - num5 * 100;
                num7 = game->box1->Line_Twocorner[1][1] / 100;
                num8 = game->box1->Line_Twocorner[1][1] - 100 * num7;
                num9 = game->box1->Line_Twocorner[2][0] / 100;
                num10 = game->box1->Line_Twocorner[2][0] - num9 * 100;
                num11 = game->box1->Line_Twocorner[2][1] / 100;
                num12 = game->box1->Line_Twocorner[2][1] - 100 * num11;
            } else {
                num1 = game->box2->Line_Twocorner[0][0] / 100;
                num2 = game->box2->Line_Twocorner[0][0] - num1 * 100;
                num3 = game->box2->Line_Twocorner[0][1] / 100;
                num4 = game->box2->Line_Twocorner[0][1] - 100 * num3;
                num5 = game->box2->Line_Twocorner[1][0] / 100;
                num6 = game->box2->Line_Twocorner[1][0] - num5 * 100;
                num7 = game->box2->Line_Twocorner[1][1] / 100;
                num8 = game->box2->Line_Twocorner[1][1] - 100 * num7;
                num9 = game->box2->Line_Twocorner[2][0] / 100;
                num10 = game->box2->Line_Twocorner[2][0] - num9 * 100;
                num11 = game->box2->Line_Twocorner[2][1] / 100;
                num12 = game->box2->Line_Twocorner[2][1] - 100 * num11;
            }
            p.drawLine(num1 * 65 + 32.5,num2 * 65 + 32.5,num3 * 65 + 32.5,num4 * 65 + 32.5);
            p.drawLine(num5 * 65 + 32.5,num6 * 65 + 32.5,num7 * 65 + 32.5,num8 * 65 + 32.5);
            p.drawLine(num9 * 65 + 32.5,num10 * 65 + 32.5,num11 * 65+ 32.5,num12 * 65 + 32.5);
            gameBox_draw_hide(num1,num2,num3,num4);
            gameBox_draw_hide(num5,num6,num7,num8);
            gameBox_draw_hide(num9,num10,num11,num12);
            if(gamedata[num3][num4]!=999 && gamedata[num3][num4] != 666)
                buttons[num3][num4]->hide();
            if(gamedata[num7][num8]!=999 && gamedata[num7][num8] != 666)
                buttons[num7][num8]->hide();
            break;
        }
    }
}


//
//析构
//
//
Mode_Double::~Mode_Double()
{
    delete ui;
}
