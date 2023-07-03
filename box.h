#ifndef BOX_H
#define BOX_H

const int ROWSofbox = 10;
const int COLSofbox = 10;

class Box{
public:
    bool isActive();
    void isBox_on();
    Box();

    int num_onBox;//激活方块的数量
    int lastx_on,lasty_on;
    int numofbox;//现在这个激活方块的代数a*100+b（a,b）
    int numofbox_now_toDraw;//为了draw一下
    int numofbox_last;//原来激活的那个方块的代数c,d。


    int Line_Nocorner[2][2];//y上的数字用于判断横0竖1不需要连2
    int Line_Onecorner[3][3];//y上的数字用于判断拆分成两条线的话是横0竖1不需要连2，最后一个点是四个方向的corner
    int Line_Twocorner[4][5];//y上的数字用于判断拆分成两条线的话是横0竖1不需要连2，最后两个点是四个方向的corner

};

#endif // BOX_H
