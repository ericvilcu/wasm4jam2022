#pragma once
#include "utils.h"
void draw_ship(float camera_pos_x,float camera_pos_y,float x,float y,float fx,float fy,char colors){
    int clr1=colors&3;
    int clr2=(colors&20)>>2;
    float md=max(fx,fy);
    fx=fx/md;
    fy=fy/md;

    
}

/*void draw_ship(float x,float y,float rotation,char colors){
    draw_ship(float x,float y,float fx,float fy,char colors);
}*/