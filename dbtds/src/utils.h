#pragma once
#include "wasm4.h"
#define SCREEN_SIZE_MINUS_1 ((int)(SCREEN_SIZE-1))
#define SCREEN_SIZE_x_BYTES ((int)(SCREEN_SIZE/4))
#define COLOR_BACKGROUND 0b00//Brightness should be low
#define COLOR_BACKGROUND_ACCENT 0b01//Brightness should be medium-low. high saturation.
#define COLOR_PLAYER 0b10//Note:Yellow? to see the player better?
#define COLOR_ENEMY 0b11//Note to self: ALWAYS BRIGHT
#define WATER_Y 160
#define DYNAMIC_FALLOFF_THRESHOLD 2.0f

#define ALL_PLAYER_COLORS 0b1011
#define ALL_ENEMY_COLORS 0b1110
#define ARENA_SIZE_X 200
#define HALF_ARENA_SIZE_X (ARENA_SIZE_X/2)
#define fSCREEN_SIZE ((float)SCREEN_SIZE)
#define BULLET_INVALID -3.40282347E38f
#define INITIAL_WATER_TIME 300
#define DASH_COOLDOWN 30


constexpr float gravity=0.01f/6.0f;
constexpr float speed=0.01f;
constexpr float dash_speed=1.000f;
constexpr float falloff=0.997f;
constexpr float brake=0.90f;
constexpr float BULLET_START_SPEED=1.0f;
/*template<T>
T max(T a,T b){
    return (a>b?a:b);
}
template<T>
T min(T a,T b){
    return (a<b?a:b);
}*/
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
inline float clamp(float x,float mn,float mx){
    return min(max(x,mn),mx);
}
inline int clampI(int x,int mn,int mx){
    return (x<mn?mn:(x>mx?mx:x));
}
inline int abs(int x){
    return (x>0?x:-x);
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
    return fmodf(pos+(float)HALF_ARENA_SIZE_X,(float)ARENA_SIZE_X);
}
/**
 * returns w/ magnitude.
 */
inline float best_looped_dir(float pos,float target){
    //both are 0..ARENA_SIZE_X-1
    float dx=target-pos;
    if(abs(dx)<=HALF_ARENA_SIZE_X)
        return dx;
    else return ((float)-sign(dx))*(((float)ARENA_SIZE_X)-abs(dx));
}
inline float best_middle_wrap(float x1,float x2){
    float dx=x1-x2;
    float avg=(x1+x2)/2.0f;
    if(abs(dx)<=HALF_ARENA_SIZE_X)
        return avg;
    else
        return fmodf(avg+(float)HALF_ARENA_SIZE_X,(float)ARENA_SIZE_X);
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
constexpr float COS_ROT=0.99452189536f;
constexpr float SIN_ROT=0.10452846326f;

constexpr float COS_ROT_HALF=0.99862953475f;
constexpr float SIN_ROT_HALF=0.05233595624f;

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

inline void rot_plus1(float&fx,float&fy){
    float ox=fx;
    float oy=fy;

    fx=ox*COS_ROT+oy*-SIN_ROT;
    fy=ox*SIN_ROT+oy*COS_ROT;
    normalize(fx,fy);
}

inline void rot_minus1(float&fx,float&fy){
    float ox=fx;
    float oy=fy;

    fx=ox*COS_ROT+oy*SIN_ROT;
    fy=ox*-SIN_ROT+oy*COS_ROT;
    normalize(fx,fy);
}

inline void rot_plus1H(float&fx,float&fy){
    float ox=fx;
    float oy=fy;

    fx=ox*COS_ROT_HALF+oy*-SIN_ROT_HALF;
    fy=ox*SIN_ROT_HALF+oy*COS_ROT_HALF;
    normalize(fx,fy);
}

inline void rot_minus1H(float&fx,float&fy){
    float ox=fx;
    float oy=fy;

    fx=ox*COS_ROT_HALF+oy*SIN_ROT_HALF;
    fy=ox*-SIN_ROT_HALF+oy*COS_ROT_HALF;
    normalize(fx,fy);
}

inline void rot_towards_N_E_C(float&fx,float&fy,float dx,float dy){
    //rotate based on the direction of the cross product
    if(abs(dx)+abs(dy)<0.1)return;
    float cross_product_z=dx*fy-dy*fx;
    if(cross_product_z>0.0f)
        rot_minus1H(fx,fy);
    else
        rot_plus1H(fx,fy);
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

inline char* itoa(int x,char*t){
    if(x>10){
        t=itoa(x/10,t);
    }
    *t='0'+(x%10);
    return t+1;
}
