#include "wasm4.h"
#include "GLOB.h"
#include "custom_draws.h"
#include "colors.h"
#include "song.h"

int progression=0;
int losses=0;
int frame;


void update () {
    //frame is used mostly for random numbers and such
    frame=frame+1;
#if ENABLE_GAMEPLAY
#endif
#if ENABLE_MUSIC
    song::process();
#endif
}


void start(){
    PALETTE[0] = SEA;
    PALETTE[1] = SEAVARIATION;
    PALETTE[2] = SACRIFICE;
    PALETTE[3] = WHITE;
    //sound::switch_track(-1);
}
