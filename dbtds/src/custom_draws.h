#pragma once
#include "utils.h"
#include "wasm4.h"
#include "position.h"

inline void switchPixelIfBackground(int x,int y){
    if(x>SCREEN_SIZE_MINUS_1 || x<0 || y>SCREEN_SIZE_MINUS_1 || y<0)
        return;
    int offset=((x&3)<<1);
    int idx=SCREEN_SIZE_x_BYTES*y+(x>>2);
    uint8_t initial=FRAMEBUFFER[idx];
    if((initial&(2<<offset))!=0){
        uint8_t swapped=initial^(uint8_t)(1<<offset);
        FRAMEBUFFER[idx]=swapped;
    }
}

inline void drawLine(uint8_t clr,int start_x,int start_y,int end_x,int end_y){
    bool bias=false;
    if(abs(start_x-end_x)<abs(start_y-end_y)){
        if(start_y>end_y){
            if(start_x<end_x)
                bias=false;
            else
                bias=true;
            int temp=start_x;
            start_x=end_x;end_x=temp;
            temp=start_y;
            start_y=end_y;end_y=temp;
        }
        else
            if(start_x>end_x)
                bias=true;
            else
                bias=false;
        for(int j=start_y;j<=end_y;++j){
            float df=((((float)(j-start_y))/((float)(end_y-start_y)))*(float)(end_x-start_x))+(float)start_x;
            int i=(bias?floor(df):ceil(df));
            setPixel(clr,i,j);
        }
    }
    else if(abs(start_x-end_x)>0){
        if(start_x>end_x){
            if(start_y<end_y)
                bias=false;
            else
                bias=true;
            int temp=start_x;
            start_x=end_x;end_x=temp;
            temp=start_y;
            start_y=end_y;end_y=temp;
        }
        else
            if(start_y>end_y)
                bias=true;
            else
                bias=false;
        for(int i=start_x;i<=end_x;++i){
            float df=((((float)(i-start_x))/((float)(end_x-start_x)))*(float)(end_y-start_y))+(float)start_y;
            int j=(bias?floor(df):ceil(df));
            setPixel(clr,i,j);
        }
    }
    else{
        setPixel(clr,start_x,start_y);
    }
}

inline void maybe_draw_indicator(int i_loc_x,int i_loc_y,uint8_t color,int strength=1){
    if(i_loc_x<0||i_loc_x>=SCREEN_SIZE||i_loc_y<0||i_loc_y>=SCREEN_SIZE){
        if(i_loc_y<0||i_loc_y>=SCREEN_SIZE){
            int sg=(i_loc_y<0?1:-1);
            int st=(i_loc_y<0?0:SCREEN_SIZE_MINUS_1);
            int ci_loc_x=clampI(i_loc_x,0,ARENA_SIZE_X);
            if(ci_loc_x>SCREEN_SIZE){
                setPixel(color,SCREEN_SIZE_MINUS_1  ,st+sg);
                if(strength==1)
                    setPixel(color,SCREEN_SIZE_MINUS_1  ,st+2*sg);
                setPixel(color,SCREEN_SIZE_MINUS_1  ,st);
                setPixel(color,SCREEN_SIZE_MINUS_1-1,st);

                setPixel(color,0  ,st+sg);
                if(strength==1)
                    setPixel(color,0  ,st+2*sg);
                setPixel(color,0  ,st);
                setPixel(color,0+1,st);
            }
            else{
                setPixel(color,ci_loc_x  ,st+sg);
                if(strength==1)
                    setPixel(color,ci_loc_x  ,st+2*sg);
                setPixel(color,ci_loc_x  ,st);
                setPixel(color,ci_loc_x+1,st);
                setPixel(color,ci_loc_x-1,st);
            }
        }
        else{//if (loc_x<0||loc_x>=SCREEN_SIZE){
            //Draw 4 pixels @ each side
            //left
            if(strength==1)
                setPixel(color,2,i_loc_y);
            setPixel(color,1,i_loc_y);
            setPixel(color,0,i_loc_y);
            setPixel(color,0,i_loc_y-1);
            setPixel(color,0,i_loc_y+1);
            //right
            if(strength==1)
                setPixel(color,SCREEN_SIZE_MINUS_1-2,i_loc_y);
            setPixel(color,SCREEN_SIZE_MINUS_1-1,i_loc_y);
            setPixel(color,SCREEN_SIZE_MINUS_1,i_loc_y);
            setPixel(color,SCREEN_SIZE_MINUS_1,i_loc_y-1);
            setPixel(color,SCREEN_SIZE_MINUS_1,i_loc_y+1);
        }
    }
}

inline void draw_faceLine(int i_loc_x, int i_loc_y, float fx, float fy, uint8_t clr1){
    float cfx=-fy;
    float cfy=fx;
    int tipX=moved(i_loc_x,fx*7.0f);
    int tipY=moved(i_loc_y,fy*7.0f);

    int leftX =moved(i_loc_x, cfx*2.9f + fx*6.0f);
    int leftY =moved(i_loc_y, cfy*2.9f + fy*6.0f);
    int rightX=moved(i_loc_x,-cfx*2.9f - fx*6.0f);
    int rightY=moved(i_loc_y,-cfy*2.9f - fy*6.0f);

    drawLine(clr1,tipX,tipY,leftX,leftY);
    drawLine(clr1,tipX,tipY,rightX,rightY);
}

//TODO: redo draw call
inline void draw_ship(float camera_pos_x,float camera_pos_y,float x,float y,float fx,float fy,char colors,bool indicators=true, bool faceLine=true){
    uint8_t clr2=colors&3;
    uint8_t clr1=(colors&12)>>2;
    //normalize(fx,fy);redundant.
    float cfx=-fy;
    float cfy=fx;

    float loc_x=fmodf(x-camera_pos_x,ARENA_SIZE_X);
    float loc_y=y-camera_pos_y;
    int i_loc_x=(int)loc_x;
    int i_loc_y=(int)loc_y;
    if(indicators)
        maybe_draw_indicator(i_loc_x,i_loc_y,clr1);
    if(faceLine)
        draw_faceLine(i_loc_x, i_loc_y, fx, fy, clr1);
    if(!(loc_x<SHIP_OFFSCREEN_MIN||loc_x>SHIP_OFFSCREEN_MAX||loc_y<SHIP_OFFSCREEN_MIN||loc_y>SHIP_OFFSCREEN_MAX)){
        //7x7
        //shape:cap
        //def:vaguely cylindrical
        //direction: towards movement.
        //~3 pixels towards move direction
        //~2 pixels in each movement direction
        int tipX  =moved(i_loc_x, fx *3.9f);
        int tipY  =moved(i_loc_y, fy *3.9f);
        int leftX =moved(i_loc_x, cfx*2.9f);
        int leftY =moved(i_loc_y, cfy*2.9f);
        int rightX=moved(i_loc_x,-cfx*2.9f);
        int rightY=moved(i_loc_y,-cfy*2.9f);
        drawLine(clr1,tipX,tipY,i_loc_x,i_loc_y);
        drawLine(clr1,leftX,leftY,rightX,rightY);
        drawLine(clr1,tipX,tipY,leftX,leftY);
        drawLine(clr1,tipX,tipY,rightX,rightY);
        drawLine(clr1,tipX,tipY,(leftX+i_loc_x)/2,(i_loc_y+leftY)/2);
        drawLine(clr1,tipX,tipY,(rightX+i_loc_x)/2,(i_loc_y+rightY)/2);
        //back-wings
        //shape: triangle-ish
        //direction: back
        //~3 pixels back
        //~1 pixels distance from center
        //~2 pixel width
        int extremityX=moved(moved(i_loc_x,cfx*3.4f),-fx*2.4f);
        int extremityY=moved(moved(i_loc_y,cfy*3.4f),-fy*2.4f);
        tipX=moved(i_loc_x,cfx*2.2f);
        tipY=moved(i_loc_y,cfy*2.2f);
        int backX=moved(tipX,-fx*2.2f);
        int backY=moved(tipY,-fy*2.2f);
        drawLine(clr1,extremityX,extremityY,tipX,tipY);
        drawLine(clr1,extremityX,extremityY,backX,backY);
        drawLine(clr1,tipX,tipY,backX,backY);

        extremityX=moved(moved(i_loc_x,-cfx*3.4f),-fx*2.4f);
        extremityY=moved(moved(i_loc_y,-cfy*3.4f),-fy*2.4f);
        tipX=moved(i_loc_x,-cfx*2.2f);
        tipY=moved(i_loc_y,-cfy*2.2f);
        backX=moved(tipX,-fx*2.2f);
        backY=moved(tipY,-fy*2.2f);
        drawLine(clr1,extremityX,extremityY,tipX,tipY);
        drawLine(clr1,extremityX,extremityY,backX,backY);
        drawLine(clr1,tipX,tipY,backX,backY);
        
        //center AKA the part that takes damage
        //deepest lore: It's usually the color of whoever they're communicating with using the ship's devices, because that's the color they see on their screens.
        setPixel(clr2,i_loc_x,i_loc_y);
    }
}

//TODO: Redo draw call
inline void draw_bullet(float camera_pos_x,float camera_pos_y,float x,float y,float fx,float fy,uint8_t color){
    float loc_x=fmodf(x-camera_pos_x,ARENA_SIZE_X);
    float loc_y=y-camera_pos_y;
    int i_loc_x=(int)loc_x;
    int i_loc_y=(int)loc_y;
    maybe_draw_indicator(i_loc_x,i_loc_y,color,0);
    for(int i=-1;i<=1;++i)
        for(int j=-1;j<=1;++j)
            setPixel(color,i_loc_x+i,i_loc_y+j);
    int trailX=moved(i_loc_x,-fx*3.7f);
    int trailY=moved(i_loc_y,-fy*3.7f);
    drawLine(color,i_loc_x,i_loc_y,trailX,trailY);
}

/*inline void draw_particle(int x,int y,uint8_t color){
    //TODO: REDO?
    if(camY==0.0f){
        if(color&2)
            setPixel(color,x,y);
        else
            switchPixelIfBackground(x,y);
    }
}*/

inline void draw_hearts(int num,bool team){
    uint8_t clr=(team?COLOR_PLAYER:COLOR_ENEMY);
    for(int i=0;i<num;++i){
        int posX=(team?3+i*7:SCREEN_SIZE-11-i*7);
        drawLine(clr,posX+0,10,posX+3,3);
        drawLine(clr,posX+1,10,posX+4,3);
        drawLine(clr,posX+6,10,posX+3,3);
        drawLine(clr,posX+7,10,posX+4,3);
    }
}