#include "wasm4.h"
#include "GLOB.h"
#include "custom_draws.h"
#include "colors.h"
#include "song.h"
#include "entities.h"

enum GameState{
    MAIN,
    SOLO,
    MULTIPLAYER_GO,
    MULTIPLAYER_321,
    MULTIPLAYER,
    MULTIPLAYER_RECAP,
    STORY,
    STORY_PAUSE,
};

char SAV = '\0';
GameState state = MAIN;

int progression=0;
int losses=0;
int aux=0;

void update () {
    //frame is used mostly for random numbers and such
    frame=frame+1;
#if ENABLE_GAMEPLAY
    switch (state)
    {
    case MAIN:
        text("DEEP BELOW", (SCREEN_SIZE-8*10)/2, 80-10-4 - 13);
        text("THE DARK SEA", (SCREEN_SIZE-8*12)/2, 80+10-4 - 13);
        
        
        text("DOWN => multiplayer", 0, SCREEN_SIZE-9-8-8);
        text("X => solo fights", 0, SCREEN_SIZE-9-8);
        text("Z => story", 0, SCREEN_SIZE-9);
        if(*GAMEPAD1&BUTTON_DOWN)state = MULTIPLAYER;
        if(*GAMEPAD1&BUTTON_1)state = STORY;
        if(*GAMEPAD1&BUTTON_2)state = SOLO;
        break;
    case MULTIPLAYER:
        state = MULTIPLAYER_321;
        //Add players.
        {
            clearBullets();
            float x = -(float)(SCREEN_SIZE/2);
            CameraPosition = FakeVector2(x,-(float)(SCREEN_SIZE/2));
            auto p1 = entities[0].ship.init(1,FakeVector2((float)0,(float)0),FakeVector2((float)1,(float)0));
            auto p2 = entities[1].ship.init(2,FakeVector2((float)0,(float)0),FakeVector2((float)-1,(float)0));
            aux=61;
        }
    case MULTIPLAYER_321:
        //3, 2, 1, GO!
        drawShip(0);drawShip(1);
        if(aux>40)text("3",(SCREEN_SIZE-8*1)/2,80-4);
        else if(aux>20)text("2",(SCREEN_SIZE-8*1)/2,80-4);
        else if(aux>0)text("1",(SCREEN_SIZE-8*1)/2,80-4);
        else if(aux>-20){
            *DRAW_COLORS = 0x23;
            text("!",(SCREEN_SIZE-8*1)/2,80-4);
            *DRAW_COLORS = 0x03;
            processShip(0);processShip(1);
        }
        aux--;
        if(aux>-20)break;
        else state = MULTIPLAYER_GO;
    case MULTIPLAYER_GO: 
        CameraFollowEntities();
        processParticles();
        processBullets();
        processShip(0);processShip(1);
        if(entities[0].ship.hp<=0){losses+=1;state = MULTIPLAYER_RECAP;aux = 240;}
        if(entities[1].ship.hp<=0){progression+=1;state = MULTIPLAYER_RECAP;aux = 240;}
        break;
    case MULTIPLAYER_RECAP:
        aux--;
        drawShip(0);drawShip(1);
        processBullets(false);
        processParticles(false);
        char txt[30];
        if(aux<180){
            *DRAW_COLORS = 0x14;
            int l=0;
            char c[2]="0";
            int sc=losses;
            text("s",80,80-4);
            //this feels like the first do while i've written in 5 years.
            do{
                c[0]= '0' + (char)(sc%10);
                text(c,80-16-4-8*l++,80-4);
                sc/=10;
            }while (sc>0);
            l=0;sc=losses/10;
            while (sc>0){l++;sc/=10;}
            sc=progression;
            *DRAW_COLORS = 0x13;
            text("v",80-8,80-4);
            do{
                c[0]= '0' + (char)(sc%10);
                text(c,80+8+4+8*l--,80-4);
                sc/=10;
            }while (sc>0);
            *DRAW_COLORS = 0x03;
        }
        if(aux<0){
            state = MULTIPLAYER;
        }
        break;
    default:
        text("UNIMPLEMENTED", (SCREEN_SIZE-8*13)/2, 80-4);
        break;
    
        //Add players.
        {
            float x = -(float)(SCREEN_SIZE/2);
            CameraPosition = FakeVector2(x,-(float)(SCREEN_SIZE/2));
            auto p1 = entities[0].ship.init(1,FakeVector2((float)0,(float)0),FakeVector2((float)1,(float)0));
            auto p2 = entities[1].ship.init(2,FakeVector2((float)0,(float)0),FakeVector2((float)-1,(float)0));
            aux=61;
        }
    }
    
#endif
#if ENABLE_MUSIC
    song::process();
#endif
}

void saveUnlocks(){
    diskw(&SAV,1);
}

void start(){
    PALETTE[0] = SEA;
    PALETTE[1] = SEA_VARIATION;
    PALETTE[2] = CAPTAIN;
    PALETTE[3] = SACRIFICE;
#if ENABLE_MUSIC
    song::switchTrack(0);
#endif
    diskr(&SAV,1);
}
