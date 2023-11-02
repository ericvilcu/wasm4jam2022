#pragma once
#include "custom_draws.h"
constexpr int max_particles=256;
int current_particle=0;
int active_particles=0;
constexpr uint8_t INVALID_PARTICLE=0b11111111;
//17 bits x/y position each, (fixed-point float, 8 bits int, 9 after)
//13 bits x/y velocity each, (fixed-point float, 1 sign bit,3 bits  int,9 bits after)
//60 total bits.
//2 bits behaviour
//2 bits for color
//64 bits total.
//0 bits free.
//in that order.
//constexpr uint64_t INVALID_PARTICLE=0xffffffffffffffff;//that's a lot of f's
//Well. I tried something with bit shifting but failed. Now we have this awkward data type that can't even be aligned properly.

struct particle
{
    unsigned int x;
    unsigned int y;
    int dx;
    int dy;
    uint8_t col_type=INVALID_PARTICLE;
};

particle particles[max_particles];
void clear_particles(){
    for(int p=0;p<max_particles;++p){
        particles[p].col_type=INVALID_PARTICLE;
    }
    active_particles=0;
}
void add_particle(float x,float y,float dx,float dy,uint8_t color,uint8_t type){
    if(y>2*SCREEN_SIZE||x<0.||y<0.)return;
    //This is the one place I'm concerned about performance...
    //That's a lot of particles, and a lot of operations on shitty data types.
    if(active_particles<1/*max_particles*/)//the ghost particle is for this line to not have a -1. am i stupid?
                                      //wait does it even help? I'm not touching it, but still...
        for(int i=current_particle+1;i!=current_particle;i=(i+1)%max_particles)
            if(particles[i].col_type==INVALID_PARTICLE){
                active_particles+1;
                particles[i].x=(unsigned int)(x*256.0f);
                particles[i].y=(unsigned int)(y*256.0f);
                particles[i].dx=(int)(dx*256.0f);
                particles[i].dy=(int)(dy*256.0f);
                particles[i].col_type=(uint8_t)(type<<2)|color;
                break;
            }
        
}
inline unsigned int sumUI(unsigned int x,int y){
    if(y<0)return x-(unsigned int)-y;
    else return x+(unsigned int)y;
}
inline unsigned int sumUIM(unsigned int x,int y,unsigned int mx){
    if(y<0){
        if(x<(unsigned int)-y)
            return mx-(unsigned int)-((int)x+y);
        return (x-(unsigned int)-y)%mx;
    } else 
        return (x+(unsigned int)y)%mx;
}
void process_particles(float camX,float camY){
    int icX=(int)camX;
    int icY=(int)camY;
    for(int i=0;i<max_particles;++i)if(particles[i].col_type!=INVALID_PARTICLE){
        unsigned int x=particles[i].x;
        unsigned int y=particles[i].y;
        int dx=particles[i].dx;
        int dy=particles[i].dy;
        uint8_t color=3&particles[i].col_type;
        uint8_t type=particles[i].col_type>>2;
        switch(type){
            case 0:{
                x=sumUIM(x,dx,ARENA_SIZE_X*256);
                y=sumUI(y,dy);
                dx=(int)((float)dx*falloff);
                dy=(int)((float)dy*falloff+gravity*1024);
                int trueX=mod_I((int)(x>>8)-icX,ARENA_SIZE_X);
                int trueY=((int)(y>>8)+icY);
                draw_particle(trueX,trueY,color);
                if(trueY<0 || trueY>SCREEN_SIZE || trueX>SCREEN_SIZE){
                    particles[i].col_type=INVALID_PARTICLE;
                }
                else{
                    //I wonder if there's a nice optimisation for these 4 lines.
                    particles[i].x=x;
                    particles[i].y=y;
                    particles[i].dx=dx;
                    particles[i].dy=dy;
                }
                break;
            }
            case 1:{
                x=sumUIM(x,dx,ARENA_SIZE_X*256);
                y=sumUI(y,dy);
                dx=(int)((float)dx);
                dy=(int)((float)dy+5*gravity*1024);
                int trueX=mod_I((int)(x>>8)-icX,ARENA_SIZE_X);
                int trueY=((int)(y>>8)+icY);
                draw_particle(trueX,trueY,color);
                if(trueY<0 || trueY>SCREEN_SIZE || trueX>SCREEN_SIZE){
                    particles[i].col_type=INVALID_PARTICLE;
                }
                else{
                    //I wonder if there's a nice optimisation for these 4 lines.
                    particles[i].x=x;
                    particles[i].y=y;
                    particles[i].dx=dx;
                    particles[i].dy=dy;
                }
                break;
            }
            case 2:particles[i].col_type=INVALID_PARTICLE;
                break;
            case 3:particles[i].col_type=INVALID_PARTICLE;
                break;
        }
    }
}
int part_rand_val=0;//Too lazy to do better.

void particle_rand_burst(uint8_t clr,float iX,float iY,int count=25,float speed_multiplier=3.0f){
    while(count--){
        int rand=(abs((part_rand_val)*148991)%94427)&1023;
        int rand2=(abs((part_rand_val++)*94427)%148991)&1023;
        float spdX=speed_multiplier*(1.0f-2.0f*(float)rand/1023.0f);
        float spdY=speed_multiplier*(1.0f-2.0f*(float)rand2/1023.0f);
        add_particle(iX,iY,spdX,spdY,clr,1);
    }
}