#include "custom_draws.h"

struct Entity{
    int type;
};

struct EntityWithPosition:public Entity{
    FakeVector2 position;
};

struct PlayerShip:public EntityWithPosition{
    FakeVector2 spd;
    FakeVector2 facing;
    //1,2 - >ctrl , etc = etc.
    int control;
    int hp;
    int fireCooldown;
    int dash_cooldown;
    int ability_state;
    inline PlayerShip* init(int ctrl, const FakeVector2& where, const FakeVector2& speed){
        type = 1;
        position = where;
        spd = speed;
        control = ctrl;
        hp = 3;
        facing = speed;
        //NOTE: speed must never be 0 at start which is a strange contrivance I will abide by
        facing.normalize();
        dash_cooldown = 0;
        ability_state = -1;
        return this;
    }
};

struct SpecialAbility:public EntityWithPosition{
    int ability;
    SpecialAbility* randInit();
};

union entity{
    EntityWithPosition positional;
    PlayerShip ship;
    SpecialAbility ability;
};

struct bullet{
    FakeVector2 position;
    FakeVector2 speed;
    int extra;
};

#define MAX_ENTITIES 4
extern entity entities[MAX_ENTITIES];
#define MAX_BULLETS 20
extern bullet bullets[MAX_BULLETS+MAX_BULLETS];
#define MAX_PARTICLES 60
enum particleFlags{
    F_COLOR_SHIFT = 2,
    F_COLOR0 = 0<<F_COLOR_SHIFT,
    F_COLOR1 = 1<<F_COLOR_SHIFT,
    F_COLOR2 = 2<<F_COLOR_SHIFT,
    F_COLOR3 = 3<<F_COLOR_SHIFT,
    F_COLOR_MASK = 3<<F_COLOR_SHIFT,
    F_COLOR_SWITCH = 1<<4,
    F_TYPE1 = 1<<0,
    F_TYPE2 = 2<<0,
    F_TYPE3 = 3<<0,
    F_TYPE_MASK = 3<<0,
};
enum bulletFlags{
    B_COLOR0 = 0,
    B_COLOR1 = 1,
    B_COLOR2 = 2,
    B_COLOR3 = 3,
    B_COLOR_MASK = 3,
    B_TYPE0 = 0,
    B_TYPE1 = 4,
    B_TYPE2 = 8,
    B_TYPE3 = 4|8,
    B_TYPE_MASK = 4|8,
    B_USER_OFFSET = 4,
    B_L_LIFETIME_OFFSET=5,
};
extern bullet particles[MAX_PARTICLES];

void CameraFollowEntities(float factor=0.1f);
void CameraFollowPlayer(float factor=0.1f);
void CameraFollow(const FakeVector2& p,float factor=0.1f);

void processBullets(bool move=true);
void processParticles(bool move=true);
void clearBullets();
void processEntities(bool move=true, bool particles=true);
void processEntity(int i, bool move=true);
void drawShip(int i);
void processShip(int i, bool move=true);
void collisionShip(int i);
inline void deleteEntity(int i){entities[i].positional.type = 0;};

void ParticleBurst(int num, FakeVector2 pos, FakeVector2 speed, int flags, float spdMul=4.0f);
