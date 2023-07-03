#include "player.h"

Player::Player(int num)
{
    if (num == 1) {
        nowx=rand() % 12;
        nowy = 0;
        lastx = nowx;
        lasty = nowy;
    } else {
        nowx = rand() % 12;
        nowy = 11;
        lastx = nowx;
        lasty = nowy;
    }
}
