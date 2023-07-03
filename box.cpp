#include "box.h"

Box::Box()
{
    num_onBox = 0;
}

//
//判断激活方块的数量
//达到两个返回true；否则返回false
//
bool Box::isActive()
{
    if (num_onBox == 2) {
        return true;//激活方块数量达到2，就返回true
    } else {
        return false;
    }
}

//
//激活方块数量+1
//
//
void Box::isBox_on()
{
    num_onBox++;
}

