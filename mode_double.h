#ifndef Mode_Double_H
#define Mode_Double_H

#include <QMainWindow>
#include "mode_single.h"

namespace Ui {
class Mode_Double;
}

class Mode_Double : public Mode_Single
{
    Q_OBJECT

public:
    Mode_Double(QWidget *parent = nullptr);
    Mode_Double(int array[],int score1,int score2,int time,QWidget *parent = nullptr);

    void saveLoad();
    void drawLines();
    void gameBox_draw_show();
    void gameBox_draw_hide(int num1,int num2,int num3,int num4);
    void drawLines_down();

    ~Mode_Double();

private slots:
    void process();                                                     //计时器
    void on_actionabout_clicked(bool checked);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Ui::Mode_Double *ui;
    //1和2显示一号玩家的，3和4显示2号玩家的
    QLabel *label_4;                                                    //label4:显示玩家图片
    QLabel *label_5;                                                    //label5:显示玩家的分数
};

#endif // Mode_Double_H
