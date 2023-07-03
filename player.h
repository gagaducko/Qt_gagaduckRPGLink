#ifndef PLAYER_H
#define PLAYER_H

#include <ctime>
#include <cstdlib>

class Player{
public:
    Player(int num);
    int score = 0;
    int nowx,nowy;
    int lastx,lasty;
    int playernum = 999;
    int player2num = 666;

private:

};


#endif // PLAYER_H
