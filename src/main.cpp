char state='t';

#include "wasm4.h"
#include "custom_draws.h"
#include "utils.h"
#include "texts.h"
#include "enemy.h"
int frame=0;
float shipX=80.0,shipY=80.0;
float shipRotX=1.0,shipRotY=0.0;
float camX=0.0,camY=0.0;
float TrueCamX=0.0;
float shipVelX=0.0,shipVelY=0.0;
int iFrames=0;
int hearts=3;
int player_shot_cooldown=0;
float invalid_time=0.0f;
float player_bullets[]={
    BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,
    BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,
    BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,
    BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,
    BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,
    BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,BULLET_INVALID,
};
void clearAllBullets(){
    for(int i=0;i<=player_bullet_count;++i){
        player_bullets[i<<2]=BULLET_INVALID;
    }
    for(int i=0;i<=max_enemy_bullets;++i){
        enemy_projectiles[i<<2]=BULLET_INVALID;
    }
}

void try_hit_enemy(float X,float Y){
    if(iFramesE<=0&&square_wrap_distance(X,Y,shipXE,shipYE)<8.0f){
        iFramesE=20;
        heartsE-=1;
    }
};
//heh.
void try_hit_enemy_player(float X,float Y){
    if(iFramesE<=0&&square_wrap_distance(X,Y,shipXE,shipYE)<4.0f){
        iFramesE=30;
        heartsE-=1;
    }
};
//Gotta figure out a way to export this
void try_hit_player(float X,float Y){
    if(iFrames<=0&&square_wrap_distance(X,Y,shipX,shipY)<4.0f){
        iFrames=30;
        hearts-=1;
    }
};
void process_player_bullet(float* bullets,int idx,uint8_t team){
    float X=bullets[idx<<2];
    float Y=bullets[(idx<<2)|1];
    if(Y<0.0f||Y>fSCREEN_SIZE){
        bullets[idx<<2]=BULLET_INVALID;
        return;
    }
    float dX=bullets[(idx<<2)|2];
    float dY=bullets[(idx<<2)|3];
    if(team==0){
        draw_bullet(camX,camY,X,Y,dX,dY,COLOR_PLAYER);
        try_hit_enemy(X,Y);
    }
    else if(team==1){
        draw_bullet(camX,camY,X,Y,dX,dY,COLOR_PLAYER);
        try_hit_enemy_player(X,Y);
    }
    else if(team==2){
        draw_bullet(camX,camY,X,Y,dX,dY,COLOR_ENEMY);
        try_hit_player(X,Y);
    }
    bullets[idx<<2]=fmodf(X+dX,ARENA_SIZE_X);
    bullets[(idx<<2)|1]+=dY;
    bullets[(idx<<2)|2]=dX*falloff;
    bullets[(idx<<2)|3]=dY*falloff+gravity;
}
constexpr float cam_fallback=0.1f;
void camera_follow(float target){
    float want=fmodf(target-(SCREEN_SIZE/2),ARENA_SIZE_X);
    float diff=((float)best_looped_dir(camX,want))*cam_fallback;
    camX=fmodf(camX+diff,ARENA_SIZE_X);
    TrueCamX+=diff;
    //camX=shipX-SCREEN_SIZE/2;
}
void doPlayer2Movement(){
    const uint8_t& gamepad = *GAMEPAD2;
    iFramesE-=1;
    if (gamepad & BUTTON_UP){
        shipVelXE+=shipRotXE*speed;
        shipVelYE+=shipRotYE*speed;
    }
    if(gamepad & BUTTON_DOWN){
        shipVelXE*=brake;
        shipVelYE*=brake;
    }
    player_shot_cooldownE-=1;
    if((gamepad & BUTTON_1)&& player_shot_cooldownE<0){
        enemy_projectiles[player_bullet_count<<2]=shipXE;
        enemy_projectiles[(player_bullet_count<<2)|1]=shipYE;
        enemy_projectiles[(player_bullet_count<<2)|2]=shipRotXE*BULLET_START_SPEED+shipVelXE;
        enemy_projectiles[(player_bullet_count<<2)|3]=shipRotYE*BULLET_START_SPEED+shipVelYE;
        player_shot_cooldownE=10;
    }
    if (gamepad & BUTTON_2){    
        if (gamepad & BUTTON_RIGHT)
            {shipVelXE+=-shipRotYE*speed_sides;shipVelYE+=+shipRotXE*speed_sides;}
        if (gamepad & BUTTON_LEFT)
            {shipVelXE+=+shipRotYE*speed_sides;shipVelYE+=-shipRotXE*speed_sides;}
    }
    else{
        if (gamepad & BUTTON_LEFT)
            rot_minus1(shipRotXE,shipRotYE);
        if (gamepad & BUTTON_RIGHT)
            rot_plus1(shipRotXE,shipRotYE);
    }
    shipVelYE+=gravity;
    if(shipYE>WATER_Y){
        invalid_timeE+=(shipYE-WATER_Y)/60.0f*0.05f;
        if(invalid_timeE>20.)
            shipVelYE=min(0.0f,shipVelYE);
        shipVelYE-=gravity*10.0f;
    }
    else if(shipYE<0.0f){
        invalid_timeE-=shipYE/60.0f*0.05f;
        if(invalid_timeE>20.)
            shipVelYE=max(0.0f,shipVelYE);
        shipVelYE+=gravity*10.0f;
    }
    else invalid_timeE=0.0f;
    shipXE=fmodf(shipXE+shipVelXE,(float)ARENA_SIZE_X);
    shipYE+=shipVelYE;
    shipVelXE=shipVelXE*falloff*pow2(min(1.0f,1.0f/max(1.0,shipVelXE-DYNAMIC_FALLOFF_THRESHOLD)));
    shipVelYE=shipVelYE*falloff*pow2(min(1.0f,1.0f/max(1.0,shipVelYE-DYNAMIC_FALLOFF_THRESHOLD)));
}
void doPlayerMovement(){
    const uint8_t& gamepad = *GAMEPAD1;
    iFrames-=1;
    if (gamepad & BUTTON_UP){
        shipVelX+=shipRotX*speed;
        shipVelY+=shipRotY*speed;
    }
    if(gamepad & BUTTON_DOWN){
        shipVelX*=brake;
        shipVelY*=brake;
    }
    player_shot_cooldown-=1;
    if((gamepad & BUTTON_1)&& player_shot_cooldown<0){
        player_bullets[player_bullet_count<<2]=shipX;
        player_bullets[(player_bullet_count<<2)|1]=shipY;
        player_bullets[(player_bullet_count<<2)|2]=shipRotX*BULLET_START_SPEED+shipVelX;
        player_bullets[(player_bullet_count<<2)|3]=shipRotY*BULLET_START_SPEED+shipVelY;
        player_shot_cooldown=10;
    }
    if (gamepad & BUTTON_2){    
        if (gamepad & BUTTON_RIGHT)
            {shipVelX+=-shipRotY*speed_sides;shipVelY+=+shipRotX*speed_sides;}
        if (gamepad & BUTTON_LEFT)
            {shipVelX+=+shipRotY*speed_sides;shipVelY+=-shipRotX*speed_sides;}
    }
    else{
        if (gamepad & BUTTON_LEFT)
            rot_minus1(shipRotX,shipRotY);
        if (gamepad & BUTTON_RIGHT)
            rot_plus1(shipRotX,shipRotY);
    }
    shipVelY+=gravity;
    if(shipY>WATER_Y){
        invalid_time+=(shipY-WATER_Y)/60.0f*0.05f;
        if(invalid_time>20.)
            shipVelY=min(0.0f,shipVelY);
        shipVelY-=gravity*10.0f;
    }
    else if(shipY<0.0f){
        invalid_time-=shipY/60.0f*0.05f;
        if(invalid_time>20.)
            shipVelY=max(0.0f,shipVelY);
        shipVelY+=gravity*10.0f;
    }
    else invalid_time=0.0f;
    shipX=fmodf(shipX+shipVelX,(float)ARENA_SIZE_X);
    shipY+=shipVelY;
    shipVelX=shipVelX*falloff*pow2(min(1.0f,1.0f/max(1.0,shipVelX-DYNAMIC_FALLOFF_THRESHOLD)));
    shipVelY=shipVelY*falloff*pow2(min(1.0f,1.0f/max(1.0,shipVelY-DYNAMIC_FALLOFF_THRESHOLD)));
}

void process_player_bullets(float*bullets,uint8_t team){
    for(int i=0;i<player_bullet_count;++i){
        if(bullets[i<<2]==BULLET_INVALID){//"empty" chambers float to the top to be filled.
            int j=i+1;
            bullets[i<<2]=bullets[j<<2];
            bullets[(i<<2)|1]=bullets[(j<<2)|1];
            bullets[(i<<2)|2]=bullets[(j<<2)|2];
            bullets[(i<<2)|3]=bullets[(j<<2)|3];
            bullets[j<<2]=BULLET_INVALID;
        }
        if(bullets[i<<2]!=BULLET_INVALID){
            process_player_bullet(bullets,i,team);
        }
    }
    bullets[player_bullet_count<<2]=BULLET_INVALID;//mark any potentially unshot bullet as none
}
int progression=0;
int aux=0;
void update () {
    //ship
    frame=frame+1;
    switch (state)
    {
    case 't':
        //Tutorial game loop.    
        *DRAW_COLORS = 2;
        draw_background(frame,camX,TrueCamX);
        tutorial();
        doPlayerMovement();
        process_player_bullets(player_bullets,3);
        camera_follow(shipX);
        draw_ship(camX,camY,shipX,shipY,shipRotX,shipRotY,ALL_PLAYER_COLORS);
        if(TrueCamX<-200.0){
            //PVP TIME!
            PALETTE[3] = 0x0000ff;
            shipYE=210.0f;
            invalid_timeE=200.0f;
            state='p';
            hearts=heartsE=3;
        }

        break;
    case 'p':
        //pvp game loop
        draw_background(frame,camX,TrueCamX);
        doPlayerMovement();
        doPlayer2Movement();
        draw_hearts(hearts,true);
        draw_hearts(heartsE,false);
        process_player_bullets(player_bullets,1);
        process_player_bullets(enemy_projectiles,2);
        draw_ship(camX,camY,shipX ,shipY ,shipRotX ,shipRotY ,ALL_PLAYER_COLORS);
        draw_ship(camX,camY,shipXE,shipYE,shipRotXE,shipRotYE,ALL_ENEMY_COLORS);
        camera_follow(best_middle_wrap(shipX,shipXE));
        if(hearts<=0||heartsE<=0){
            if(hearts<=0&&heartsE<=0){
                iFrames=300;
                state='q';
            }
            else if(hearts<=0){
                iFrames=300;
                progression+=1<<8;
                state='d';
            }
            else{
                iFrames=300;
                progression+=1;
                state='b';
            }
        }
        break;
    case 'd'://player 1 death
    case 'b'://player 2 death
    case 'q'://both die
        *DRAW_COLORS =3;
        draw_background(frame,camX,TrueCamX);
        if(state=='b'){
            doPlayerMovement();
        }
        else{
            shipX+=shipVelX;
            shipY+=0.01f*(float)(300-iFrames);
        }
        if(state=='d'){
            doPlayer2Movement();
        }
        else{
            shipXE+=shipVelXE;
            shipYE+=0.01f*(float)(300-iFrames);
        }
        process_player_bullets(player_bullets,1);
        process_player_bullets(enemy_projectiles,2);
        draw_ship(camX,camY,shipX ,shipY ,shipRotX ,shipRotY ,ALL_PLAYER_COLORS);
        draw_ship(camX,camY,shipXE,shipYE,shipRotXE,shipRotYE,ALL_ENEMY_COLORS);
        draw_hearts(hearts,true);
        draw_hearts(heartsE,false);
        display_score(progression&0xff,progression>>8);//I know there's 8 bytes I'm not using.
        if(state=='q')
            camera_follow(best_middle_wrap(shipX,shipXE));
        else if(state=='d')
            camera_follow(shipXE);
        else
            camera_follow(shipX);
        if(state=='q'){
            *DRAW_COLORS =4;
            text("IMPRESSIVE!",2,10);
            *DRAW_COLORS =4;
            text("WE BOTH DIED.",2,30);
            *DRAW_COLORS =3;
            text("No points counted.",2,50);
        }
        iFrames-=1;
        if(iFrames==0){
            clearAllBullets();
            if(state!='b')
                shipY=SCREEN_SIZE+5;
            if(state!='d')
                shipYE=SCREEN_SIZE+5;
            state='p';
            hearts=heartsE=3;
        }
        break;

    default:
        break;
    }
}

void start(){
    PALETTE[0] = 0x003211;
    PALETTE[1] = 0x001132;
    PALETTE[2] = 0xff0000;
    PALETTE[3] = 0xffffff;
}