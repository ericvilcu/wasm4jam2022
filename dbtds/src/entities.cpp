
#include "entities.h"
#include "env.h"
#include "random.h"

entity entities[MAX_ENTITIES];
bullet bullets[MAX_BULLETS*2];
bullet particles[MAX_PARTICLES];

void clearBullets(){
    for(int i=0; i<MAX_BULLETS*2; ++i)
        bullets[i].extra=0;
}

void processBullets(bool move){
    for(int i=0; i<MAX_BULLETS*2; ++i){
        if(bullets[i].extra==0)continue;
        uint8_t clr = bullets[i].extra & B_COLOR_MASK;
        if(move){
            bullets[i].position=bullets[i].position+bullets[i].speed;
            bullets[i].speed.decelerate(0.98f);
            if(abs(bullets[i].speed.x)+abs(bullets[i].speed.y)<700000){
                bullets[i].extra = 0;
                ParticleBurst(4, bullets[i].position, FakeVector2((int)0,(int)0), F_TYPE1|((int)clr<<F_COLOR_SHIFT), 0.3f);
            }
        }
        int x,y;
        bullets[i].position.ToCamera(CameraPosition, x, y);
        custom_draws::draw_bullet(x,y,FakeVector2::real(bullets[i].speed.x),FakeVector2::real(bullets[i].speed.y),clr);
    }
}

//I wonder if having them in reverse order might be better as it may be block copied due to upside down stack shenanigans.
//NOTE: due to 20 bullet maximum, n^2 on bursts is ok?
void addBullet(FakeVector2 position,FakeVector2 speed, int flags, bool team){
    for(int i=(team?MAX_BULLETS:0); i<MAX_BULLETS*(team?2:1); ++i)
        if(bullets[i].extra==0){
            bullets[i].position=position;
            bullets[i].speed=speed;
            bullets[i].extra=flags;
            return;
        }
}

bool checkCollision(FakeVector2 position, bool team, float dim = 3.0f){
    for(int i=(!team?0:MAX_BULLETS); i<MAX_BULLETS*(!team?1:2); ++i)
        if(bullets[i].extra!=0)
            if((position-bullets[i].position).squareHits(dim))
                return true;
    return false;
}

void processParticles(bool move){
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        if(particles[i].extra==0) continue;
        int x,y;
        if(move) particles[i].position.ToCamera(CameraPosition, x, y);
        switch (particles[i].extra&F_TYPE_MASK)
        {
        case 0://Note: impossible
            break;
        case 1:
            if(particles[i].extra&F_COLOR_SWITCH_BACKGROUND) custom_draws::switchPixelIfBackground(x,y);
            else setPixel((particles[i].extra&F_COLOR_MASK)>>F_COLOR_SHIFT,x,y);
            if(move) particles[i].speed.decelerate(0.98f);
            if(!isInScreen(x, y) || (abs(particles[i].speed.x)+abs(particles[i].speed.y)<30000)) particles[i].extra = 0;
            break;
        case 2:
            break;
        case 3:
            break;
        }
        particles[i].position = particles[i].position + particles[i].speed;
    }
}

void ParticleBurst(int num, FakeVector2 pos, FakeVector2 speed, int flags, float spdMul){
    for (int i = 0; i < MAX_PARTICLES && num>0; i++)
        if(particles[i].extra==0)
        {
            particles[i].position = pos;
            particles[i].speed = speed + random::randomPosition11() * (spdMul * (random::random01() + 0.2f));
            particles[i].extra = flags;
            num--;
        }
}

void processEntity(int i){
    auto type = entities[i].ship.type;
    switch (type)
    {
    case 1:
        processShip(i);
    default:
        return;
    }
}

void CameraFollowEntities(float factor){
    int n=0;
    for(int i=0;i<MAX_ENTITIES;++i)
        if(entities[i].positional.type!=0)
            n++;
    float weight = (1.0f/(float)n);
    FakeVector2::FkFLT candidateX=0;
    FakeVector2::FkFLT candidateY=0;
    float candidateCostX = 10000;
    float candidateCostY = 10000;
    for(int c=0;c<MAX_ENTITIES;++c){
        FakeVector2 averagePos = FakeVector2(0,0);
        float costX=0.0f, costY=0.0f;
        for(int i=0;i<MAX_ENTITIES;++i)
            if(entities[i].positional.type!=0)
                averagePos=averagePos+(entities[i].positional.position-entities[c].positional.position)*weight;
        averagePos = averagePos + entities[c].positional.position;
        for(int i=0;i<MAX_ENTITIES;++i)
            if(entities[i].positional.type!=0){
                FakeVector2 dist = entities[i].positional.position.closestVectorTo(averagePos);
                costX += abs(FakeVector2::real(dist.x));
                costY += abs(FakeVector2::real(dist.y));
            }
        FakeVector2 distCam = CameraPosition.fromTopRightCornerToCenter().closestVectorTo(averagePos);
        //Camera shouldn't swing around wildly and constantly.
        costX += abs(FakeVector2::real(distCam.x)/5.0f*(float)n);
        costY += abs(FakeVector2::real(distCam.y)/5.0f*(float)n);
        if(costX<candidateCostX){candidateCostX=costX;candidateX=averagePos.x;}
        if(costY<candidateCostY){candidateCostY=costY;candidateY=averagePos.y;}
    }
    CameraFollow(FakeVector2(candidateX,candidateY),factor);
}

void CameraFollowPlayer(float factor){
    CameraFollow(entities[0].positional.position,factor);
}

void CameraFollow(const FakeVector2& p, float factor){
    CameraPosition = CameraPosition + CameraPosition.fromTopRightCornerToCenter().closestVectorTo(p)*factor;
}

void processShip(int i){
    auto& shipData = entities[i].ship;
    shipData.position=shipData.position+shipData.spd;
    shipData.dash_cooldown-=1;
    shipData.spd.decelerate(falloff);
    switch (shipData.control)
    {
    case 1: case 2:
        auto ctrl = *(shipData.control==1?GAMEPAD1:GAMEPAD2);
        if(ctrl&BUTTON_2)
        {
            int fx=0,fy=0;
            if(ctrl&BUTTON_LEFT) fx-=1;
            if(ctrl&BUTTON_RIGHT)fx+=1;
            if(ctrl&BUTTON_UP)   fy+=1;
            if(ctrl&BUTTON_DOWN) {fy-=1;shipData.spd.decelerate(0.95f);}
            if((fx|fy) && shipData.dash_cooldown<=0){
                                                                //NOTE: fast diagonal dashes are fun. I had to stop myself from adding extra speed to diagonals.
                shipData.spd=shipData.spd+FakeVector2(-fx*shipData.facing.y+fy*shipData.facing.x,fy*shipData.facing.y+fx*shipData.facing.x)/*.normalize()*/ * dash_speed;
                shipData.dash_cooldown=DASH_COOLDOWN;
            }
        }
        else
        {
            if(ctrl&BUTTON_LEFT) shipData.facing.rotMinus1();
            if(ctrl&BUTTON_RIGHT)shipData.facing.rot1();
            if(ctrl&BUTTON_UP)   shipData.spd=shipData.spd+(shipData.facing * 0.05f);
            if(ctrl&BUTTON_DOWN) shipData.spd.decelerate(0.85f);
        }
        shipData.fireCooldown-=1;
        if((ctrl&BUTTON_1) && shipData.fireCooldown<=0){
            shipData.fireCooldown=40;
            addBullet(shipData.position,shipData.spd+shipData.facing*4,i&1?B_COLOR3:B_COLOR2,i&1);
            //Maybe use that for a specific boss?
            //ParticleBurst(20, shipData.position, shipData.spd, ((3^(shipData.control&1))<<F_COLOR_SHIFT) | F_TYPE1);
        }
        if(shipData.ability_state<0) shipData.ability_state+=1;
        else if(checkCollision(shipData.position, 1^(i&1))){
            ParticleBurst(30, shipData.position, shipData.spd, ((2^(i&1))<<F_COLOR_SHIFT) | F_TYPE1);
            shipData.hp-=1;
            shipData.ability_state=-100;
        }
        break;
    }
    drawShip(i);
}

void drawShip(int i){
    auto& s=entities[i].ship;
    custom_draws::draw_hearts(entities[i].ship.hp,(i&1));
    int x,y;
    s.position.ToCamera(CameraPosition,x,y);
    custom_draws::draw_ship(x,y,
              FakeVector2::real(s.facing.x),FakeVector2::real(s.facing.y),
              (char)(((2^(i&1))<<2)|(3^(i&1))));
}