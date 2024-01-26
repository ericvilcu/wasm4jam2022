#include "custom_draws.h"


struct PlayerShip{
    int type;
    Position position;
    Position acc;
    Position facing;
    //1,2 - >ctrl , etc = etc.
    int control;
    int hp;
    int dash_cooldown;
    int ability_state;
    inline PlayerShip* init(){
        type = 1;
        return this;
    }
};

union entity{
    PlayerShip ship;
};

struct bullet{
    Position position;
    int speed;
    int extra;
};

#define MAX_ENTITIES 4
extern entity entities[MAX_ENTITIES];
#define MAX_BULLETS 20
extern bullet bullets1[MAX_BULLETS];
extern bullet bullets2[MAX_BULLETS];
#define MAX_PARTICLES 60
extern bullet particles[MAX_PARTICLES];

void processEntity(int i);
void drawShip(int i);
void processShip(int i);
inline void deleteEntity(int i){entities[i].ship.type = 0;};
