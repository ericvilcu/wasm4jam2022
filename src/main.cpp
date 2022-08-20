char state='t';

#include "wasm4.h"
#include "custom_draws.h"
#include "utils.h"
#include "texts.h"
#include "enemy.h"
#include "particles.h"
int progression=0;
int losses=0;
unsigned int standard_color_0=0x00210b;
unsigned int standard_color_1=0x000b21;
void clearAllBullets(){
    for(int i=0;i<player_bullet_count;++i){
        for(int dx=-1;dx<=1;dx+=2)
            for(int dy=-1;dy<=1;dy+=2)
                add_particle(player_bullets[i<<2],player_bullets[(i<<2)|1],(float)dx/2.0f,(float)dy/2.0f,COLOR_PLAYER,0b00);
        player_bullets[i<<2]=BULLET_INVALID;
    }
    for(int i=0;i<max_enemy_bullets;++i){
        for(int dx=-1;dx<=1;dx+=2)
            for(int dy=-1;dy<=1;dy+=2)
                add_particle(enemy_projectiles[i<<2],enemy_projectiles[(i<<2)|1],(float)dx/2.0f,(float)dy/2.0f,COLOR_ENEMY,0b00);
        enemy_projectiles[i<<2]=BULLET_INVALID;
    }
}

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
void doPlayerMovement(uint8_t gamepad,ship& my_ship,uint8_t team=0){
    if((gamepad & BUTTON_1)&& my_ship.player_shot_cooldown<0){
        if(team<2){
            player_bullets[player_bullet_count<<2]=my_ship.shipX;
            player_bullets[(player_bullet_count<<2)|1]=my_ship.shipY;
            player_bullets[(player_bullet_count<<2)|2]=my_ship.shipRotX*BULLET_START_SPEED+my_ship.shipVelX;
            player_bullets[(player_bullet_count<<2)|3]=my_ship.shipRotY*BULLET_START_SPEED+my_ship.shipVelY;
            my_ship.player_shot_cooldown=10;
        }
        else{
            enemy_projectiles[player_bullet_count<<2]=my_ship.shipX;
            enemy_projectiles[(player_bullet_count<<2)|1]=my_ship.shipY;
            enemy_projectiles[(player_bullet_count<<2)|2]=my_ship.shipRotX*BULLET_START_SPEED+my_ship.shipVelX;
            enemy_projectiles[(player_bullet_count<<2)|3]=my_ship.shipRotY*BULLET_START_SPEED+my_ship.shipVelY;
            my_ship.player_shot_cooldown=10;
        }
    }

    if (gamepad & BUTTON_2){    
        if ((gamepad & BUTTON_RIGHT )&&my_ship.dash_cooldown<0){
            my_ship.dash_cooldown=DASH_COOLDOWN;
            my_ship.shipVelX+=-my_ship.shipRotY*dash_speed;
            my_ship.shipVelY+=+my_ship.shipRotX*dash_speed;
        }
        if ((gamepad & BUTTON_LEFT)&&my_ship.dash_cooldown<0){
            my_ship.dash_cooldown=DASH_COOLDOWN;
            my_ship.shipVelX+=+my_ship.shipRotY*dash_speed;
            my_ship.shipVelY+=-my_ship.shipRotX*dash_speed;
        }
        if ((gamepad & BUTTON_UP)&&my_ship.dash_cooldown<0){
            my_ship.dash_cooldown=DASH_COOLDOWN;
            my_ship.shipVelX+=my_ship.shipRotX*dash_speed;
            my_ship.shipVelY+=my_ship.shipRotY*dash_speed;
        }
    }
    else{
        if (gamepad & BUTTON_LEFT)
            rot_minus1(my_ship.shipRotX,my_ship.shipRotY);
        if (gamepad & BUTTON_RIGHT)
            rot_plus1(my_ship.shipRotX,my_ship.shipRotY);
        if (gamepad & BUTTON_UP){
            my_ship.shipVelX+=my_ship.shipRotX*speed;
            my_ship.shipVelY+=my_ship.shipRotY*speed;
        }
    }
    ship_physics(my_ship,gamepad & BUTTON_DOWN);
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
void reset_enemy(){
    enemy.hearts=ENEMY_INIT_LISTS[progression>>1].hearts&15;
    enemy.shipX=fmodf(player.shipX+HALF_ARENA_SIZE_X,ARENA_SIZE_X);
    enemy.shipY=(float)ENEMY_INIT_LISTS[progression>>1].preferred_height;
    enemy.shipVelY=0.0;
    enemy.shipVelX=-3.0;
    enemy.shipRotX=-1.0;
    enemy.shipRotY=0.0;
    AI_STATE=AI_STATE_STEADY;
    player.waterTime=enemy.waterTime=INITIAL_WATER_TIME;
}
void update () {
    frame=frame+1;
    //I think i forgot enums existed when I wrote this.
    switch (state)
    {
    case 't':
        //Tutorial game loop.
        draw_background(frame,camX,TrueCamX);
        tutorial();
        doPlayerMovement(*GAMEPAD1,player);
        process_player_bullets(player_bullets,0);
        camera_follow(player.shipX);
        draw_ship(camX,camY,player.shipX,player.shipY,player.shipRotX,player.shipRotY,ALL_PLAYER_COLORS);
        if(TrueCamX<-200.0){
            //PVP TIME!
            clearAllBullets();
            PALETTE[3] = 0x4455ff;
            enemy.shipY=210.0f;
            enemy.invalid_time=200.0f;
            state='p';
            enemy.hearts=player.hearts=3;
        }
        else if(TrueCamX>200.0){
            //Story time!
            PALETTE[3] = ENEMY_INIT_LISTS[0].color;
            state='c';
            enemy.hearts=player.hearts=3;
        }
        
        break;
    case 'c'://C is for cutscene
        draw_background(frame,camX,TrueCamX);
        process_particles(camX,camY);
        doPlayerMovement(*GAMEPAD1,player);
        process_player_bullets(player_bullets,0);
        camera_follow(player.shipX);
        handle_enemy_bullets(progression>>1);
        draw_ship(camX,camY,player.shipX,player.shipY,player.shipRotX,player.shipRotY,ALL_PLAYER_COLORS);
        if(progression&1){
            ship_die_physics(enemy,frame,ALL_ENEMY_COLORS);
            draw_ship(camX,camY,enemy.shipX,enemy.shipY,enemy.shipRotX,enemy.shipRotY,ALL_ENEMY_COLORS,false);
        }
        if(talk()==2){
            if(progression&1){//Enemy death cutscene played. Switch colors & stuff
                PALETTE[3]=ENEMY_INIT_LISTS[(progression&1)+(progression>>1)].color;
            }
            else{//Intro cutscene played. Initialize fight.
                state='s';
                //Start battle
                reset_enemy();
                player.hearts=3;
            }
            progression=progression+1;
        }
        break;
    case 's':{//Story fight
        bool underwater = player.shipY>SCREEN_SIZE;

        if(underwater){
            //God save all us sinners.
            PALETTE[0]=standard_color_1;
            PALETTE[1]=standard_color_0;
            uint8_t* CLRS8=(uint8_t*)PALETTE;
            CLRS8[2]=(uint8_t)max(0,(int)200-player.waterTime);
            draw_background_underwater(frame,TrueCamX,player.waterTime);
            camY=SCREEN_SIZE;
            clearAllBullets();
        }
        else{
            PALETTE[0]=standard_color_0;
            PALETTE[1]=standard_color_1;
            draw_background(frame,camX,TrueCamX);
        }
        process_particles(camX,camY);
        doPlayerMovement(*GAMEPAD1,player);
        do_ai_tick(frame,progression>>1);
        handle_enemy_bullets(progression>>1);
        process_player_bullets(player_bullets,0);
        draw_hearts(player.hearts,true);
        draw_hearts(enemy.hearts,false);
        camera_follow(player.shipX);
        draw_ship(camX,camY,player.shipX,player.shipY,player.shipRotX,player.shipRotY,ALL_PLAYER_COLORS);
        draw_ship(camX,camY,enemy.shipX,enemy.shipY,enemy.shipRotX,enemy.shipRotY,ALL_ENEMY_COLORS);
        if(enemy.hearts<=0){
            PALETTE[0]=standard_color_0;
            PALETTE[1]=standard_color_1;
            state='c';losses=0;
            clearAllBullets();
        }
        else if(player.hearts<=0){
            PALETTE[0]=standard_color_1;
            PALETTE[1]=standard_color_0;
            player.iFrames=120;
            clearAllBullets();
            state='o';losses+=1;
        }
        camY=0.0;
        break;
    }
    case 'o'://game over :<
        draw_background(frame,camX,TrueCamX);
        draw_ship(camX,camY,player.shipX,player.shipY,player.shipRotX,player.shipRotY,ALL_PLAYER_COLORS,false);
        draw_ship(camX,camY,enemy.shipX,enemy.shipY,enemy.shipRotX,enemy.shipRotY,ALL_ENEMY_COLORS);
        do_ai_tick(frame,progression>>1);
        process_particles(camX,camY);
        ship_die_physics(player,frame,ALL_PLAYER_COLORS);
        *DRAW_COLORS=3;
        text("Let's try\n        that again.",4,0);
        if(losses>2)
            text("You can hold\nx+z to skip\nthis faster.",4,60);
        if(losses%3==0)
            text("This loss will\nmake you stronger.",4,160-8*2-1);
        player.iFrames-=1;
        if(player.iFrames<0 || (player.iFrames<120 && (((*GAMEPAD1) & (BUTTON_1|BUTTON_2))==0b11)))
        {
            clearAllBullets();
            clear_particles();
            state='s';
            //Start battle
            reset_enemy();
            player.hearts=3+losses/4;
            player.shipY=95.5f;
            player.shipVelX=0.0f;
            player.shipVelY=0.0f;
            player.shipRotX=-1.0f;
            player.shipRotY=0.0f;
        }
        break;
    case 'p':
        //pvp game loop
        draw_background(frame,camX,TrueCamX);
        process_particles(camX,camY);
        doPlayerMovement(*GAMEPAD1,player,1);
        doPlayerMovement(*GAMEPAD2,enemy,2);
        draw_hearts(player.hearts,true);
        draw_hearts(enemy.hearts,false);
        process_player_bullets(player_bullets,1);
        process_player_bullets(enemy_projectiles,2);
        draw_ship(camX,camY,player.shipX ,player.shipY ,player.shipRotX ,player.shipRotY ,ALL_PLAYER_COLORS);
        draw_ship(camX,camY,enemy.shipX,enemy.shipY,enemy.shipRotX,enemy.shipRotY,ALL_ENEMY_COLORS);
        if(!((*NETPLAY)&0b100)){
            camera_follow(best_middle_wrap(player.shipX,enemy.shipX));
        }
        else{
            if((*NETPLAY)&1){
                camera_follow(enemy.shipX);
            }
            else{
                camera_follow(player.shipX);
            }
        }
        if(player.hearts<=0||enemy.hearts<=0){
            if(player.hearts<=0&&enemy.hearts<=0){
                player.iFrames=300;
                state='q';
            }
            else if(player.hearts<=0){
                player.iFrames=300;
                losses+=1;
                state='d';
            }
            else{
                player.iFrames=300;
                progression+=1;
                state='b';
            }
        }
        break;
    case 'd'://player 1 death
    case 'b'://player 2 death
    case 'q'://both die
        draw_background(frame,camX,TrueCamX);
        process_particles(camX,camY);
        if(state=='b'){
            doPlayerMovement(*GAMEPAD1,player,1);
        }
        else{
            ship_die_physics(player,frame,ALL_PLAYER_COLORS);
        }
        if(state=='d'){
            doPlayerMovement(*GAMEPAD2,enemy,2);
        }
        else{
            ship_die_physics(enemy,frame,ALL_ENEMY_COLORS);
        }
        process_player_bullets(player_bullets,1);
        process_player_bullets(enemy_projectiles,2);
        draw_ship(camX,camY,player.shipX, player.shipY, player.shipRotX ,player.shipRotY ,ALL_PLAYER_COLORS);
        draw_ship(camX,camY,enemy.shipX , enemy.shipY , enemy.shipRotX  ,enemy.shipRotY  ,ALL_ENEMY_COLORS);
        draw_hearts(player.hearts,true);
        draw_hearts(enemy.hearts,false);
        display_score(progression,losses);
        if(state=='q')
            camera_follow(best_middle_wrap(player.shipX,enemy.shipX));
        else if(state=='d')
            camera_follow(enemy.shipX);
        else
            camera_follow(player.shipX);
        if(state=='q'){
            *DRAW_COLORS =4;
            text("IMPRESSIVE!",2,10);
            *DRAW_COLORS =4;
            text("WE BOTH DIED.",2,30);
            *DRAW_COLORS =3;
            text("No points counted.",2,50);
        }
        player.iFrames-=1;
        if(player.iFrames==0){
            clearAllBullets();
            if(state!='b'){
                player.shipY=SCREEN_SIZE+5;
                player.shipVelY=-1;
            }
            if(state!='d'){
                enemy.shipY=SCREEN_SIZE+5;
                enemy.shipVelY=-1;
            }
            state='p';
            player.hearts=enemy.hearts=3;
            player.waterTime=enemy.waterTime=INITIAL_WATER_TIME;

        }
        break;
    }
}


void start(){
    PALETTE[0] = standard_color_0;
    PALETTE[1] = standard_color_1;
    PALETTE[2] = 0xff0000;
    PALETTE[3] = 0xffffff;
    if(abs(sqrt(1.0f)-1.0f)>=1e-2f)trace("FIX sqrt (again)");
    if(abs(sqrt(4.0f)-2.0f)>=1e-2f)trace("FIX sqrt (again)");
    if(abs(sqrt(81.0f)-9.0f)>=1e-2f)trace("FIX sqrt (again)");
    if(abs(sqrt(25.0f)-5.0f)>=1e-2f)trace("FIX sqrt (again)");
    if(abs(sqrt(49.0f)-7.0f)>=1e-2f)trace("FIX sqrt (again)");
}
