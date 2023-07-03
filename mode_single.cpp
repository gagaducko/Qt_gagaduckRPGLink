#include "Mode_Single.h"
#include "ui_Mode_Single.h"


char* str1=":/Images/00";
char* str2=".png";
char* str3="_on.png";
char* str4="_hint.png";

//
//设定button上的url：string
//将两个串串在一起
//
void copy2(char* to, const char* from)
{
    char* cp = to;
    //找到要拼接的目的字符串的'\0'位置
    while (*cp) {
        cp++;
    }
    while (*cp++ = *from++);
}


//
//初始化mode_single
//
//
Mode_Single::Mode_Single(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Mode_Single)
{
    ui->setupUi(this);
    ui->centralwidget->installEventFilter(this);
    game = new Game_Logic(1);
    //设置图标和标题与背景音乐
    setWindowTitle("Qlink连连看");
    setWindowIcon(QIcon(":/Images/icon.ico"));
    setFixedSize(780,1000);
    sound->play();
    sound->setLoops(100);
    this->grabKeyboard();
    //绘制初始地图
    drawGame();
    //设定计时器
    pProgressBar = new QProgressBar(this);
    pProgressBar->setOrientation(Qt::Horizontal);                // 水平方向
    pProgressBar->move(65,850);                                  //位置
    pProgressBar->resize(350,45);                                //设置控件大小
    myTimer = new QTimer(this);
    myTimer->start(1000);					                     //设置每个时间为1000ms，启动计时器
    connect(myTimer, SIGNAL(timeout()), this, SLOT(process()));  //将超时信号和自定义的槽函数连接
    prog_num = 300;                                              //一共三百秒的游戏时间
    label_1 = new QLabel(this);
    label_1->move(420,850);
    label_1->resize(50,45);
    label_1->setNum(prog_num);
    //设定游戏分数
    label_2 = new QLabel(this);
    label_2->move(500,830);
    label_2->resize(100,100);
    QPixmap pixmap(":/Images/score.png");
    label_2->setPixmap(pixmap);
    label_2->setFixedSize(100,100);
    label_3 = new QLabel(this);
    label_3->move(620,830);
    label_3->resize(100,100);
    label_3->setNum(game->mary->score);
    label_3->setStyleSheet("QLabel{font: bold;  font-size:70px;}");
    //绑定按键和函数
    connect(ui->actionexit,&QAction::triggered,this,&Mode_Single::on_actionexit_clicked);
    connect(ui->actionsave,&QAction::triggered,this,&Mode_Single::on_actionsave_clicked);
    connect(ui->actionabout,&QAction::triggered,this,&Mode_Single::on_actionabout_clicked);
    connect(ui->actionout,&QAction::triggered,this,&Mode_Single::on_actionout_clicked);
    connect(ui->actionstop,&QAction::triggered,this,&Mode_Single::on_actionstop_clicked);
}


//
//初始化
//接受array、score、time,以便由此初始化game
//
Mode_Single::Mode_Single(int array[],int score,int time,QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::Mode_Single)
{
    ui->setupUi(this);
    ui->centralwidget->installEventFilter(this);
    game = new Game_Logic(array , score);
    //设置图标和标题与背景音乐
    setWindowTitle("Qlink连连看");
    setWindowIcon(QIcon(":/Images/icon.ico"));
    setFixedSize(780,1000);
    sound->play();
    sound->setLoops(100);
    this->grabKeyboard();
    //绘制初始地图
    drawGame();
    //设定计时器
    pProgressBar = new QProgressBar(this);
    pProgressBar->setOrientation(Qt::Horizontal);                // 水平方向
    pProgressBar->move(65,850);                                  //位置
    pProgressBar->resize(350,45);                                //设置控件大小
    myTimer = new QTimer(this);
    myTimer->start(1000);					                     //设置每个时间为1000ms，启动计时器
    connect(myTimer, SIGNAL(timeout()), this, SLOT(process()));  //将超时信号和自定义的槽函数连接
    prog_num=time;
    label_1 = new QLabel(this);
    label_1->move(420,850);
    label_1->resize(50,45);
    label_1->setNum(prog_num);
    //设定游戏分数
    label_2 = new QLabel(this);
    label_2->move(500,830);
    label_2->resize(100,100);
    QPixmap pixmap(":/Images/score.png");
    label_2->setPixmap(pixmap);
    label_2->setFixedSize(100,100);
    label_3 = new QLabel(this);
    label_3->move(620,830);
    label_3->resize(100,100);
    label_3->setNum(game->mary->score);
    label_3->setStyleSheet("QLabel{font: bold;  font-size:70px;}");
    //绑定按键和函数
    connect(ui->actionexit,&QAction::triggered,this,&Mode_Single::on_actionexit_clicked);
    connect(ui->actionsave,&QAction::triggered,this,&Mode_Single::on_actionsave_clicked);
    connect(ui->actionabout,&QAction::triggered,this,&Mode_Single::on_actionabout_clicked);
    connect(ui->actionout,&QAction::triggered,this,&Mode_Single::on_actionout_clicked);
    connect(ui->actionstop,&QAction::triggered,this,&Mode_Single::on_actionstop_clicked);
}


//
//changeShow
//改变is_show，好让主窗口重新显示
//
void Mode_Single::changeShow()
{
    is_show = true;
}


//
//暂停/开始的按键
//
//
void Mode_Single::on_actionstop_clicked(bool checked)
{
    qDebug()<<"stop succeed";
    if(is_stoped == false) {
        is_stoped = true;
        qDebug()<<"stop kaishi zanting";
        myTimer->stop();
        can_go = false;
    } else {
        is_stoped = false;
        qDebug()<<"stop kai sshi qi dong !";
        myTimer->start();
        can_go = true;
    }
}


//
//timer的process
//
//
void Mode_Single::process()
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
        int value = prog_num / 3;
        if(value > 100)
        {
            value = 100;
        }
        pProgressBar->setValue(value);        //设置进度条数值
        label_1->setNum(prog_num);            //设置剩余时间
        label_3->setNum(game->mary->score);   //设置现在的score
    } else {
        qDebug()<<"it is down!!!!!!!!!!";
        return;
    }
}


//
//没有方块可以消除了
//点yes能退回到主界面
//
void Mode_Single::show_Gameover_Nobox_toclear()
{
    can_onprocess=false;
    if(messageBox_go == 0){
        QMessageBox messageBox(QMessageBox::NoIcon,
                                   "抱歉", "场上没有你能够消除的方块啦！",
                                   QMessageBox::Yes, this);
            int result=messageBox.exec();
            messageBox_go=1;
            switch (result) {
            case QMessageBox::Yes:
                changeShow();
                sound->stop();
                myTimer->stop();
                this->hide();
                qDebug()<<"Yes";
                break;
            default:
                break;
            }
    }
}


//
//全都消除了游戏结束
//点yes能退回到主界面
//
void Mode_Single::show_Gameover_allclear()
{
    can_onprocess=false;
    if(messageBox_go == 0){
        QMessageBox messageBox(QMessageBox::NoIcon,
                                   "恭喜！", "恭喜你消除了所有的方块！",
                                   QMessageBox::Yes, this);
            int result=messageBox.exec();
            messageBox_go=1;
            switch (result) {
            case QMessageBox::Yes:
                changeShow();
                sound->stop();
                myTimer->stop();
                this->hide();
                qDebug()<<"Yes";
                break;
            default:
                break;
            }
    }
}


//
//游戏时间到，游戏结束
//点yes能退回到主界面
//
void Mode_Single::show_Gameover_time()
{
    can_onprocess=false;
    if(messageBox_go == 0){
        QMessageBox messageBox(QMessageBox::NoIcon,
                                   "抱歉", "游戏时间到啦！",
                                   QMessageBox::Yes, this);
            int result=messageBox.exec();
            messageBox_go=1;
            switch (result) {
            case QMessageBox::Yes:
                changeShow();
                sound->stop();
                myTimer->stop();
                this->hide();
                break;
            default:
                break;
            }
    }
}


//
//画出游戏形态
//
//
void Mode_Single::drawGame()
{
    //绘制连连看
    for(int i = 0 ; i < ROWS + 2 ; ++ i) {
        for(int j = 0 ; j < COLS + 2 ; ++ j) {
            buttons[i][j] = new QPushButton(this->centralWidget());
            buttons[i][j]->installEventFilter(this);
            buttons[i][j]->resize(65,65); //设置控件的大小
            buttons[i][j]->move(i*65,j*65);
            buttons[i][j]->setIconSize(QSize(65,65));
            buttons[i][j]->setCursor(Qt::PointingHandCursor);
            switch (gamedata[i][j]) {
            case 999:
                buttons[i][j]->setIcon(QIcon(QString(":/Images/icon.png")));
                connect(buttons[i][j],&QPushButton::clicked,this,&Mode_Single::box_Clicked);
                continue;
            case 666:
                buttons[i][j]->setIcon(QIcon(QString(":/Images/player2.png")));
                connect(buttons[i][j],&QPushButton::clicked,this,&Mode_Single::box_Clicked);
                continue;
            case 47:
                buttons[i][j]->setIcon(QIcon(QString(":/Images/101.png")));
                connect(buttons[i][j],&QPushButton::clicked,this,&Mode_Single::box_Clicked);
                continue;
            case 48:
                buttons[i][j]->setIcon(QIcon(QString(":/Images/102.png")));
                connect(buttons[i][j],&QPushButton::clicked,this,&Mode_Single::box_Clicked);
                continue;
            case 49:
                buttons[i][j]->setIcon(QIcon(QString(":/Images/103.png")));
                connect(buttons[i][j],&QPushButton::clicked,this,&Mode_Single::box_Clicked);
                continue;
            case 50:
                buttons[i][j]->setIcon(QIcon(QString(":/Images/104.png")));
                connect(buttons[i][j],&QPushButton::clicked,this,&Mode_Single::box_Clicked);
                continue;
                //有105、106的没有点击道具，因此不需要绑定clicked
            case 105:
                buttons[i][j]->setIcon(QIcon(QString(":/Images/105.png")));
                continue;
            case 106:
                buttons[i][j]->setIcon(QIcon(QString(":/Images/106.png")));
                continue;
            case 0:
                buttons[i][j]->setIcon(QIcon(QString(":/Images/BGP.png")));
                connect(buttons[i][j],&QPushButton::clicked,this,&Mode_Single::box_Clicked);
                continue;
            }
            if(gamedata[i][j] != 0) {
                int picnum = gamedata[i][j] % 9 + 1;
                char picchar = picnum+'0';
                char picstr[2];
                picstr[0] = picchar;
                picstr[1] = '\0';
                char img_path[200];
                img_path[0] = '\0';
                copy2(img_path,str1);
                copy2(img_path,picstr);
                copy2(img_path,str2);
                buttons[i][j]->setIcon(QIcon(QString(img_path)));
            }
            connect(buttons[i][j],&QPushButton::clicked,this,&Mode_Single::box_Clicked);
        }
    }
}


//
//点击方块
//设置dataFlash
//
void Mode_Single::box_Clicked()
{
    //获取被点击的按钮的坐标
    QObject *obj = qobject_cast<QPushButton *>(sender());
    int clickx = 0, clicky = 0;
    for(int i = 0; i < ROWS + 2; ++ i) {
        for(int j = 0; j < COLS + 2; ++ j) {
            if(buttons[i][j] == obj) {
                clickx = i;
                clicky = j;
                game->dataFlash = i * 100 + j;
                break;
            }
        }
    }
    changed = true;
}


//
//关于选项
//
//
void Mode_Single::on_actionabout_clicked(bool checked)
{
    QMessageBox messageBox(QMessageBox::NoIcon,
                               "单人游戏", "单人游戏通过WASD四键进行操作！",
                               QMessageBox::Yes, this);
        int result=messageBox.exec();

        switch (result) {
        case QMessageBox::Yes:
            qDebug()<<"Yes";
            break;
        default:
            break;
        }
}


//
//退出
//
//
void Mode_Single::on_actionexit_clicked(bool checked)
{
    changeShow();
    sound->stop();
    myTimer->stop();
    this->hide();
}


//
//保存
//
//
void Mode_Single::on_actionsave_clicked(bool checked)
{
    saveLoad();
}


//
//saveLoad
//
//
void Mode_Single::saveLoad()
{
    QFile file("saveLoad.txt");
    qDebug()<<"it have get the things";
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << file.errorString();
    }

    int a = 1;
    QTextStream in(&file);
    in << a << " ";
        for(int i = 0 ; i < ROWS + 2 ; i ++) {
            for(int j = 0 ; j < COLS + 2 ; j ++){
                in << gamedata[i][j] << " ";
            }
        }//写入现在的gamedate
        in << game->mary->score << " ";//写入游戏分数
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
//保存并退出
//
//
void Mode_Single::on_actionout_clicked(bool checked)
{
    saveLoad();
    changeShow();
    sound->stop();
    myTimer->stop();
    this->hide();
}


//
//析构
//
//
Mode_Single::~Mode_Single()
{
    delete ui;
}


//
//重新画面
//
//
void Mode_Single::refreshGame()
{
    gameBox_draw_show();
    for(int i = 0 ; i < ROWS + 2 ; ++ i) {
        for(int j = 0 ; j < COLS + 2 ; ++ j) {
            switch (gamedata[i][j]) {
            case 999:
                buttons[i][j]->setIcon(QIcon(QString(":/Images/icon.png")));
                continue;
            case 666:
                buttons[i][j]->setIcon(QIcon(QString(":/Images/player2.png")));
                continue;
            case 47:
                buttons[i][j]->setIcon(QIcon(QString(":/Images/101.png")));
                continue;
            case 105:
                buttons[i][j]->setIcon(QIcon(QString(":/Images/105.png")));
                continue;
            case 106:
                buttons[i][j]->setIcon(QIcon(QString(":/Images/106.png")));
                continue;
            case 48:
                buttons[i][j]->setIcon(QIcon(QString(":/Images/102.png")));
                continue;
            case 49:
                buttons[i][j]->setIcon(QIcon(QString(":/Images/103.png")));
                continue;
            case 50:
                buttons[i][j]->setIcon(QIcon(QString(":/Images/104.png")));
                continue;
            case 0:
                buttons[i][j]->setIcon(QIcon(QString(":/Images/BGP.png")));
                qDebug()<<"BGP";
                continue;
            }

            if(gamedata[i][j] != 0) {
                int picnum = gamedata[i][j] % 9 + 1;
                char picchar = picnum+'0';
                char picstr[2];
                picstr[0] = picchar;
                picstr[1] = '\0';
                char img_path[200];
                img_path[0] = '\0';
                copy2(img_path,str1);
                copy2(img_path,picstr);
                copy2(img_path,str2);
                buttons[i][j]->setIcon(QIcon(QString(img_path)));
            }
        }
    }
}


//
//重排道具
//
//
void Mode_Single::refreshButton()
{
    if(game->needRefresh == true) {
        refreshGame();
        game->needRefresh = false;
        qDebug()<<"画面更新啦";
    }
}


//
//+1s道具
//
//
void Mode_Single::plusThirtys()
{
    if(game->needPlusTime == true) {
        prog_num = prog_num + 30;//增加三十秒
        game->needPlusTime = false;
        qDebug()<<"时间增加啦";
    }
}


//
//flash后的图片更换
//
//
void Mode_Single::drawFlash_after()
{
    if(changed == true && game->canFlash == true) {
        changed = false;
        game->canFlash = false;
        int player = game->flashBeforetoNow[0];//0是原本的，1是人物的新位置
        int bg_pic = game->flashBeforetoNow[1];
        int on_box = game->flashReturn;
        int num1 = player / 100;
        int num2 = player - num1 * 100;
        int num3 = bg_pic / 100;
        int num4 = bg_pic - num3 * 100;
        gameBox_dowm(num1,num2);
        buttons[num3][num4]->setIcon(QIcon(QString(":/Images/icon.png")));
        switch (on_box) {
        case 2://激活一下这个方块就行
            {
                int num1 = game->box1->numofbox_last / 100;
                int num2 = game->box1->numofbox_last - num1 * 100;
                gameBox_on(1,num1,num2);
                break;
            }
        case 3://消掉了
            {
            int num1 = game->box1->numofbox / 100;
            int num2 = game->box1->numofbox-num1 * 100;
            int num3 = game->box1->numofbox_last / 100;
            int num4 = game->box1->numofbox_last-num3 * 100;
            gameBox_on(1,num1,num2);
            update();
            //等待0.18s
            QEventLoop eventloop;
            QTimer::singleShot(180, &eventloop, SLOT(quit()));
            eventloop.exec();
            game->drawLineNum = 0;
            gameBox_draw_show();
            gameBox_dowm(num1,num2);
            gameBox_dowm(num3,num4);//把两个方块的图片设置为BGP
            }
        case 4://两个方块不匹配的情况，也要把上一个给撤回去
            {
                int num1 = game->box1->numofbox_last / 100;
                int num2 = game->box1->numofbox_last-num1 * 100;
                gameBox_back(num1,num2);
                break;
            }
        case 5://点同一个方块
            {
                int num1 = game->box1->numofbox_last / 100;
                int num2 = game->box1->numofbox_last-num1 * 100;
                gameBox_back(num1,num2);
                break;
            }
        }
    } else {
        return;
    }
}


//
//flash的计时器
//
//
void Mode_Single::flash_process()
{
    flash_num --;
    if(flash_num <= 0) {
        flashTimer->stop();
        game->needFlash = false;
        isFlash_on = false;
        flash_num = 50;
    } else {
        qDebug()<<"the num of flash_num is:"<<flash_num;
        game->flashLogic();
        drawFlash_after();
    }
}


//
//flash道具
//
//
void Mode_Single::useFlash()
{
    if(game->needFlash == true && isFlash_on == false) {
        game->needFlash = false;
        isFlash_on = true;
        //flash状态
        flashTimer = new QTimer(this);
        connect(flashTimer, SIGNAL(timeout()), this, SLOT(flash_process()));  //将超时信号和自定义的槽函数连接
        qDebug()<<"进入了flash状态";
        flashTimer->start(100);
        game->needFlash = false;
    } else if(game->needFlash == true && isFlash_on == true) {
        flash_num = flash_num + 50;
        game->needFlash = false;
    }
    game->dataFlash = 9999;
}


//
//freeze的计时器
//
//
void Mode_Single::freeze_process()
{
    if(dizzy_1 == true || dizzy_2 == true) {
        return;
    } else {
        if(can_go == false) {
            can_go = true;
        }
        if(can_go2 == false) {
            can_go2 = true;
        }
    }
}


//
//dizzy的计时器
//时间到了之后把状态解除了
//
void Mode_Single::dizzy_process()
{
    can_go=true;
    can_go2=true;
    dizzy_1=false;
    dizzy_2=false;
}


//
//freeze道具
//
//
void Mode_Single::useFreeze()
{
    if(game->needFreeze == true) {
        game->needFreeze = false;
        freezeTimer = new QTimer(this);
        connect(freezeTimer,SIGNAL(timeout()),this,SLOT(freeze_process()));
        qDebug()<<"启动了freeze函数";
        if(game->freezeMan == 1) {
            can_go = false;
        } else if(game->freezeMan == 2) {
            can_go2 = false;
        }
        freezeTimer->start(3000);
        game->needFreeze = false;
    }
}


//
//dizzy道具
//
//
void Mode_Single::useDizzy()
{
    if(game->needDizzy == true) {
        game->needDizzy = false;
        dizzyTimer = new QTimer(this);
        connect(dizzyTimer,SIGNAL(timeout()),this,SLOT(dizzy_process()));
        qDebug()<<"启动了dizzy函数";
        if(game->dizzyMan == 1) {
            can_go = false;
            dizzy_1 = true;
        } else if(game->dizzyMan == 2) {
            can_go2 = false;
            dizzy_2 = true;
        }
        dizzyTimer->start(10000);
        game->needDizzy = false;
    }
}


//
//hint_timer的绑定
//
//
void Mode_Single::hint_process()
{
    hint_num --;                             //值自减
    if(hint_num <= 0) {
        hintTimer->stop();
        game->needHint = false;
        isHint_on = false;
        hint_num = 20;
    }
    int num1 = game->hintNum[0] / 100;
    int num2 = game->hintNum[0] - num1 * 100;
    int num3 = game->hintNum[1] / 100;
    int num4 = game->hintNum[1] - num3 * 100;
    if(gamedata[num1][num2] != 0 && gamedata[num3][num4] != 0) {
            gameBox_hint_on(num1,num2);
            gameBox_hint_on(num3,num4);
    } else if(gamedata[num1][num2] != 0 && gamedata[num3][num4] == 0) {
        gameBox_back(num1,num2);
        game->hintLogic();
    } else if(gamedata[num1][num2] == 0 && gamedata[num3][num4] != 0) {
        gameBox_back(num3,num4);
        game->hintLogic();
    } else {
        game->hintLogic();
    }
}


//
//hint道具
//
//
void Mode_Single::useHint()
{
    if(game->needHint == true && isHint_on == false) {
        game->needHint = false;
        isHint_on = true;
        //hint状态
        hintTimer = new QTimer(this);
        connect(hintTimer, SIGNAL(timeout()), this, SLOT(hint_process()));  //将超时信号和自定义的槽函数连接
        qDebug()<<"进入了hint状态";
        hintTimer->start(500);
        game->needHint = false;
    } else if(game->needHint == true && isHint_on == true) {
        qDebug()<<"the second time get in hint";
        hint_num = hint_num + 20;
        game->needHint = false;
    }
}


//
//让制定的图片进入激活状态
//num1,num2为目标的坐标
//
void Mode_Single::gameBox_hint_on(int num1,int num2)
{
    int picnum = gamedata[num1][num2] % 9 + 1;
    if(gamedata[num1][num2] == 0) {
        picnum = game->box1->numofbox_now_toDraw % 9 + 1;
    }
    char picchar = picnum + '0';
    char picstr[2];
    picstr[0] = picchar;
    picstr[1] = '\0';
    char img_path[200];
    img_path[0] = '\0';
    copy2(img_path,str1);
    copy2(img_path,picstr);
    copy2(img_path,str4);
    buttons[num1][num2]->setIcon(QIcon(QString(img_path)));
}


//
//让制定的图片进入激活状态
//num1,num2为目标的坐标，mode为玩家
//
void Mode_Single::gameBox_on(int mode,int num1,int num2)
{
    int picnum = gamedata[num1][num2] % 9 + 1;
    if(gamedata[num1][num2] == 0) {
        if(mode == 1) {
            picnum = game->box1->numofbox_now_toDraw % 9 + 1;
        } else if(mode == 2) {
            picnum = game->box2->numofbox_now_toDraw % 9 + 1;
        }
    }
    char picchar = picnum + '0';
    char picstr[2];
    picstr[0] = picchar;
    picstr[1] = '\0';
    char img_path[200];
    img_path[0] = '\0';
    copy2(img_path,str1);
    copy2(img_path,picstr);
    copy2(img_path,str3);
    buttons[num1][num2]->setIcon(QIcon(QString(img_path)));
}


//
//让指定的图片从激活状态变回未激活状态
//num1,num2为目标的坐标
//
void Mode_Single::gameBox_back(int num1,int num2)
{
    if(gamedata[num1][num2] == 0 || gamedata[num1][num2] == 999 || gamedata[num1][num2] == 666) {
        return;
    }
    int picnum = gamedata[num1][num2] % 9 + 1;
    char picchar = picnum + '0';
    char picstr[2];
    picstr[0] = picchar;
    picstr[1] = '\0';
    char img_path[200];
    img_path[0] = '\0';
    copy2(img_path,str1);
    copy2(img_path,picstr);
    copy2(img_path,str2);
    buttons[num1][num2]->setIcon(QIcon(QString(img_path)));
}


//
//让该位置进入空白状态
//num1,num2为目标的坐标
//
void Mode_Single::gameBox_dowm(int num1, int num2)
{
    buttons[num1][num2]->setIcon(QIcon(QString(":/Images/BGP.png")));
}


//
//按键触发后的行为判断
//numofplayer——哪一个玩家，num——1为人物能移动，2为激活了一个方块，3为可以消除两个方块，4、5为需要退回原先激活方块，888是无法消除了
//为了保证函数在五十行以内，进行拆分
//
void Mode_Single::gameBox_how_to_do_Player1(int num)
{
    if(num == 1) {//人物能移动
        buttons[game->mary->lastx][game->mary->lasty]->setIcon(QIcon(QString(":/Images/BGP.png")));
        buttons[game->mary->nowx][game->mary->nowy]->setIcon(QIcon(QString(":/Images/icon.png")));
        refreshButton();
        plusThirtys();
        useHint();
        useFlash();
        useFreeze();
        useDizzy();
    } else if(num == 2) {
        int num1 = game->box1->numofbox_last / 100;
        int num2 = game->box1->numofbox_last - num1 * 100;
        gameBox_on(1,num1,num2);
    } else if(num == 3) {
        int num1 = game->box1->numofbox / 100;
        int num2 = game->box1->numofbox-num1 * 100;
        int num3 = game->box1->numofbox_last / 100;
        int num4 = game->box1->numofbox_last-num3 * 100;
        //需要延长时间，并做线，然后消除之
        gameBox_on(1,num1,num2);
        draw_line_man = 1;
        update();
        //等待0.18s
        QEventLoop eventloop;
        QTimer::singleShot(180, &eventloop, SLOT(quit()));
        eventloop.exec();
        game->drawLineNum = 0;
        gameBox_draw_show();
        gameBox_dowm(num1,num2);
        gameBox_dowm(num3,num4);//把两个方块的图片设置为BGP
    } else if(num == 4 || num == 5) {
        int num1 = game->box1->numofbox_last / 100;
        int num2 = game->box1->numofbox_last - num1 * 100;
        gameBox_back(num1,num2);
    } else if(num==888) {
        QMessageBox messageBox(QMessageBox::NoIcon,
                                   "抱歉！", "场上已经没有可以消除的元素了！",
                                   QMessageBox::Yes, this);
            int result=messageBox.exec();
            switch (result) {
            case QMessageBox::Yes:
                qDebug()<<"Yes";
                break;
            default:
                break;
            }
    }
}

void Mode_Single::gameBox_how_to_do_Player2(int num)
{
    if(num == 1) {
        buttons[game->tom->lastx][game->tom->lasty]->setIcon(QIcon(QString(":/Images/BGP.png")));
        buttons[game->tom->nowx][game->tom->nowy]->setIcon(QIcon(QString(":/Images/player2.png")));
        refreshButton();
        plusThirtys();
        useHint();
        useFlash();
        useFreeze();
        useDizzy();
    } else if(num == 2) {
        int num1 = game->box2->numofbox_last/100;
        int num2 = game->box2->numofbox_last-num1*100;
        gameBox_on(2,num1,num2);
    } else if(num == 3) {
        int num1 = game->box2->numofbox/100;
        int num2 = game->box2->numofbox-num1*100;
        int num3 = game->box2->numofbox_last/100;
        int num4 = game->box2->numofbox_last-num3*100;
        //需要延长时间，并做线，然后消除之
        gameBox_on(2,num1,num2);
        draw_line_man = 2;
        update();
        //等待0.18s
        QEventLoop eventloop;
        QTimer::singleShot(180, &eventloop, SLOT(quit()));
        eventloop.exec();
        game->drawLineNum = 0;
        gameBox_draw_show();
        gameBox_dowm(num1,num2);
        gameBox_dowm(num3,num4);//把两个方块的图片设置为BGP
    } else if(num == 4||num == 5) {
        int num1 = game->box2->numofbox_last/100;
        int num2 = game->box2->numofbox_last-num1*100;
        gameBox_back(num1,num2);
    } else if(num == 888) {
        QMessageBox messageBox(QMessageBox::NoIcon,
                                   "抱歉！", "场上已经没有可以消除的元素了！",
                                   QMessageBox::Yes, this);
            int result=messageBox.exec();
            switch (result) {
            case QMessageBox::Yes:
                qDebug()<<"Yes";
                break;
            default:
                break;
            }
    }
}

void Mode_Single::gameBox_how_to_do(int numofplayer,int num)
{
    if(numofplayer == 1) {
        gameBox_how_to_do_Player1(num);
    }
    if(numofplayer == 2) {
        gameBox_how_to_do_Player2(num);
    }
}


//
//将线回归本源
//
//
void Mode_Single::drawLines_down()
{
    gameBox_draw_show();
}


//
//eventfilter过滤事件
//
//
bool Mode_Single::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == this->ui->centralwidget && event->type() == QEvent::Paint) {
        drawLines();
    }
    return QWidget::eventFilter(watched,event);
}


//
//看看需要隐藏哪些图片
//num1\num2为前一个点的坐标，num3num4为后一个点的坐标
//
void Mode_Single::gameBox_draw_hide(int num1,int num2,int num3,int num4)
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
void Mode_Single::gameBox_draw_show()
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
void Mode_Single::drawLines()
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
            int num1 = game->box1->Line_Nocorner[0][0] / 100;
            int num2 = game->box1->Line_Nocorner[0][0] - num1 * 100;
            int num3 = game->box1->Line_Nocorner[0][1] / 100;
            int num4 = game->box1->Line_Nocorner[0][1] - 100 * num3;
            p.drawLine(num1 * 65 + 32.5,num2 * 65 + 32.5,num3 * 65 + 32.5,num4 * 65 + 32.5);
            gameBox_draw_hide(num1,num2,num3,num4);
            break;
        }
        case 2:
        {
            int num1 = game->box1->Line_Onecorner[0][0] / 100;
            int num2 = game->box1->Line_Onecorner[0][0] - num1 * 100;
            int num3 = game->box1->Line_Onecorner[0][1] / 100;
            int num4 = game->box1->Line_Onecorner[0][1] - 100 * num3;
            int num5 = game->box1->Line_Onecorner[1][0] / 100;
            int num6 = game->box1->Line_Onecorner[1][0] - num5 * 100;
            int num7 = game->box1->Line_Onecorner[1][1] / 100;
            int num8 = game->box1->Line_Onecorner[1][1] - 100 * num7;
            p.drawLine(num1 * 65 + 32.5,num2 * 65 + 32.5,num3 * 65 + 32.5,num4 * 65 + 32.5);
            p.drawLine(num5 * 65 + 32.5,num6 * 65 + 32.5,num7 * 65 + 32.5,num8 * 65 + 32.5);
            gameBox_draw_hide(num1,num2,num3,num4);
            gameBox_draw_hide(num5,num6,num7,num8);
            if(gamedata[num3][num4] != 999)
                buttons[num3][num4]->hide();
            break;
        }
        case 3:
        {
            int num1 = game->box1->Line_Twocorner[0][0] / 100;
            int num2 = game->box1->Line_Twocorner[0][0] - num1 * 100;
            int num3 = game->box1->Line_Twocorner[0][1] / 100;
            int num4 = game->box1->Line_Twocorner[0][1] - 100 * num3;
            int num5 = game->box1->Line_Twocorner[1][0] / 100;
            int num6 = game->box1->Line_Twocorner[1][0] - num5 * 100;
            int num7 = game->box1->Line_Twocorner[1][1] / 100;
            int num8 = game->box1->Line_Twocorner[1][1] - 100 * num7;
            int num9 = game->box1->Line_Twocorner[2][0] / 100;
            int num10 = game->box1->Line_Twocorner[2][0] - num9 * 100;
            int num11 = game->box1->Line_Twocorner[2][1] / 100;
            int num12 = game->box1->Line_Twocorner[2][1] - 100 * num11;
            p.drawLine(num1 * 65 + 32.5,num2 * 65 + 32.5,num3 * 65 + 32.5,num4 * 65 + 32.5);
            p.drawLine(num5 * 65 + 32.5,num6 * 65 + 32.5,num7 * 65 + 32.5,num8 * 65 + 32.5);
            p.drawLine(num9 * 65 + 32.5,num10 * 65 + 32.5,num11 * 65+ 32.5,num12 * 65 + 32.5);
            gameBox_draw_hide(num1,num2,num3,num4);
            gameBox_draw_hide(num5,num6,num7,num8);
            gameBox_draw_hide(num9,num10,num11,num12);
            if(gamedata[num3][num4]!=999)
                buttons[num3][num4]->hide();
            if(gamedata[num7][num8]!=999)
                buttons[num7][num8]->hide();
            break;
        }
    }
}


//
//键盘按下触发事件
//
//
void Mode_Single::pressEvent_1(QKeyEvent *ev)
{
    if(ev->key() == Qt::Key_W) {
       int num_up = game->moveUp(1);
       gameBox_how_to_do(1,num_up);
    }
    if(ev->key() == Qt::Key_S) {
       int num_down = game->moveDown(1);
       gameBox_how_to_do(1,num_down);
    }
    if(ev->key() == Qt::Key_A) {
       int num_left = game->moveLeft(1);
       gameBox_how_to_do(1,num_left);
    }
    if(ev->key() == Qt::Key_D) {
       int num_right = game->moveRight(1);
       gameBox_how_to_do(1,num_right);
    }
}

void Mode_Single::pressEvent_2(QKeyEvent *ev)
{
    if(ev->key() == Qt::Key_Up) {
       int num_up = game->moveUp(2);
       gameBox_how_to_do(2,num_up);
    }
    if(ev->key() == Qt::Key_Down) {
       int num_down = game->moveDown(2);
       gameBox_how_to_do(2,num_down);
    }
    if(ev->key() == Qt::Key_Left) {
       int num_left = game->moveLeft(2);
       gameBox_how_to_do(2,num_left);
    }
    if(ev->key() == Qt::Key_Right) {
       int num_right = game->moveRight(2);
       gameBox_how_to_do(2,num_right);
    }
}

void Mode_Single::pressEvent_3(QKeyEvent *ev)
{
    if(ev->key() == Qt::Key_S) {
       int num_up = game->moveUp(1);
       gameBox_how_to_do(1,num_up);
    }
    if(ev->key() == Qt::Key_W) {
       int num_down = game->moveDown(1);
       gameBox_how_to_do(1,num_down);
    }
    if(ev->key() == Qt::Key_D) {
       int num_left = game->moveLeft(1);
       gameBox_how_to_do(1,num_left);
    }
    if(ev->key() == Qt::Key_A) {
       int num_right = game->moveRight(1);
       gameBox_how_to_do(1,num_right);
    }
}

void Mode_Single::pressEvent_4(QKeyEvent *ev)
{
    if(ev->key() == Qt::Key_Down) {
       int num_up = game->moveUp(2);
       gameBox_how_to_do(2,num_up);
    }
    if(ev->key() == Qt::Key_Up) {
       int num_down = game->moveDown(2);
       gameBox_how_to_do(2,num_down);
    }
    if(ev->key() == Qt::Key_Right) {
       int num_left = game->moveLeft(2);
       gameBox_how_to_do(2,num_left);
    }
    if(ev->key() == Qt::Key_Left) {
       int num_right = game->moveRight(2);
       gameBox_how_to_do(2,num_right);
    }
}

void Mode_Single::keyPressEvent(QKeyEvent *ev)
{
    if(can_go) {
        pressEvent_1(ev);
    }
    if(can_go2) {
        pressEvent_2(ev);
    }
    if(dizzy_1) {
        pressEvent_3(ev);
    }
    if(dizzy_2) {
        pressEvent_4(ev);
    }
    QWidget::keyPressEvent(ev);
}
