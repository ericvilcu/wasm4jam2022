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
            CameraPosition = Position((float)(SCREEN_SIZE/2),(float)(SCREEN_SIZE/2));
            auto p1 = entities[0].ship.init();
            p1->position=Position((float)0,(float)0);
            p1->acc = Position((float)-1,(float)0);
            p1->ability_state = 0;
            p1->dash_cooldown = 0;
            p1->hp = 1;
            p1->control=1;
            p1->facing = p1->acc;
            auto p2 = entities[1].ship.init();
            p2->position=Position((float)1,(float)0);
            p2->acc = Position((float)-1,(float)0);
            p2->ability_state = 0;
            p2->dash_cooldown = 0;
            p2->control=2;
            p2->facing = p2->acc;
            p2->hp = 1;
            aux=61;
        }
    case MULTIPLAYER_321:
        //3, 2, 1, GO!
        drawShip(1);drawShip(2);
        if(aux>40)text("3",(SCREEN_SIZE-8*1)/2,80-4);
        else if(aux>20)text("2",(SCREEN_SIZE-8*1)/2,80-4);
        else if(aux>0)text("1",(SCREEN_SIZE-8*1)/2,80-4);
        else if(aux>-20){
            text("!",(SCREEN_SIZE-8*1)/2,80-4);
            processShip(1);processShip(2);
        }
        aux--;
        if(aux>-20)break;
    case MULTIPLAYER_GO:
        processShip(1);processShip(2);
        //Maybe a short cooldown?
        if(entities[0].ship.hp<0){progression+=1;state = MULTIPLAYER;}
        if(entities[1].ship.hp<0){losses+=1;state = MULTIPLAYER;}
        break;
    default:
        text("UNIMPLEMENTED", (SCREEN_SIZE-8*13)/2, 80-4);
        break;
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
