#pragma once
#include "utils.h"
#include "custom_draws.h"

struct V2{//Could have used this in other places to great effect but i forgor.
    float x;
    float y;
};
struct V3{
    float x;
    float y;
    float z;
};
V3 add(V3 a,V3 b){//off, passing around *6* floats
    return {a.x+b.x,a.y+b.y,a.z+b.z};
}
V2 project(V3 a){
    return {a.x/a.z,a.y/a.z};//todo? multiply by a factor or something
}
/*uint8_t building_top(float x,float y){
    if(x>=0.0f&&y>=0.0f)//To avoid a warning
        return 0x1;
    return 0x2;
}*/

uint8_t building_side(float x,float y){
    if(((x>=0.1f&&x<=0.4)||(x>=0.6f&&x<=0.9))&&
        ((y>=0.1f&&y<=0.3)||(y>=0.4f&&y<=0.7)))//Check if it should be a window
        return 0x0;
    return 0x1;
}

typedef uint8_t (*texture)(float x,float y);

float M11ToPixX(float x){
    return (x+1.0f)*(((float)SCREEN_SIZE)/2.0f);
}
float M11ToPixY(float y){
    return (float)HORIZON+(float)HORIZON*y;
}
/*float M11ToPixY(float y){
    return ((y/((float)-HORIZON))*2.0f)-1.0f;
}*/
float PixToM11X(float x){
    return (x*(2.0f/((float)SCREEN_SIZE)))-1.0f;
}

//Guarantees : p1 is top-left,p1 is bottom-left,p3 is bottom-right,p2 is top-right
//also, p0.y==p1.y,p2.y==p3.y,p0.z==p1.z,p2.z==p3.z,aka d1=(?,0,0)
//Given that, this function works, otherwise idk.
//This is the only one i'll need for this particular game.
void rasterize_quad(V3 p0,V3 d1,V3 d2,texture tex){
    V2 pp0=project(p0);
    V3 p1=add(p0,d2);
    V2 pp1=project(p1);
    V3 p2=add(p0,d1);
    V2 pp2=project(p2);
    V3 p3=add(add(p0,d1),d2);
    V2 pp3=project(p3);
    float mx=min(pp0.x,pp1.x);//Note:should always be equal
    float Mx=max(pp2.x,pp3.x);//Note:should always be equal
    int mx_pix=floor(M11ToPixX(mx));
    int Mx_pix=ceil(M11ToPixX(Mx));
    for(int x=mx_pix;x<=Mx_pix;++x){
        if(x>=0&&x<SCREEN_SIZE){
            //tracef("X:%d",x);
            float my=(pp0.y*(float)(Mx_pix-x)+pp2.y*(float)(x-mx_pix))/(float)(Mx_pix-mx_pix);
            float My=(pp1.y*(float)(Mx_pix-x)+pp3.y*(float)(x-mx_pix))/(float)(Mx_pix-mx_pix);
            float frac_x=PixToM11X((float)x);
            //Man I don't know how right this is I did the math by hand. If it works, I guess I'll keep it...
            float tx=(p2.x-frac_x*p2.z)/(frac_x*(p0.z-p2.z)-(p0.x-p2.x));
            int my_pix=floor(M11ToPixY(my));
            int My_pix=ceil(M11ToPixY(My));
            if(my_pix>0)
                for(int y=my_pix;y<=My_pix;++y){
                    float ty=((float)(y-my_pix))/((float)(My_pix-my_pix));
                    setPixel(tex(tx,ty),x,y);
                }
        }
    }
}
