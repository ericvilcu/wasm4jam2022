#pragma once
#include "utils.h"
#include "particles.h"
#include "sound.h"
//Due to my goofy-ass 1-cpp-dozen-header project structure, this will work quite well actually.
constexpr int max_enemy_bullets=20;

struct ENEMY_INIT_LIST_STRUC
{
    unsigned int color;
    uint8_t hearts;
    uint8_t preferred_height;
    uint8_t special;
    uint8_t max_bullets=20;
    uint8_t preference_state_1=0;//steady
    uint8_t preference_state_2=0;//aggressive
    uint8_t preference_state_3=0;//defensive
    uint8_t preference_state_4=0;//dive
    uint8_t preference_state_5=0;//special
    uint8_t songs=0x10;
};
const ENEMY_INIT_LIST_STRUC ENEMY_INIT_LISTS[]={
    //color ,hp,H,spec,bullets, state probabilities
    //{0x999999,2,80 ,4,4,    7*16+1,0,0,0,15},//His name dave
    //RIP dave. you will be missed. (lore accurate)
    //Our ship keeps blowing up
    {0xffff00,2,80 ,0,4,         1,1,1,0,15     ,0x10},
    //diver stats
    {0x11ffff,4,120,1,15,   7*16+0,5,1,5,2      ,0x40},
    //dragon/dagger ship 
    {0xAAAAAA,4,80 ,4,7,    5*16+1,4,1,0,5+16   ,0x23},
    //{0xAAAAAA,4,80 ,4,19,        0,9,1,0,0      ,},
    //The OwO
    {0xffaed0,7,40 ,0,19,   0*16+0,5,1,1,4      ,0x40},
    //Introspection
    {0xFFAA00,2,80 ,2,14+20*12,  5,1,9,0,0      ,0x23},
    //Our ship keeps blowing up 2
    {0xffff00,5,80 ,8+2,19+20*2,17,1,1,0,1      ,0x10},
};

//big array because thou shalt not use malloc/free/new/delete.
float enemy_projectiles[]={
    BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,
    BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,
    BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,
    BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,
    BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,
    BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,
    BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,
    BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,
    BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,
    BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,
    BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,
    BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,
    BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,
    BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,
    BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,
    BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,
    BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,
    BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,
    BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,
    BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,
};

uint8_t projectile_type[]={
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

float player_bullets[]={
    BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,
    BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,
    BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,
    BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,
    BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,
    BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,
};

struct ship{
    float shipX=50.0,shipY=80.0;
    float shipRotX=1.0,shipRotY=0.0;
    float shipVelX=0.0,shipVelY=0.0;
    int player_shot_cooldown=0;
    int iFrames=20;
    int hearts=20;
    float invalid_time=0.0f;
    int dash_cooldown=0;
    int waterTime=300;//~5 seconds
}enemy,player;
//Please don't judge my code too hard. I was a either tired from lack of sleep or delirious from the heat during almost the entirety of this jam.
void ship_die_physics(ship& my_ship,int frame,uint8_t color){
    int rnd1=(frame*148991)%94427;
    int rnd2=(frame*94427)%148991;
    float dx=(rnd1&31)-15;
    float dy=(rnd2&31)-15;
    normalize(dx,dy);
    add_particle(my_ship.shipX,my_ship.shipY,my_ship.shipVelX+dx,my_ship.shipVelY+dy,color>>2,0b00);
    my_ship.shipX=fmodf(my_ship.shipX+my_ship.shipVelX,(float)ARENA_SIZE_X);
    if(!(my_ship.shipY>2.0f*SCREEN_SIZE))my_ship.shipY+=my_ship.shipVelY;
    my_ship.shipVelX=my_ship.shipVelX*falloff;
    my_ship.shipVelY=my_ship.shipVelY*(my_ship.shipVelY>0?1.05f:0.7f)+gravity*4.0f;
}
void ship_physics(ship& my_ship,bool brake_on){
    my_ship.iFrames-=1;
    my_ship.dash_cooldown-=1;
    //Worried it might loop back to a huge number? That is literally going to take 2^31/60 seconds. That's a little over a year, I think...
    my_ship.player_shot_cooldown-=1;
    my_ship.shipVelY+=gravity;
    if(my_ship.shipY>WATER_Y){
        my_ship.invalid_time+=(my_ship.shipY-WATER_Y)/60.0f*0.05f;
        if(my_ship.invalid_time>20.)
            my_ship.shipVelY=min(0.0f,my_ship.shipVelY);
        my_ship.shipVelY-=gravity*10.0f;
    }
    else if(my_ship.shipY<0.0f){
        my_ship.invalid_time-=my_ship.shipY/60.0f*0.05f;
        if(my_ship.invalid_time>20.)
            my_ship.shipVelY=max(0.0f,my_ship.shipVelY);
        my_ship.shipVelY+=gravity*10.0f;
    }
    else
        my_ship.invalid_time=0.0f;
    if(brake_on){
        my_ship.shipVelX*=brake;
        my_ship.shipVelY*=brake;
    }
    my_ship.shipX=fmodf(my_ship.shipX+my_ship.shipVelX,(float)ARENA_SIZE_X);
    my_ship.shipY+=my_ship.shipVelY;
    my_ship.shipVelX=my_ship.shipVelX*falloff-max(0.0f,my_ship.shipVelX-DYNAMIC_FALLOFF_THRESHOLD)*0.5f;
    my_ship.shipVelY=my_ship.shipVelY*falloff-max(0.0f,my_ship.shipVelY-DYNAMIC_FALLOFF_THRESHOLD)*0.5f;
    if(my_ship.shipY>SCREEN_SIZE){
        my_ship.waterTime-=1;
        if(my_ship.waterTime<0&&my_ship.iFrames<0){
            my_ship.waterTime=INITIAL_WATER_TIME;
            my_ship.hearts-=1;
        }
    }
}
void try_hit_enemy(float X,float Y){
    if(enemy.iFrames<=0&&square_wrap_distance(X,Y,enemy.shipX,enemy.shipY)<8.0f){
        enemy.iFrames=20;
        enemy.hearts-=1;
    }
};
void try_hit_enemy_player(float X,float Y){
    if(enemy.iFrames<=0&&square_wrap_distance(X,Y,enemy.shipX,enemy.shipY)<4.0f){
        enemy.iFrames=20;
        enemy.hearts-=1;
    }
};
void fire_default_enemy_projectile_at(float x,float y,float dx,float dy,int max){
    for(int i=0;i<max;++i){
        if(enemy_projectiles[i<<2]==BULLET_INVALID){
            projectile_type[i]=0;
            enemy_projectiles[ i<<2   ]=x;
            enemy_projectiles[(i<<2)|1]=y;
            enemy_projectiles[(i<<2)|2]=dx;
            enemy_projectiles[(i<<2)|3]=dy;
            break;
        }
    }
}
void fire_default_enemy_projectile(int max){
    fire_default_enemy_projectile_at(enemy.shipX,enemy.shipY,enemy.shipRotX*BULLET_START_SPEED+enemy.shipVelX,enemy.shipRotY*BULLET_START_SPEED+enemy.shipVelY,max);
}
//Gotta figure out a way to export this
void try_hit_player(float X,float Y){
    if(player.iFrames<=0&&square_wrap_distance(X,Y,player.shipX,player.shipY)<4.0f){
        player.iFrames=20;
        player.hearts-=1;
    }
};
uint8_t AI_STATE=0;
uint8_t AI_STATE_TIMER=0;
#define AI_STATE_READYING 0
#define AI_STATE_STEADY 1
#define AI_STATE_AGGRESSIVE 2
#define AI_STATE_DEFENSIVE 3
#define AI_STATE_DIVE 4
#define AI_STATE_SPECIAL_ATTACK 5
bool try_dodge(float projX,float projY,float max_dist_squared){
    if(square_distance(projX,projY,enemy.shipX,enemy.shipY)<max_dist_squared)
    {
        //trace("dodged!");
        float dx=best_looped_dir(enemy.shipX,projX);
        float dy=projY-enemy.shipY;
        //find the best direction to dash in
        float dirX=enemy.shipRotX;
        float dirY=enemy.shipRotY;
        float c1=enemy.shipRotX*dx+enemy.shipRotY*dy;//find smallest dot product, go in that direction.
        if(c1>enemy.shipRotY*dx-enemy.shipRotX*dy){
            dirY=-enemy.shipRotX;
            dirX=enemy.shipRotY;
            c1=enemy.shipRotY*dx-enemy.shipRotX*dy;
        }
        if(c1>-enemy.shipRotY*dx+enemy.shipRotX*dy){
            dirY=enemy.shipRotX;
            dirX=-enemy.shipRotY;
        }
        enemy.shipVelX+=dirX*dash_speed;
        enemy.shipVelY+=dirY*dash_speed;
        enemy.dash_cooldown=DASH_COOLDOWN;
        return true;
    };
    return false;
}
/**
 * Takes care of the bullets as well.
 */
void do_ai_tick(int timestamp,int enemy_id){
    bool brake=false,boost=false;
    int mx_bullets=min(max_enemy_bullets,1+ENEMY_INIT_LISTS[enemy_id].max_bullets);
    //tracef("%d %d",AI_STATE,(int)AI_STATE_TIMER);
    switch (AI_STATE)
    {
    case AI_STATE_READYING:{//RESET TO ANOTHER STATE
        int rand=(timestamp*67391)%12347;
        int rand2=(timestamp*12347)%67391;
        int sum=(ENEMY_INIT_LISTS[enemy_id].preference_state_1&15)+
                (ENEMY_INIT_LISTS[enemy_id].preference_state_2&15)+
                (ENEMY_INIT_LISTS[enemy_id].preference_state_3&15)+
                (ENEMY_INIT_LISTS[enemy_id].preference_state_4&15)+
                (ENEMY_INIT_LISTS[enemy_id].preference_state_5&15);
        rand=(rand*sum)/12347;
        //a for loop would have been better...
        if(rand<=(ENEMY_INIT_LISTS[enemy_id].preference_state_1&15)){
            AI_STATE=AI_STATE_STEADY;
            break;
        }
        rand-=ENEMY_INIT_LISTS[enemy_id].preference_state_1&15;
        if(rand<=(ENEMY_INIT_LISTS[enemy_id].preference_state_2&15)){
            AI_STATE=AI_STATE_AGGRESSIVE;
            AI_STATE_TIMER=60*(3+(rand2&7));
            break;
        }
        rand-=ENEMY_INIT_LISTS[enemy_id].preference_state_2&15;
        if(rand<=(ENEMY_INIT_LISTS[enemy_id].preference_state_3&15)){
            AI_STATE=AI_STATE_DEFENSIVE;
            AI_STATE_TIMER=60*(3+(rand2&7));
            break;
        }
        rand-=ENEMY_INIT_LISTS[enemy_id].preference_state_3&15;
        if(rand<=(ENEMY_INIT_LISTS[enemy_id].preference_state_4&15)){
            AI_STATE_TIMER=rand2&1;
            AI_STATE=AI_STATE_DIVE;
            break;
        }
        rand-=ENEMY_INIT_LISTS[enemy_id].preference_state_4&15;
        if(rand<=(ENEMY_INIT_LISTS[enemy_id].preference_state_5&15)){
            AI_STATE_TIMER=255;
            AI_STATE=AI_STATE_SPECIAL_ATTACK;
            break;
        }
        break;
    }
    case AI_STATE_STEADY:{//Steady. Only 1 shot afterwards.
        float dist=ENEMY_INIT_LISTS[enemy_id].preferred_height-enemy.shipY;
        float diffX=float((ENEMY_INIT_LISTS[enemy_id].preference_state_1>>4)*(-1+2*(ENEMY_INIT_LISTS[enemy_id].preference_state_5>15)));
        float abs_dist=abs(dist);
        if(abs_dist>10.0f*enemy.shipVelY){
            rot_towards(enemy.shipRotX,enemy.shipRotY,diffX,dist);
            boost=true;
        }
        //See if done.
        if(abs_dist<0.5){
            fire_default_enemy_projectile(mx_bullets);
            AI_STATE=AI_STATE_READYING;
        }
        break;
    }
    case AI_STATE_AGGRESSIVE:{//Shoot every 40 frames. Rotate towards player. always boost.
        AI_STATE_TIMER-=1;
        if(AI_STATE_TIMER%40==0)
            fire_default_enemy_projectile(mx_bullets);
        //tracef("D");
        //tracef("%f %f %f %f",enemy.shipRotX,enemy.shipRotY,best_looped_dir(enemy.shipX,player.shipX),player.shipY-enemy.shipY);
        rot_towards(enemy.shipRotX,enemy.shipRotY,best_looped_dir(enemy.shipX,player.shipX),player.shipY-enemy.shipY);
        //boost
        boost=true;
        if(AI_STATE_TIMER<=0)
            AI_STATE=AI_STATE_READYING;
        break;
    }
    case AI_STATE_DEFENSIVE:{//This is a 2-part. state: 1: speed. 2 potentially attack
        AI_STATE_TIMER-=1;
        if((AI_STATE_TIMER%90)>60){
            brake=true;
            if(AI_STATE_TIMER%30==5)
                fire_default_enemy_projectile(mx_bullets);
            rot_towards(enemy.shipRotX,enemy.shipRotY,best_looped_dir(enemy.shipX,player.shipX),player.shipY-enemy.shipY);
        }
        else{
            rot_towards(enemy.shipRotX,enemy.shipRotY,2.0f,ENEMY_INIT_LISTS[enemy_id].preferred_height-enemy.shipY);
        }
        boost=true;
        if(AI_STATE_TIMER<=0)
            AI_STATE=AI_STATE_READYING;
        break;
    }
    case AI_STATE_DIVE:{//Just slowly rotate down and dive into the abyss
        rot_towards(enemy.shipRotX,enemy.shipRotY,-1+2*(AI_STATE_TIMER&1),1);
        boost=true;
        if(enemy.shipY>SCREEN_SIZE)AI_STATE=AI_STATE_STEADY;
        break;
    }
    case AI_STATE_SPECIAL_ATTACK:{//Special attacks :>
        //tracef("SPECIAL TIMER:%d;atk=%d",(int)AI_STATE_TIMER,(int)(ENEMY_INIT_LISTS[enemy_id].special>>2));
        switch (ENEMY_INIT_LISTS[enemy_id].special>>2){
            //tracef("%d",(int)AI_STATE);
            case 1:{
                AI_STATE_TIMER-=1;
                int ast=AI_STATE_TIMER&63;

                if(ast>=52){
                    float tx=best_looped_dir(enemy.shipX,player.shipX);
                    float ty=player.shipY-enemy.shipY;
                    if(abs(tx)+abs(ty)>0.3){//To protect against the impossible odds of the two ships overlapping (or, almost overlapping).
                        enemy.shipRotX=tx;
                        enemy.shipRotY=ty;
                        normalize_squareify(enemy.shipRotX,enemy.shipRotY);
                    }
                    if(ast<=53)
                    {
                        enemy.shipVelX+=enemy.shipRotX*dash_speed;
                        enemy.shipVelY+=enemy.shipRotY*dash_speed;
                    }
                    else {
                        brake=true;
                        boost=true;
                    }
                }
                else{
                    boost=true;brake=false;
                    //Didn't look good :(
                    /*if((AI_STATE_TIMER&3)==0){
                        float VX=3.0f*((AI_STATE_TIMER&4)?-enemy.shipRotY:enemy.shipRotY),
                            VY=3.0f*((AI_STATE_TIMER&4)?enemy.shipRotX:-enemy.shipRotX);
                        add_particle(enemy.shipX,enemy.shipY,VX,VY,COLOR_ENEMY,0b01);
                    }*/
                    try_hit_player(enemy.shipX,enemy.shipY);
                }
                if(AI_STATE_TIMER==0)
                    AI_STATE=AI_STATE_READYING;
                break;
            }
            case 2:
                if(enemy.hearts<=4){
                    if(AI_STATE_TIMER==255){
                        if(enemy.hearts<=2){
                            enemy.shipRotX=0;
                            enemy.shipRotY=1;
                        }
                        else{
                            enemy.shipRotX=(float)(1-2*(frame&1));
                            enemy.shipRotY=0;
                        }
                    }
                    brake=true;
                    bool v=(enemy.shipRotX>=0);
                    bool h=(enemy.shipRotX<=0);
                    AI_STATE_TIMER-=1;
                    if(AI_STATE_TIMER<90){
                        int laser_size=min(10,45-abs((int)AI_STATE_TIMER-45));
                        if(v)
                            draw_laser_v(enemy.shipX-camX,laser_size);
                        if(h)
                            draw_laser(enemy.shipY-camY,laser_size);
                        if(laser_size>=0){
                            if((v&&abs(player.shipX-enemy.shipX)+1.0f<0.9f*(float)laser_size)
                            ||(h&&abs(player.shipY-enemy.shipY)+1.0f<0.9f*(float)laser_size)
                            )
                                player.hearts=0;//insta-kill may be a little much.
                        }
                        if(AI_STATE_TIMER==0){
                            AI_STATE=AI_STATE_AGGRESSIVE;
                            AI_STATE_TIMER=120;
                        }
                    }
                    else{
                        if(v)
                            draw_pre_laser_v(fmodf(enemy.shipX-camX,ARENA_SIZE_X),frame);
                        if(h)
                            draw_pre_laser(enemy.shipY,frame);
                    }
                }
                else AI_STATE=AI_STATE_READYING;
                //tracef("%d",(int)AI_STATE);
                break; 
            case 0:
            default://special attack 0: do nothing.
                AI_STATE_TIMER-=40;


                if(AI_STATE_TIMER<=40)
                    AI_STATE=AI_STATE_READYING;
                break;
        }
        break;
    }
    default:
        AI_STATE=AI_STATE_READYING;
        break;
    }
    if(ENEMY_INIT_LISTS[enemy_id].special&1)
        //Special behaviour 1: dive bullets
        if(enemy.shipY>SCREEN_SIZE+1 && frame%20==0)
            fire_default_enemy_projectile_at(enemy.shipX,SCREEN_SIZE-1,enemy.shipVelX,-BULLET_START_SPEED/1.2f,mx_bullets);

    if(ENEMY_INIT_LISTS[enemy_id].special&2)if(enemy.dash_cooldown<0){
        float dist=pow2(3+ENEMY_INIT_LISTS[enemy_id].max_bullets/20);//I gotta use more of those empty bits...
        
        //Special behaviour 2: dodge (badly)
        if(!try_dodge(player.shipX,player.shipY,dist*2.0f))
            for(int b=0;b<=player_bullet_count;++b)
                if(player_bullets[b<<2]!=BULLET_INVALID)
                    if(try_dodge(player_bullets[b<<2],player_bullets[(b<<2)|1],dist))
                        break;
    }
    //Finally, ship physics.
    if(boost){
        enemy.shipVelX+=enemy.shipRotX*speed;
        enemy.shipVelY+=enemy.shipRotY*speed;
    }
    ship_physics(enemy,brake);
    //and processing bullets
}
void handle_enemy_bullets(int enemy_id){
    for(int i=0;i<min(max_enemy_bullets,1+ENEMY_INIT_LISTS[enemy_id].max_bullets);++i){
        if(enemy_projectiles[i<<2]!=BULLET_INVALID){
            float X=enemy_projectiles[i<<2];
            float Y=enemy_projectiles[(i<<2)|1];
            if(Y<0.0f||Y>fSCREEN_SIZE){
                enemy_projectiles[i<<2]=BULLET_INVALID;
                return;
            }
            float dX=enemy_projectiles[(i<<2)|2];
            float dY=enemy_projectiles[(i<<2)|3];
            draw_bullet(camX,camY,X,Y,dX,dY,COLOR_ENEMY);
            try_hit_player(X,Y);
            enemy_projectiles[i<<2]=fmodf(X+dX,ARENA_SIZE_X);
            enemy_projectiles[(i<<2)|1]+=dY;
            enemy_projectiles[(i<<2)|2]=dX*falloff;
            enemy_projectiles[(i<<2)|3]=dY*falloff+gravity;
        }
    }
}

