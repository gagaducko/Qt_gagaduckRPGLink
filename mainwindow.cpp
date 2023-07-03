#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //设置图标和标题与背景音乐
    setWindowTitle("Qlink连连看");
    setWindowIcon(QIcon(":/Images/icon.ico"));
    sound->play();
    sound->setLoops(100);
    is_show_timer=new QTimer(this);
    is_show_timer->start(500);
    connect(is_show_timer, SIGNAL(timeout()), this, SLOT(process()));  //将超时信号和自定义的槽函数连接
    //按键函数绑定
    connect(ui->action_exit,&QAction::triggered,this,&MainWindow::on_action_exit_click);
    connect(ui->action_about,&QAction::triggered,this,&MainWindow::on_action_about_click);
    connect(ui->action_playmode,&QAction::triggered,this,&MainWindow::on_action_mode_click);
    connect(ui->action_item,&QAction::triggered,this,&MainWindow::on_action_item_click);
    connect(ui->action_singlegame,&QAction::triggered,this,&MainWindow::on_action_single_click);
    connect(ui->action_doublegame,&QAction::triggered,this,&MainWindow::on_action_double_click);
    connect(ui->action_load,&QAction::triggered,this,&MainWindow::on_actionload_click);
}


//
//监控is_show
//看能不能回到主界面
//
void MainWindow::process()//用来看看能不能返回原先界面
{
    if(is_show == true) {
        is_show = false;
        this->show();
        sound->play();
    }
}


//
//单人游戏
//
//
void MainWindow::startGame_single()
{
    //进入单人游戏场景且自身隐藏
    sound->stop();
    singlegame = new Mode_Single(this);
    this->hide();
    singlegame->show();
}


//
//双人游戏
//
//
void MainWindow::startGame_double()
{
    //进入双人游戏场景且自身隐藏
    sound->stop();
    doublegame = new Mode_Double(this);
    this->hide();
    doublegame->show();
}


//
//载入游戏
//
//
void MainWindow::loadGame()
{
    QFileInfo fileInfo("saveload.txt");
    if(fileInfo.isFile()) {
        QFile file("saveload.txt");
        QVector<int> array;
        if(file.open(file.ReadOnly)) {
            QTextStream out(&file);
            while(!out.atEnd()) {
                static int buf;
                out>>buf;
                array.append(buf);
            }
            file.close();
        }
        int array_data[(ROWS+2)*(COLS+2)];
        int score1;
        int score2;
        int time;
        if(array[0] == 1) {
            for(int i = 0 ; i < (ROWS + 2) * (COLS + 2) ; i ++) {
                array_data[i] = array[i + 1];
            }
            score1 = array[(ROWS + 2) * (COLS + 2) + 1];
            time=array[(ROWS + 2) * (COLS + 2) + 2];
            qDebug()<<"the score is:"<<score1;
            qDebug()<<"the time is:"<<time;
            //进入单人游戏场景且自身隐藏
            sound->stop();
            singlegame = new Mode_Single(array_data,score1,time,this);
            this->hide();
            singlegame->show();
        }
        if(array[0] == 2) {
            for(int i = 0 ; i < (ROWS + 2) * (COLS + 2) ; i ++) {
                array_data[i] = array[i + 1];
            }
            score1 = array[(ROWS + 2) * (COLS + 2) + 1];
            score2 = array[(ROWS + 2) * (COLS + 2) + 2];
            time = array[(ROWS + 2) * (COLS + 2) + 3];
            qDebug()<<"the score is:"<<score1;
            qDebug()<<"the time is:"<<time;
            //进入双人游戏场景且自身隐藏
            sound->stop();
            doublegame = new Mode_Double(array_data,score1,score2,time,this);
            this->hide();
            doublegame->show();
        }
    } else {
        QMessageBox messageBox(QMessageBox::NoIcon,
                                   "抱歉", "并未找到存档信息！！！",
                                   QMessageBox::Yes, this);
            int result=messageBox.exec();
            switch (result){
            case QMessageBox::Yes:
                qDebug()<<"Yes";
                break;
            default:
                break;
            }
    }
}


//
//退出游戏
//
//
void MainWindow::exitGame()
{   
    QMessageBox messageBox(QMessageBox::NoIcon,
                               "退出", "您确定要退出游戏吗?",
                               QMessageBox::Yes | QMessageBox::No, this);
        int result=messageBox.exec();
        switch (result)
        {
        case QMessageBox::Yes:
            qDebug()<<"Yes";
            close();
            break;
        case QMessageBox::No:
            qDebug()<<"NO";
            break;
        default:
            break;
        }
}


//
//析构
//
//
MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_singleButton_clicked(bool checked)
{
    QSound::play(":/Sounds/clicks.wav");
    QTimer::singleShot(300,this,[=](){
        startGame_single();
    });
}


void MainWindow::on_doubleButton_clicked(bool checked)
{
    QSound::play(":/Sounds/clicks.wav");
    startGame_double();
}


void MainWindow::on_loadButton_clicked(bool checked)
{
    QSound::play(":/Sounds/clicks.wav");
    loadGame();
}


void MainWindow::on_exitButton_clicked(bool checked)
{
    QSound::play(":/Sounds/clicks.wav");
    exitGame();
}


void MainWindow::on_action_exit_click(bool checked)
{
    exitGame();
}


void MainWindow::on_action_about_click(bool checked)
{
    QMessageBox messageBox(QMessageBox::NoIcon,
                               "关于", "2021, made by gagaduck, a course assignment from my freshman year in university. ",
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


void MainWindow::on_action_mode_click(bool checked)
{
    QMessageBox messageBox(QMessageBox::NoIcon,
                               "游戏介绍", "本程序分为两种模式：\n    1)单人模式\n    2)双人模式",
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


void MainWindow::on_action_item_click(bool checked)
{
    QMessageBox messageBox(QMessageBox::NoIcon,
                               "道具介绍", "本程序共包括四种道具：\n    1）+1s道具:将能延长剩余时间30s\n    2)Shuffle道具:所有方块位置将重排\n    3)Hint道具:10s内会高亮一对可能链接的方块，被消除后会继续亮下一对，直到10s时间结束\n    4)Flash道具：5s内允许通过鼠标单击移动人物位置，人物移动到的位置必须通过空地可到达，否则点击不产生任何效果。如果点击到方块，且人物可以移动到该方块旁，则人物移动到该方块旁，且该方块被激活。如果方块四周有多个位置可以让人物停留，则人物移动到其中任何一个位置均可",
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


void MainWindow::on_action_single_click(bool checked)
{
    startGame_single();
}


void MainWindow::on_action_double_click(bool checked)
{
    startGame_double();
}


void MainWindow::on_actionload_click(bool checked)
{
    loadGame();
}
