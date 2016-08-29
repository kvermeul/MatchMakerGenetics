#include "player.h"

player::player(std::string fn, std::string ln):first_name(fn),last_name(ln){

}


player::~player(){

}

bool player::canPlay(position p) const
{
    if(values.count(p)>0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
