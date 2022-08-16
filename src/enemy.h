#pragma once
#include "utils.h"
//Due to my goofy-ass 1-cpp-dozen-header project structure, this will work quite well actually.
int max_enemy_bullets=20;



constexpr uint8_t mx_bullets=20;
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
};
const ENEMY_INIT_LIST_STRUC ENEMY_INIT_LISTS[]={
    //{0x999999},//His name dave
    //RIP dave. you will be missed. (lore accurate)
    //Our ship keeps blowing up
    {0xffff00,2,80 ,0,4,        1,1,1,0,15},
    //diver
    {0x11ffff,4,120,1,15,       0,5,1,5,2},
    //The OwO
    {0xffaed0,7,40 ,0,19,       0,5,1,1,4},
    //Introspection
    {0xFFAA00,2,80 ,2,14+20*12, 5,1,9,0,0},
    //Our ship keeps blowing up 2
    {0xffff00,5,80 ,2,19+20*2,  1,1,1,0,0},
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
    if(brake_on){
        my_ship.shipVelX*=brake;
        my_ship.shipVelY*=brake;
    }
    my_ship.shipX=fmodf(my_ship.shipX+my_ship.shipVelX,(float)ARENA_SIZE_X);
    my_ship.shipY+=my_ship.shipVelY;
    my_ship.shipVelX=my_ship.shipVelX*falloff*pow2(min(1.0f,1.0f/max(1.0,my_ship.shipVelX-DYNAMIC_FALLOFF_THRESHOLD)));
    my_ship.shipVelY=my_ship.shipVelY*falloff*pow2(min(1.0f,1.0f/max(1.0,my_ship.shipVelY-DYNAMIC_FALLOFF_THRESHOLD)));
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
void fire_default_enemy_projectile_at(float x,float y,float dx,float dy){
    for(int i=0;i<(int)(sizeof(projectile_type)/sizeof(uint8_t));++i){
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
void fire_default_enemy_projectile(){
    fire_default_enemy_projectile_at(enemy.shipX,enemy.shipY,enemy.shipRotX*BULLET_START_SPEED+enemy.shipVelX,enemy.shipRotY*BULLET_START_SPEED+enemy.shipVelY);
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
/**
 * Takes care of the bullets as well.
 */
void do_ai_tick(int timestamp,int enemy_id){
    bool brake=false,boost=false;
    //tracef("%d",AI_STATE);
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
            AI_STATE=AI_STATE_DIVE;
            break;
        }
        rand-=ENEMY_INIT_LISTS[enemy_id].preference_state_4&15;
        if(rand<=(ENEMY_INIT_LISTS[enemy_id].preference_state_5&15)){
            AI_STATE_TIMER=rand2&1;
            AI_STATE=AI_STATE_SPECIAL_ATTACK;
            break;
        }
        break;
    }
    case AI_STATE_STEADY:{//Steady. Only 1 shot afterwards.
        float dist=ENEMY_INIT_LISTS[enemy_id].preferred_height-enemy.shipY;
        float abs_dist=abs(dist);
        if(abs_dist>10.0f*enemy.shipVelY){
            rot_towards(enemy.shipRotX,enemy.shipRotY,0,dist);
            boost=true;
        }
        //See if done.
        if(abs_dist<0.5){
            fire_default_enemy_projectile();
            AI_STATE=AI_STATE_READYING;
        }
        break;
    }
    case AI_STATE_AGGRESSIVE:{//Shoot every 40 frames. Rotate towards player. always boost.
        AI_STATE_TIMER-=1;
        if(AI_STATE_TIMER%40==0)
            fire_default_enemy_projectile();
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
                fire_default_enemy_projectile();
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
        rot_towards(enemy.shipRotX,enemy.shipRotY,-1+2*AI_STATE_TIMER,1);
        boost=true;
        if(enemy.shipY>SCREEN_SIZE)AI_STATE=AI_STATE_STEADY;
        break;
    }
    case AI_STATE_SPECIAL_ATTACK:{//Special attacks :>
        switch (ENEMY_INIT_LISTS[enemy_id].special>>2){
            case 0:
            default://special attack 0: do nothing.
                AI_STATE=AI_STATE_READYING;
        }
    }
    default:
        AI_STATE=AI_STATE_READYING;
        break;
    }
    if(ENEMY_INIT_LISTS[enemy_id].special&1)
        //Special behaviour 1: dive bullets
        if(enemy.shipY>SCREEN_SIZE+1 && frame%20==0)
            fire_default_enemy_projectile_at(enemy.shipX,SCREEN_SIZE-1,enemy.shipVelX,-BULLET_START_SPEED/1.2f);

    if(ENEMY_INIT_LISTS[enemy_id].special&2)
        //Special behaviour 2: dodge (badly)
        for(int b=0;b<=player_bullet_count;++b)
            if(player_bullets[b<<2]!=BULLET_INVALID&&enemy.dash_cooldown<0)
                if(square_distance(player_bullets[b<<2],player_bullets[(b<<2)|1],enemy.shipX,enemy.shipY)<pow2(ENEMY_INIT_LISTS[enemy_id].max_bullets/20))//I gotta use more of those empty bits...
                {
                    //trace("dodged!");
                    float dx=best_looped_dir(enemy.shipX,player_bullets[b<<2]);
                    float dy=player_bullets[(b<<2)|1]-enemy.shipY;
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
                    brake=false;
                    break;
                };
    //Finally, ship physics.
    if(boost){
        enemy.shipVelX+=enemy.shipRotX*speed;
        enemy.shipVelY+=enemy.shipRotY*speed;
    }
    ship_physics(enemy,brake);
    //and processing bullets
}
void handle_enemy_bullets(int enemy_id){
    for(int i=0;i<min(mx_bullets,1+ENEMY_INIT_LISTS[enemy_id].max_bullets);++i){
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

