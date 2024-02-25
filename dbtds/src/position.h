#pragma once
#include "utils.h"
//NOTE: I am using integer overflow intentionally.
#define FakeBitShift 24
#define ToFakeMagnitude ((float)(1<<FakeBitShift))
#define ToRealMagnitude (1.0f/ToFakeMagnitude)

struct FakeVector2{
    typedef int FkFLT;
    static_assert(sizeof(FkFLT)==4);
    FkFLT x, y;
    inline FakeVector2() = default;
    inline FakeVector2(FkFLT x, FkFLT y):x{x},y{y}{};
    inline FakeVector2(float x, float y):x{fake(x)},y{fake(y)}{};
    static inline float real(FkFLT d) { return (float)d*ToRealMagnitude; }
    inline void ToCamera(const FakeVector2& camera, int& outX, int& outY) const {
        int x = this->x-camera.x;
        int y = this->y-camera.y;
        outX = (int)((unsigned int)x>>FakeBitShift);
        outY = (int)((unsigned int)y>>FakeBitShift);
    }
    static inline FkFLT fake(float d) { return (FkFLT)(d*ToFakeMagnitude); }
    static inline FkFLT fakeI(int d) { return (FkFLT)(d<<FakeBitShift); }
    inline bool squareHits(float size = 0.5f){
        return max((int)abs(x),(int)abs(y))<=fake(size);
    }
    inline FakeVector2 operator+(const FakeVector2& ot) const {
        return FakeVector2(x+ot.x,y+ot.y);
    }
    inline FakeVector2 operator-(const FakeVector2& ot) const {
        return FakeVector2(x-ot.x,y-ot.y);
    }
    inline FakeVector2 operator*(const FakeVector2& ot) const {
        return FakeVector2((FkFLT)(((long long)x*ot.x)>>FakeBitShift),(FkFLT)(((long long)y*ot.y)>>FakeBitShift));
    }
    inline FakeVector2 operator*(float v) const {
        FkFLT fk = fake(v);
        FkFLT X=(FkFLT)(((long long)x*fk)>>FakeBitShift);
        FkFLT Y=(FkFLT)(((long long)y*fk)>>FakeBitShift);
        return FakeVector2(X,Y);
    }
    inline FakeVector2 closestVectorTo(const FakeVector2& target) const{
        return target-*this;
    }
    inline FakeVector2 fromTopRightCornerToCenter() const{
        return FakeVector2(x+((SCREEN_SIZE/2)<<FakeBitShift),y+((SCREEN_SIZE/2)<<FakeBitShift));
    }

    // operations that cause mutations
    inline FakeVector2& normalize(){
        //working with fixed-point shit sounds like a pain...
        float rx=real(x),ry=real(y);
        ::normalize(rx,ry);
        x=fake(rx);y=fake(ry);
        return *this;
    }
    inline FakeVector2& decelerate(float v){
        FkFLT fk = fake(v);
        x=(FkFLT)(((long long)x*fk)>>FakeBitShift);
        y=(FkFLT)(((long long)y*fk)>>FakeBitShift);
        return *this;
    }
    inline FakeVector2& rot(uint8_t rot_type){
        float xr = real(x);float yr = real(y);
        rotate(xr,yr,rot_type);
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
    x&=255;
    y&=255;
    if(!isInScreen(x,y))
        return;
    int offset=((x&3)<<1);
    int idx=SCREEN_SIZE_x_BYTES*y+(x>>2);
    uint8_t initial=FRAMEBUFFER[idx];
    initial^=initial&(3<<offset);
    initial|=(clr)<<offset;
    FRAMEBUFFER[idx]=initial;
}
