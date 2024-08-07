#pragma once
#include "utils.h"
#include "wasm4.h"
#include "position.h"
#include "random.h"
#include "env.h"
namespace custom_draws
{
    inline void switchPixel(int x,int y){
        if(x>SCREEN_SIZE_MINUS_1 || x<0 || y>SCREEN_SIZE_MINUS_1 || y<0)
            return;
        int offset=((x&3)<<1);
        int idx=SCREEN_SIZE_x_BYTES*y+(x>>2);
        uint8_t initial=FRAMEBUFFER[idx];
        uint8_t swapped=initial^(uint8_t)(1<<offset);
        FRAMEBUFFER[idx]=swapped;
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
                int ci_loc_x=clampI(i_loc_x,0,ARENA_SIZE);
                if(i_loc_x<0||i_loc_x>=SCREEN_SIZE){
                    setPixel(color,SCREEN_SIZE_MINUS_1,SCREEN_SIZE_MINUS_1);
                    setPixel(color,0,SCREEN_SIZE_MINUS_1);
                    setPixel(color,SCREEN_SIZE_MINUS_1,0);
                    setPixel(color,0,0);
                    if(strength==1){
                        setPixel(color,SCREEN_SIZE_MINUS_1-1,SCREEN_SIZE_MINUS_1-1);
                        setPixel(color,1,SCREEN_SIZE_MINUS_1-1);
                        setPixel(color,SCREEN_SIZE_MINUS_1-1,1);
                        setPixel(color,1,1);
                    }
                }
                else{
                    setPixel(color,ci_loc_x  ,SCREEN_SIZE_MINUS_1);
                    if(strength==1)
                        setPixel(color,ci_loc_x  ,SCREEN_SIZE_MINUS_1-2);
                    if(strength>=0){
                        setPixel(color,ci_loc_x  ,SCREEN_SIZE_MINUS_1-1);
                        setPixel(color,ci_loc_x+1,SCREEN_SIZE_MINUS_1);
                        setPixel(color,ci_loc_x-1,SCREEN_SIZE_MINUS_1);
                    }
                    setPixel(color,ci_loc_x  ,0);
                    if(strength==1)
                        setPixel(color,ci_loc_x  ,2);
                    if(strength>=0){
                        setPixel(color,ci_loc_x  ,1);
                        setPixel(color,ci_loc_x+1,0);
                        setPixel(color,ci_loc_x-1,0);
                    }
                }
            }
            else{//if (loc_x<0||loc_x>=SCREEN_SIZE){
                //Draw 4 pixels @ each side
                //left
                if(strength==1)
                    setPixel(color,2,i_loc_y);
                setPixel(color,0,i_loc_y);
                if(strength>=0){
                    setPixel(color,0,i_loc_y-1);
                    setPixel(color,0,i_loc_y+1);
                    setPixel(color,1,i_loc_y);
                }
                //right
                if(strength==1)
                    setPixel(color,SCREEN_SIZE_MINUS_1-2,i_loc_y);
                setPixel(color,SCREEN_SIZE_MINUS_1,i_loc_y);
                if(strength>=0){
                    setPixel(color,SCREEN_SIZE_MINUS_1-1,i_loc_y);
                    setPixel(color,SCREEN_SIZE_MINUS_1,i_loc_y-1);
                    setPixel(color,SCREEN_SIZE_MINUS_1,i_loc_y+1);
                }
            }
        }
    }

    inline void draw_faceLine(int i_loc_x, int i_loc_y, float fx, float fy, uint8_t clr1){
        float cfx=-fy;
        float cfy=fx;
        int tipX=moved(i_loc_x,fx*10.0f);
        int tipY=moved(i_loc_y,fy*10.0f);

        int leftX =moved(i_loc_x, cfx*1.3f + fx*8.0f);
        int leftY =moved(i_loc_y, cfy*1.3f + fy*8.0f);
        int rightX=moved(i_loc_x,-cfx*1.3f + fx*8.0f);
        int rightY=moved(i_loc_y,-cfy*1.3f + fy*8.0f);

        drawLine(clr1,tipX,tipY,leftX,leftY);
        drawLine(clr1,tipX,tipY,rightX,rightY);
    }


    inline void draw_ship(int i_loc_x, int i_loc_y, float fx, float fy, char colors, bool center=true, bool indicators=true, bool faceLine=true){
        uint8_t clr2=colors&3;
        uint8_t clr1=(colors&12)>>2;
        //normalize(fx,fy);redundant.
        float cfx=-fy;
        float cfy=fx;

        if(indicators)
            maybe_draw_indicator(i_loc_x,i_loc_y,clr1);
        if(faceLine)
            draw_faceLine(i_loc_x, i_loc_y, fx, fy, clr1);
        if(!(i_loc_x<SHIP_OFFSCREEN_MIN||i_loc_x>SHIP_OFFSCREEN_MAX||i_loc_y<SHIP_OFFSCREEN_MIN||i_loc_y>SHIP_OFFSCREEN_MAX)){
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
            if(center)setPixel(clr2,i_loc_x,i_loc_y);
        }
    }

    
    inline void drawLineDbl(uint8_t clr,int start_x,int start_y,int end_x,int end_y){
        drawLine(clr,start_x,start_y,end_x,end_y);
        drawLine(clr,end_x,end_y,start_x,start_y);
    }

    inline void draw_laser(int i_loc_x,int i_loc_y, float fx,float fy, uint8_t color, float power=0.0f, bool active=false){
        maybe_draw_indicator(i_loc_x,i_loc_y,color,0);
        float cfx=-fy;
        constexpr float thickness = 7.0f;
        constexpr float thickness_radar = 0.4f;
        constexpr int length = 100;
        constexpr int tip_length = 25;
        float cfy=fx;
        if(active)
        {
            int llx=moved(i_loc_x,fx*(float)0.0f/2.0f),lly=moved(i_loc_y,fy*(float)0.0f/2.0f);
            float q=power;
            for(int i=0;i<length;++i){
                if(i>length-tip_length)q=power*(float)(length-i)/(float)tip_length;
                int FX=moved(i_loc_x,fx*(float)i);
                int FY=moved(i_loc_y,fy*(float)i);
                //TODO: FX-RX and others are almost always the same... do something about it?
                int RX=moved(FX, cfx*q*thickness);
                int RY=moved(FY, cfy*q*thickness);
                int LX=moved(FX,-cfx*q*thickness);
                int LY=moved(FY,-cfy*q*thickness);
                drawLineDbl(color,RX,RY,LX,LY);
                if(llx!=FX&&lly!=FY){
                    RX=moved(FX , cfx*q*thickness);
                    RY=moved(lly, cfy*q*thickness);
                    LX=moved(FX ,-cfx*q*thickness);
                    LY=moved(lly,-cfy*q*thickness);
                    drawLineDbl(color,RX,RY,LX,LY);
                    RX=moved(llx, cfx*q*thickness);
                    RY=moved(FY , cfy*q*thickness);
                    LX=moved(llx,-cfx*q*thickness);
                    LY=moved(FY ,-cfy*q*thickness);
                    drawLineDbl(color,RX,RY,LX,LY);
                }
                llx=FX;lly=FY;
            }
        }
        else
        {
            float q=power*0.8f;
            float p=(1.0f-power)*thickness_radar;
            for(int i=0;i<(int)(q*(float)(length-(int)(tip_length/3))/3.0f);++i){
                int FX=moved(i_loc_x,fx*(float)i*3);
                int FY=moved(i_loc_y,fy*(float)i*3);

                int RX=moved(FX, cfx*((float)i*p+q)*thickness);
                int RY=moved(FY, cfy*((float)i*p+q)*thickness);
                int LX=moved(FX,-cfx*((float)i*p+q)*thickness);
                int LY=moved(FY,-cfy*((float)i*p+q)*thickness);

                drawLine(color,RX,RY,LX,LY);
            }
        }
    }

    inline void draw_bullet(int i_loc_x,int i_loc_y, float fx,float fy, uint8_t color){
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
        uint8_t clr=(team?COLOR_ENEMY:COLOR_PLAYER);
        for(int i=0;i<num;++i){
            int posX=(team?3+i*7:SCREEN_SIZE-11-i*7);
            drawLine(clr,posX+1,10,posX+4,3);
            drawLine(clr,posX+6,10,posX+3,3);
            drawLine(clr,posX+0,10,posX+2,6);
            drawLine(clr,posX+7,10,posX+5,6);
        }
    }

    inline void boxContour(int i_loc_x,int i_loc_y, uint8_t radius, uint8_t clr){
        drawLine(clr,i_loc_x-radius,i_loc_y-radius,i_loc_x-radius,i_loc_y+radius);
        drawLine(clr,i_loc_x-radius,i_loc_y+radius,i_loc_x+radius,i_loc_y+radius);
        drawLine(clr,i_loc_x+radius,i_loc_y+radius,i_loc_x+radius,i_loc_y-radius);
        drawLine(clr,i_loc_x+radius,i_loc_y-radius,i_loc_x-radius,i_loc_y-radius);
    }

    inline void draw_ability(int i_loc_x,int i_loc_y, int type){
        boxContour(i_loc_x,i_loc_y,5,COLOR_BACKGROUND_ACCENT);
        boxContour(i_loc_x,i_loc_y,4,COLOR_BACKGROUND);
        boxContour(i_loc_x,i_loc_y,3,COLOR_BACKGROUND_ACCENT);
        boxContour(i_loc_x,i_loc_y,2,COLOR_BACKGROUND);
        boxContour(i_loc_x,i_loc_y,1,COLOR_BACKGROUND_ACCENT);
        setPixel(COLOR_BACKGROUND,i_loc_x,i_loc_y);
        if(type==0){
            boxContour(i_loc_x,i_loc_y,4,COLOR_PLAYER);
            boxContour(i_loc_x-2,i_loc_y-2,1,COLOR_ENEMY);
            boxContour(i_loc_x-1,i_loc_y+3,1,COLOR_ENEMY);
            boxContour(i_loc_x+3,i_loc_y,1,COLOR_ENEMY);
        }
        else if(type==1){
            boxContour(i_loc_x,i_loc_y,4,COLOR_ENEMY);
            drawLine(COLOR_PLAYER,i_loc_x-3,i_loc_y-4,i_loc_x+3,i_loc_y+4);
            drawLine(COLOR_PLAYER,i_loc_x-4,i_loc_y-4,i_loc_x+2,i_loc_y+4);
            drawLine(COLOR_PLAYER,i_loc_x-2,i_loc_y-4,i_loc_x+4,i_loc_y+4);
        }
        else if(type==2){
            boxContour(i_loc_x,i_loc_y,4,COLOR_ENEMY);
            drawLine(COLOR_PLAYER,i_loc_x+0,i_loc_y  ,i_loc_x+1,i_loc_y+4);
            drawLine(COLOR_PLAYER,i_loc_x-2,i_loc_y+4,i_loc_x-2,i_loc_y);
            drawLine(COLOR_PLAYER,i_loc_x+3,i_loc_y+4,i_loc_x+3,i_loc_y);
            drawLine(COLOR_PLAYER,i_loc_x  ,i_loc_y-4,i_loc_x+0,i_loc_y);
            drawLine(COLOR_PLAYER,i_loc_x  ,i_loc_y-4,i_loc_x-2,i_loc_y);
            drawLine(COLOR_PLAYER,i_loc_x  ,i_loc_y-4,i_loc_x+3,i_loc_y);
        }
        else if(type==3){
            boxContour(i_loc_x,i_loc_y,4,COLOR_ENEMY);
            boxContour(i_loc_x,i_loc_y,3,COLOR_PLAYER);
            boxContour(i_loc_x,i_loc_y,2,COLOR_ENEMY);
            boxContour(i_loc_x,i_loc_y,1,COLOR_PLAYER);
        }

        maybe_draw_indicator(i_loc_x,i_loc_y,COLOR_PLAYER,1);
        maybe_draw_indicator(i_loc_x,i_loc_y,COLOR_BACKGROUND_ACCENT,0);
        maybe_draw_indicator(i_loc_x,i_loc_y,COLOR_ENEMY,-1);
    }

#if ALLOW_EFFECTS
    inline void processEffect(){
        int screen_byte_length = SCREEN_SIZE >>2;
        int mode = ::effect_type>>24;
        int time = ::effect_type & 0b111111111111111111111111;
        int shift=0;
        int fake_time = time;
        switch (mode){
            case 3:
            custom_draws::drawLine(1,0,0,160,160);
            custom_draws::drawLine(1,160,0,0,160);
        }
        //for(int i=min(160,time)-1;i>=0;--i){
        for(int i=0;i<min(160,time);i++){
            //tracef("%f",(float)i);
            int bop = (fake_time & 0b110101)^0b000101;
            bool bpp = 0==(1&(bop ^ (bop>>2)^ (bop>>4)^ (bop>>5)));
            
            switch (mode)
            {
                case 0:{
                    shift=10-(int)abs(((i-time)%20+20)%20-10);
                    break;
                }
                case 1:{
                    shift=(random::randomInt()&1)-(random::randomInt()&1);
                    break;
                }
                case 2:{
                    shift=60*((i+time)%20<=10?-1:1);
                    break;
                }
                case 3:{
                    shift=0;
                    if(i%5==0)
                        shift=random::randomInt();
                    break;
                }
                case 4:{
                    shift=10-(int)abs(((i-time)%20+20)%20-10);
                    if(i%2)shift+=(random::randomInt()&1)-(random::randomInt()&1);
                    if(i%20==0)
                        shift=random::randomInt();
                    break;
                }
                case 5:{
                    int crash=(((time+i)*107)>>4)+(time+i)*7;
                    shift=10-(int)abs(((crash)%20+20)%20-10);
                    break;
                }
                case 6:{
                    shift=(time*(i/10))/2;
                    break;
                }
                case 7:{
                    shift=0;
                    if(time<80)time=80;
                    if(i>80)shift=80;
                    break;
                }
                case 8:{
                    shift=0;
                    if(i%3==0)
                        shift+=(random::randomInt()&3)-(random::randomInt()&3);
                    else if(i%2)shift+=(random::randomInt()&1)-(random::randomInt()&1);
                    break;
                }
                default:break;
            }
            fake_time-=1;
            int motion = shift%160;//>>2;
            if(motion<0)motion+=160;
            int sub_motion = motion&3;
            motion>>=2;
            int y_idx = i*screen_byte_length;
            //tracef("%f",(float)motion);
            /*if(sub_motion==0 && motion != 0){
                int target0 = (motion+screen_byte_length)%screen_byte_length;
                //160=32*5. split this into factors.
                int gcd = 1;
                {
                    int sht=motion;
                    while((sht&1) == 0){
                        sht>>=1;
                        gcd<<=1;
                    }
                    if(sht%5 == 0)gcd*=5;
                }
                if(gcd<screen_byte_length)
                for(int ji=0;ji<gcd;++ji){    
                    //tracef("%f %f",(float)y_idx,(float)ji);
                    int target = target0 +ji;
                    if(target>screen_byte_length) target-=screen_byte_length;
                    auto carry = FRAMEBUFFER[target];
                    int j=ji;
                    do{
                        target+=motion;
                        j+=motion;
                        if(target>=screen_byte_length) target-=screen_byte_length;
                        if(j>=screen_byte_length) j-=screen_byte_length;
                        FRAMEBUFFER[y_idx + j]=FRAMEBUFFER[y_idx+target];
                        //tracef("%f %f %f %f",(float)y_idx,(float)j,(float)target,(float)ji);
                    }while(target!=ji);
                    FRAMEBUFFER[y_idx+ji]=carry;
                }
            }
            else*/
            {
                //Slow as f?
                uint8_t temp[screen_byte_length];
                for(int j=0;j<screen_byte_length;++j)
                    temp[j]=FRAMEBUFFER[y_idx+j];
                int shift1=sub_motion*2;
                int shift2=8-shift1;
                int src0,src1;
                //tracef("%f %f",(float)sub_motion,(float)motion);
                motion = screen_byte_length-motion;
                for(int j=0;j<screen_byte_length;++j){
                    FRAMEBUFFER[y_idx+j]=
                        (uint8_t)((uint8_t)temp[(j+motion+0)%screen_byte_length]<<(uint8_t)shift1)
                        |
                        (uint8_t)((uint8_t)temp[(j+motion+screen_byte_length-1)%screen_byte_length]>>(uint8_t)shift2)
                        ;
                }
            }
        }
        //trace("exit");
        if(0==(frame&7)) time += 1;
        ::effect_type = (mode<<24) | (time);
    }
#endif
} // namespace custom_draws