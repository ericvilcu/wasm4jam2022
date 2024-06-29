#pragma once
#include "wasm4.h"

#define ARENA_SIZE 256
#define HALF_ARENA_SIZE (ARENA_SIZE/2)
#define SCREEN_SIZE_MINUS_1 ((int)(SCREEN_SIZE-1))
#define SCREEN_SIZE_x_BYTES ((int)(SCREEN_SIZE/4))
#define COLOR_BACKGROUND 0b00
#define COLOR_BACKGROUND_ACCENT 0b01
#define COLOR_PLAYER 0b10
#define COLOR_ENEMY 0b11
#define DYNAMIC_FALLOFF_THRESHOLD 2.0f

#define ALL_PLAYER_COLORS 0b1011
#define ALL_ENEMY_COLORS 0b1110
#define fSCREEN_SIZE ((float)SCREEN_SIZE)
#define BULLET_INVALID -3.40282347E38f
#define INITIAL_WATER_TIME 300
#define DASH_COOLDOWN 30


constexpr float gravity=0.01f/6.0f;
constexpr float speed=0.01f;
constexpr float dash_speed=4.000f;
constexpr float falloff=0.98f;
constexpr float brake=0.90f;
constexpr float BULLET_START_SPEED=1.0f;

inline int mod_I(int x,int mx){
    int md=x%mx;
    if(md<0)return mx+md;
    return md;
}
inline float rand_range_seed(float mn,float mx,int sd){
    float bts=(float)(sd&0b11111)/((float)(0b11111));
    return bts*(mx-mn)+mn;
}
inline int sign(float x){
    if(x<0)return -1;
    else if(x>0) return 1;
    return 0;
}
inline float pow2(float x){
    return x*x;
}
inline int floor(float x){
    return (int)x;
}
inline int ceil(float x){
    return (int)(x-1e-9)+(x>0?1:(x<0?-1:0));
}
inline int max(int x,int y){
    return (x>y?x:y);
}
inline float max(float x,float y){
    return (x>y?x:y);
}
inline float min(float x,float y){
    return (x<y?x:y);
}
inline int min(int x,int y){
    return (x<y?x:y);
}
inline uint8_t min(uint8_t x,uint8_t y){
    return (x<y?x:y);
}
inline float clamp(float x,float mn,float mx){
    return min(max(x,mn),mx);
}
inline int clampI(int x,int mn,int mx){
    return (x<mn?mn:(x>mx?mx:x));
}
inline unsigned int abs(int x){
    return (unsigned int)(x>0?x:-x);
}
inline float abs(float x){
    return (x>0?x:-x);
}
inline float fmodf(float x,float mx){
    while(x>=mx)x-=mx;
    while(x<0.0f)x+=mx;//mathematically correct?
    return x;
}

inline float looped_polar_opposite(float pos){
    return fmodf(pos+(float)HALF_ARENA_SIZE,(float)ARENA_SIZE);
}
/**
 * returns w/ magnitude.
 */
inline float best_looped_dir(float pos,float target){
    //both are 0..ARENA_SIZE_X-1
    float dx=target-pos;
    if(abs(dx)<=HALF_ARENA_SIZE)
        return dx;
    else return ((float)-sign(dx))*(((float)ARENA_SIZE)-abs(dx));
}
inline float best_middle_wrap(float x1,float x2){
    float dx=x1-x2;
    float avg=(x1+x2)/2.0f;
    if(abs(dx)<=HALF_ARENA_SIZE)
        return avg;
    else
        return fmodf(avg+(float)HALF_ARENA_SIZE,(float)ARENA_SIZE);
}
inline float square_distance(float x1,float y1,float x2,float y2){
    return pow2(x1-x2)+pow2(y1-y2);
}
inline float square_wrap_distance(float x1,float y1,float x2,float y2){
    float midX=best_middle_wrap(x1,x2);
    float midY=(y1+y2)/2.0f;
    return 2.0f*min(square_distance(x1,y1,midX,midY),square_distance(x2,y2,midX,midY));
}
//https://en.wikipedia.org/wiki/Methods_of_computing_square_roots#A_two-variable_iterative_method
//Since I'm mostly using this to normalize almost-normal vectors, it is fast.
inline float sqrt(float x,float error=1e-3f){
    if(x<0)return -sqrt(-x,error);//it is a convention,ok?
    if(x>2){
        return 2.0f*sqrt(x*0.25f,error);
    }
    if(x==0)return 0.0f;
    if(x<0.5){
        return 0.5f*sqrt(x*4.00f,error);
    }
    float a=x;
    float b=x-1.0f;
    error*=error;
    while (abs(b)>error)
    {
        a=a-a*b/2.0f;
        b=b*b*(b-3.0f)/4.0f;
    }
    return a;
}

inline void normalize(float&fx,float&fy){
    float md=sqrt((fx*fx)+(fy*fy));
    fx/=md;
    fy/=md;
}
inline void normalize_squareify(float&fx,float&fy){
    //B/C I do NOT  trust that sqrt function
    float md=(abs(fx)>abs(fy)?abs(fx):abs(fy));
    fx/=md;
    fy/=md;
    normalize(fx,fy);
}

enum RotationType{
    ROT_NORMAL_C=0,//C = clock-wise
    ROT_NORMAL_T=1,//T = trigonometrically
    ROT_HALF_C=2,
    ROT_HALF_T=3,
    ROT_NINETY_PERCENT_C=4,
    ROT_NINETY_PERCENT_T=5,
};
void rotate(float&fx, float&fy, uint8_t idx);

inline void rot_towards_N_E_C(float&fx,float&fy,float dx,float dy){
    //rotate based on the direction of the cross product
    if(abs(dx)+abs(dy)<0.1)return;
    float cross_product_z=dx*fy-dy*fx;
    if(cross_product_z>0.0f)
        rotate(fx,fy,2);
    else
        rotate(fx,fy,3);
}

inline void rot_towards(float&fx,float&fy,float dx,float dy){
    //rotate based on the direction of the cross product
    rot_towards_N_E_C(fx,fy,dx,dy);
    rot_towards_N_E_C(fx,fy,dx,dy);
}

inline int moved(int X,float dX){
    return X+(int)(abs(dX))*(dX>0?1:-1);
}

#define SHIP_SIZE 7
#define SHIP_OFFSCREEN_MIN ((int)(-SHIP_SIZE))
#define SHIP_OFFSCREEN_MAX ((int)(SCREEN_SIZE+SHIP_SIZE))
