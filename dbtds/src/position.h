#pragma once
#include "utils.h"
struct Position{
    typedef unsigned int FkFLT;
    FkFLT x, y;
    inline Position() = default;
    inline Position(FkFLT x, FkFLT y):x{x},y{y}{};
    inline Position(float x, float y):x{fake(x)},y{fake(y)}{};
    static inline float real(FkFLT d) { return (float)d*0x1p-12f; }
    static inline FkFLT fake(float d) { return (FkFLT)(d*0x1p12f); }
    inline Position& normalize(){
        //working with fixed-point shit sounds like a pain...
        float rx=real(x),ry=real(y);
        ::normalize(rx,ry);
        x=fake(rx);y=fake(ry);
        return *this;
    }
    inline Position operator+(const Position& ot){
        return Position(x+ot.x,y+ot.y);
    }
    inline Position operator-(const Position& ot){
        return Position(x-ot.x,y-ot.y);
    }
    inline Position operator*(const Position& ot){
        return Position((FkFLT)(((unsigned long long)x*ot.x)>>24),(FkFLT)(((unsigned long long)y*ot.y)>>24));
    }
    inline Position operator*(float v){
        FkFLT fk = fake(v);
        FkFLT X=(FkFLT)(((unsigned long long)x*fk)>>24);
        FkFLT T=(FkFLT)(((unsigned long long)y*fk)>>24);
        return Position(x,y);
    }
    inline Position& decelerate(float v){
        FkFLT fk = fake(v);
        x=(FkFLT)(((unsigned long long)x*fk)>>24);
        y=(FkFLT)(((unsigned long long)y*fk)>>24);
        return *this;
    }
    inline Position& rot1(){
        float xr = real(x);float yr = real(y);
        rot_plus1(xr,yr);
        x=fake(xr);y=fake(yr);
        return *this;
    }
    inline Position& rotMinus1(){
        float xr = real(x);float yr = real(y);
        rot_minus1(xr,yr);
        x=fake(xr);y=fake(yr);
        return *this;
    }
    inline Position& rot1H(){
        float xr = real(x);float yr = real(y);
        rot_plus1H(xr,yr);
        x=fake(xr);y=fake(yr);
        return *this;
    }
    inline Position& rotMinus1H(){
        float xr = real(x);float yr = real(y);
        rot_minus1H(xr,yr);
        x=fake(xr);y=fake(yr);
        return *this;
    }
};

inline bool isInScreen(int x,int y){
    return !(x>SCREEN_SIZE_MINUS_1 || x<0 || y>SCREEN_SIZE_MINUS_1 || y<0);
}

inline void setPixel(uint8_t clr,int x,int y){
    //Too safe?
    //no.
    //bloody inefficient, but we have CPU speed more than anything else.
    if(!isInScreen(x,y))
        return;
    int offset=((x&3)<<1);
    int idx=SCREEN_SIZE_x_BYTES*y+(x>>2);
    uint8_t initial=FRAMEBUFFER[idx];
    initial^=initial&(3<<offset);
    initial|=(clr)<<offset;
    FRAMEBUFFER[idx]=initial;
}
