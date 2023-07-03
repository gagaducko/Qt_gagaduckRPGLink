#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QSound>
#include <QMessageBox>
#include <QTimer>
#include <QtDebug>
#include <QPushButton>
#include "mode_single.h"
#include "mode_double.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Mode_Single * singlegame;
    Mode_Double * doublegame;
    QSound *sound = new QSound(":/Sounds/BGM_menu.wav", this);

    void startGame_single();
    void startGame_double();
    void loadGame();
    void exitGame();

private slots:
    void on_singleButton_clicked(bool checked);
    void on_doubleButton_clicked(bool checked);
    void on_loadButton_clicked(bool checked);
    void on_exitButton_clicked(bool checked);
    void on_action_exit_click(bool checked);
    void on_action_about_click(bool checked);
    void on_action_mode_click(bool checked);
    void on_action_item_click(bool checked);
    void on_action_single_click(bool checked);
    void on_action_double_click(bool checked);
    void on_actionload_click(bool checked);
    void process();

private:
    Ui::MainWindow *ui;
    QTimer *is_show_timer;
};
#endif // MAINWINDOW_H
