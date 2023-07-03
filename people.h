#ifndef PEOPLE_H
#define PEOPLE_H

class people
{
public:
    people(int x,int y);
    void move_up();
    void move_down();
    void move_left();
    void move_right();
    bool isWalkable();
    void renewscore();

private:
    int score=0;
    int peoplenum=999;
    int nowx,nowy;
};

#endif // PEOPLE_H
