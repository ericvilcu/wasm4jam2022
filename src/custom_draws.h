#pragma once
#include "utils.h"
#include "wasm4.h"

int frame=0;
float camX=0.0,camY=0.0;
float TrueCamX=0.0;

void setPixel(uint8_t clr,int x,int y){
    //Too safe?
    //no.
    //bloody inefficient, but we have CPU speed more than anything else.
    if(x>SCREEN_SIZE_MINUS_1 || x<0 || y>SCREEN_SIZE_MINUS_1 || y<0)
        return;
    int offset=((x&3)<<1);
    int idx=SCREEN_SIZE_x_BYTES*y+(x>>2);
    uint8_t initial=FRAMEBUFFER[idx];
    initial^=initial&(3<<offset);
    initial|=(clr)<<offset;
    FRAMEBUFFER[idx]=initial;
}
void switchPixelIfBackground(int x,int y){
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

void drawLine(uint8_t clr,int start_x,int start_y,int end_x,int end_y){
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
//Very bodge.
#include "raseterize.h"
inline void draw_buildings(float TrueCamX){
    //buildings
    int first_building=floor(TrueCamX/60.0f)-1;
    //simplest and probably least efficient way to z-sort.
    float best_z=-9999.0f;
    float last_best_z=9999.0f;
    while(true){
        for(int X=first_building;X<first_building+7;++X){
            int RandZ=(X*67129)%41947;
            float posZ=rand_range_seed(1.0f,1.5f,RandZ);
            if(posZ<last_best_z&&posZ>best_z){
                best_z=posZ;
            }
            else if (last_best_z==posZ){
                float BPOS=60.0f*(float)(X-1);
                float S_BPOS=BPOS-TrueCamX;
                int RandX=(X*41947)%67129;//Random big-ass prime numbers
                int RandXS=(X*71537)%51721;
                int RandZS=(X*51721)%71537;
                float posX=rand_range_seed(-0.1f,0.1f,RandX);
                float scaleX=rand_range_seed(0.3f,0.6f,RandXS);
                float scaleZ=rand_range_seed(0.05f,0.2f,RandZS);
                float start_height=0.1f;//CONSTANT! DO NOT CHANGE!!
                float height=rand_range_seed(-0.2f,-0.9f,RandXS^RandZS);//Close enough.
                float raster_X0=PixToM11X(S_BPOS);
                rasterize_quad({raster_X0+posX       ,height,posZ       },{scaleX,0.0f,0.0f   },{0.0f,start_height-height,0.0f},building_side);
                rasterize_quad({raster_X0+posX       ,height,posZ+scaleZ},{.0f   ,0.0f,-scaleZ},{0.0f,start_height-height,0.0f},building_side);
                rasterize_quad({raster_X0+posX+scaleX,height,posZ       },{.0f   ,0.0f,scaleZ },{0.0f,start_height-height,0.0f},building_side);
            }
        }
        last_best_z=best_z;
        if(best_z==-9999.0f)return;
        best_z=-9999.0f;
    }
}

void draw_background(int timestamp,float camX,float TrueCamX){
    *DRAW_COLORS=2;
    rect(0,HORIZON,SCREEN_SIZE,SCREEN_SIZE-HORIZON);
    int anim_frame=((timestamp)/30)&3;
    int first_wave=((int)-camX)%7;
    int first_wave_2=((int)(-(camX)/1.6f))%7;
    //waves that are a little further
    for(int i=first_wave_2;i<SCREEN_SIZE+7;i+=7)
        switch (anim_frame)
        {
        case 0:case 2:
            drawLine(COLOR_BACKGROUND,i,WAVES_2_Y,i+3,WAVES_2_Y);
            break;
        case 1:case 3:
            drawLine(COLOR_BACKGROUND,i  ,WAVES_2_Y,i+1,WAVES_2_Y-1);
            drawLine(COLOR_BACKGROUND,i+2,WAVES_2_Y-1,i+3,WAVES_2_Y);
            break;
        default:
            break;
        }
    draw_buildings(TrueCamX);
    //WAVES THAT ARE CLOSE TO THE CAMERA
    for(int i=first_wave;i<SCREEN_SIZE+7;i+=7)
        switch (anim_frame)
        {
        case 0:
            drawLine(COLOR_BACKGROUND,i,WAVES_1_Y,i+3,WAVES_1_Y-3);
            setPixel(COLOR_BACKGROUND,i+4,WAVES_1_Y-1);
            setPixel(COLOR_BACKGROUND,i+4,WAVES_1_Y);
            setPixel(COLOR_BACKGROUND,i+5,WAVES_1_Y);
            setPixel(COLOR_BACKGROUND,i+6,WAVES_1_Y-1);
            setPixel(COLOR_BACKGROUND,i+6,WAVES_1_Y-2);
            setPixel(COLOR_BACKGROUND,i+5,WAVES_1_Y-3);
            setPixel(COLOR_BACKGROUND,i+4,WAVES_1_Y-3);
            break;
        case 1:
            setPixel(COLOR_BACKGROUND,i,WAVES_1_Y-1);
            drawLine(COLOR_BACKGROUND,i+1,WAVES_1_Y-1,i+6,WAVES_1_Y-2);
            break;
        case 2:
            drawLine(COLOR_BACKGROUND,i,WAVES_1_Y-2,i+4,WAVES_1_Y-1);
            setPixel(COLOR_BACKGROUND,i+5,WAVES_1_Y);
            setPixel(COLOR_BACKGROUND,i+6,WAVES_1_Y-1);
            break;
        case 3:
            drawLine(COLOR_BACKGROUND,i,WAVES_1_Y-1,i+2,WAVES_1_Y-3);
            drawLine(COLOR_BACKGROUND,i+3,WAVES_1_Y-1,i+3,WAVES_1_Y-3);
            drawLine(COLOR_BACKGROUND,i+4,WAVES_1_Y,i+6,WAVES_1_Y);
            break;
    
        default:
            break;
        }
}
void interpolated_line(uint8_t color,int beginX,int beginY,int dx,int dy,float progress){
    if(progress>0.0f)
        drawLine(color, beginX, beginY,(int) ((float)beginX+(float)dx*min(1.0,progress)), (int) ((float)beginY+(float)dy*min(1.0f,progress)));
}
void draw_background_underwater(int frame,float TrueCamPos,int time){
    //float BPOS=60.0f*(float)(X-1);
    //float S_BPOS=BPOS-TrueCamX;
    int rnd=(frame*71)&31+(int)TrueCamPos;
    float percentage=(float)(INITIAL_WATER_TIME-time)*13.0f/INITIAL_WATER_TIME;
    interpolated_line(COLOR_PLAYER, 10, 80, 20,  0,percentage- 0.0f);
    interpolated_line(COLOR_PLAYER,150, 80,-20,  0,percentage- 1.0f);
    interpolated_line(COLOR_PLAYER,130, 80,-30, 30,percentage- 2.0f);
    interpolated_line(COLOR_PLAYER,100,110,-40,  0,percentage- 3.0f);
    interpolated_line(COLOR_PLAYER, 60,110,-30,-30,percentage- 4.0f);
    interpolated_line(COLOR_PLAYER, 30, 80, 30,-30,percentage- 5.0f);
    interpolated_line(COLOR_PLAYER, 60, 50, 40,  0,percentage- 6.0f);
    interpolated_line(COLOR_PLAYER,100, 50, 30, 30,percentage- 7.0f);
    interpolated_line(COLOR_PLAYER, 70, 80, 10, 20,percentage- 8.0f);
    interpolated_line(COLOR_PLAYER, 80,100, 10,-20,percentage- 9.0f);
    interpolated_line(COLOR_PLAYER, 90, 80,-10,-20,percentage-10.0f);
    interpolated_line(COLOR_PLAYER, 80, 60,-10, 20,percentage-11.0f);
    interpolated_line(COLOR_PLAYER, 80, 70,  0, 20,percentage-12.0f);
}
void maybe_draw_indicator(int i_loc_x,int i_loc_y,uint8_t color,int strength=1){
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

void draw_ship(float camera_pos_x,float camera_pos_y,float x,float y,float fx,float fy,char colors,bool indicators=true){
    uint8_t clr2=colors&3;
    uint8_t clr1=(colors&12)>>2;
    //normalize(fx,fy);redundant.
    float cfx=-fy;
    float cfy=fx;

    float loc_x=fmodf(x-camera_pos_x,ARENA_SIZE_X);
    float loc_y=y-camera_pos_y;
    int i_loc_x=(int)loc_x;
    int i_loc_y=(int)loc_y;
    if(indicators)//wh
        maybe_draw_indicator(i_loc_x,i_loc_y,clr1);
    if(!(loc_x<SHIP_OFFSCREEN_MIN||loc_x>SHIP_OFFSCREEN_MAX||loc_y<SHIP_OFFSCREEN_MIN||loc_y>SHIP_OFFSCREEN_MAX)){
        //7x7
        //shape:cap
        //def:vaguely cylindrical
        //direction: towards movement.
        //~3 pixels towards move direction
        //~2 pixels in each movement direction
        int tipX=moved(i_loc_x,fx*3.9f);
        int tipY=moved(i_loc_y,fy*3.9f);
        int leftX=moved(i_loc_x,cfx*2.9f);
        int leftY=moved(i_loc_y,cfy*2.9f);
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
        setPixel(clr2,i_loc_x,i_loc_y);
        //setPixel(colors>>2);
    }
}

void draw_bullet(float camera_pos_x,float camera_pos_y,float x,float y,float fx,float fy,uint8_t color){
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

void draw_particle(int x,int y,uint8_t color){
    if(camY==0.0f){
        if(color&2)
            setPixel(color,x,y);
        else
            switchPixelIfBackground(x,y);
    }
}

void draw_hearts(int num,bool team){
    uint8_t clr=(team?COLOR_PLAYER:COLOR_ENEMY);
    for(int i=0;i<num;++i){
        int posX=(team?3+i*7:SCREEN_SIZE-11-i*7);
        drawLine(clr,posX+0,10,posX+3,3);
        drawLine(clr,posX+1,10,posX+4,3);
        drawLine(clr,posX+6,10,posX+3,3);
        drawLine(clr,posX+7,10,posX+4,3);
    }
}
void display_score(int x,int y){
    *DRAW_COLORS=COLOR_PLAYER+1;
    char message[20];
    char*ptr=itoa(x,message);
    *ptr='\0';
    int w=(ptr-message);
    text(message,40-4*w,76);

    *DRAW_COLORS=COLOR_ENEMY+1;
    ptr=itoa(y,message);
    *ptr='\0';
    w=(ptr-message);
    text(message,120-4*w,76);
    if(x>y)*DRAW_COLORS=COLOR_PLAYER+1;
    text("-",76,76);
}

void draw_laser(float Y,int size){
    int i_Y=(int)Y;
    for(int y0=i_Y-size;y0<=size+i_Y;++y0)
        if(y0<SCREEN_SIZE && y0>=0)
            drawLine(COLOR_ENEMY,0,y0,SCREEN_SIZE_MINUS_1,y0);
}

void draw_laser_v(float X,int size){
    int i_X=(int)fmodf(X,ARENA_SIZE_X);
    for(int x0=i_X-size;x0<=size+i_X;++x0){
        int tx=mod_I(x0,ARENA_SIZE_X);
        if(tx<SCREEN_SIZE && tx>=0)
            drawLine(COLOR_ENEMY,tx,0,tx,SCREEN_SIZE_MINUS_1);
    }
}

void draw_pre_laser(float Y,int frame){
    unsigned int u_frame=(unsigned int) frame;
    int i_Y=(int)Y;
    if(i_Y<SCREEN_SIZE && i_Y>=0){
        unsigned int sed=(u_frame*u_frame)%6143;
        for(int i=0;i<SCREEN_SIZE;++i){
            sed=(sed*u_frame)%6143;
            if(sed%5==0)
                setPixel(COLOR_ENEMY,i,i_Y);
        }
    }
}
void draw_pre_laser_v(float X,int frame){
    unsigned int u_frame=(unsigned int) frame;
    int i_X=(int)X;
    if(i_X<SCREEN_SIZE && i_X>=0){
        unsigned int sed=(u_frame*u_frame)%6143;
        for(int i=0;i<SCREEN_SIZE;++i){
            sed=(sed*u_frame)%6143;
            if(sed%5==0)
                setPixel(COLOR_ENEMY,i_X,i);
        }
    }
}
