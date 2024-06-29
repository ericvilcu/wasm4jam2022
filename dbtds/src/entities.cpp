
#include "entities.h"
#include "env.h"
#include "random.h"

constexpr int LASER_WARMUP_FRAMES = 40;
constexpr int LASER_FIRE_FRAMES = 90;
constexpr float LASER_FIRE_FRAMES_BIAS = (LASER_WARMUP_FRAMES+LASER_FIRE_FRAMES)/(float)LASER_WARMUP_FRAMES;

entity entities[MAX_ENTITIES];
bullet bullets[MAX_BULLETS*2];
bullet particles[MAX_PARTICLES];

void clearBullets(){
    for(int i=0; i<MAX_PARTICLES; ++i)
        bullets[i].extra=0;
    for(int i=0; i<MAX_PARTICLES; ++i)
        particles[i].extra=0;
}

void processBullets(bool move){
    for(int i=0; i<MAX_BULLETS*2; ++i){
        if(bullets[i].extra==0)continue;
        auto& bullet = bullets[i];
        int b_type = bullet.extra & B_TYPE_MASK;
        uint8_t clr = bullet.extra & B_COLOR_MASK;
        FakeVector2 mv=FakeVector2(0,0);//used only for aimed shots, here so we don't calculate it twice. should use minimal stack memory.
        if(move){
            if(b_type == B_TYPE0||b_type == B_TYPE2){
                bullet.position=bullet.position+bullet.speed;
                bullet.speed.decelerate(0.98f);
                if(abs(bullet.speed.x)+abs(bullet.speed.y)<(b_type == B_TYPE0?700000:100)){
                    bullet.extra = 0;
                    ParticleBurst(4, bullet.position, FakeVector2((int)0,(int)0), F_TYPE1|((int)clr<<F_COLOR_SHIFT), 0.3f);
                }
            }
            else if(b_type == B_TYPE1){
                //find target
                for(int idx = i>=MAX_BULLETS?0:1; idx < (i>=MAX_BULLETS?1:MAX_ENTITIES);++idx)
                    if(entities[idx].positional.type&1)
                        mv= bullet.position.closestVectorTo(entities[idx].positional.position).normalize();
                bullet.speed = bullet.speed + mv * 0.1f;
                bullet.position=bullet.position+bullet.speed;
                bullet.speed.decelerate(0.98f);
                //shit just never dies, for now.
                //if(abs(bullet.speed.x)+abs(bullet.speed.y)<700000){
                //    bullet.extra = 0;
                //    ParticleBurst(4, bullet.position, FakeVector2((int)0,(int)0), F_TYPE1|((int)clr<<F_COLOR_SHIFT), 0.3f);
                //}
            }
            else{
                //LASERS! :>
                if(bullet.extra&(1<<B_USER_OFFSET)){
                    //do stuff
                    bullet.extra^=(1<<B_USER_OFFSET);
                }
                else{//delete self. this will only happen if the ability is interrupted.
                    bullet.extra = 0;continue;
                }

            }
        }
        int x,y;
        bullet.position.ToCamera(CameraPosition, x, y);
        if(b_type==B_TYPE1)
            custom_draws::draw_bullet(x,y,FakeVector2::real(mv.x)*1.5f,FakeVector2::real(mv.y)*1.5f,clr);
        else if(b_type==B_TYPE0 || b_type==B_TYPE2)
            custom_draws::draw_bullet(x,y,FakeVector2::real(bullet.speed.x),FakeVector2::real(bullet.speed.y),clr);
        else{
            auto pwr = min(LASER_FIRE_FRAMES_BIAS-(float)(bullet.extra>>B_L_LIFETIME_OFFSET)/LASER_WARMUP_FRAMES,(float)(bullet.extra>>B_L_LIFETIME_OFFSET)/LASER_FIRE_FRAMES);
            if(bullet.extra<(1<<B_L_LIFETIME_OFFSET)*(LASER_FIRE_FRAMES*1+1))pwr=-pwr;
            custom_draws::draw_laser(x,y,
                FakeVector2::real(bullet.speed.x),
                FakeVector2::real(bullet.speed.y),
                bullet.extra&B_COLOR_MASK,abs(pwr),pwr<0);
        }
    }
}

//I wonder if having them in reverse order might be better as it may be block copied due to upside down stack shenanigans.
//NOTE: due to 20 bullet maximum, n^2 on bursts is ok?
void addBullet(FakeVector2 position,FakeVector2 speed, int flags, bool team, bool force = false){
    for(int i=(team?MAX_BULLETS:0); i<MAX_BULLETS*(team?2:1); ++i)
        if(bullets[i].extra==0){
            bullets[i].position=position;
            bullets[i].speed=speed;
            bullets[i].extra=flags;
            return;
        }
    if(force){
        int i= (team?MAX_BULLETS:0);
        bullets[i].position=position;
        bullets[i].speed=speed;
        bullets[i].extra=flags;
    }
}

//NOTE: a is to the left.
void IsBelow(FakeVector2 position, FakeVector2 a, FakeVector2 b, bool& outBelow, bool& outAbove){
    if(a.x==b.x)return;
    //overflows will happen. this is ok.
    if(a.x<b.x){
        bool ok_x = a.x<position.x && b.x>position.x;
        if(!ok_x) return;
        int px = (int)(position.x-b.x), py = (int)(position.y-b.y);
        int bx = (int)(a.x-b.x)       , by=(int)(a.y-b.y);
        outAbove|=(long long)px*by>(long long)(py)*(long long)(bx);
    }
    else{
        bool ok_xy = b.x<position.x && a.x>position.x;
        if(!ok_xy) return;
        int px = (int)(position.x-a.x), py = (int)(position.y-a.y);
        int bx = (int)(b.x-a.x)       , by=(int)(b.y-a.y);
        outBelow|=(long long)px*by<(long long)(py)*(long long)(bx);
    }
}
bool checkLaserCollision(FakeVector2 position, const bullet& laser){
    FakeVector2 keyPoints[5];
    int frames = laser.extra>>B_L_LIFETIME_OFFSET;
    if(frames>LASER_FIRE_FRAMES) return false;
    float pwr = min(LASER_FIRE_FRAMES_BIAS-(float)(frames)/LASER_WARMUP_FRAMES,(float)(frames)/LASER_FIRE_FRAMES);
    float width=7.0f*pwr;
    float fx = FakeVector2::real(laser.speed.x);
    float fy = FakeVector2::real(laser.speed.y);
    float cfx=fy;
    float cfy=-fx;
    bool isBelow1Above=false;
    bool isAbove1Below=false;
    keyPoints[0]=laser.position                       +FakeVector2(cfx,cfy)*width;
    keyPoints[1]=laser.position+FakeVector2(fx,fy)*75 +FakeVector2(cfx,cfy)*width -keyPoints[0];
    keyPoints[2]=laser.position+FakeVector2(fx,fy)*100                            -keyPoints[0];
    keyPoints[3]=laser.position+FakeVector2(fx,fy)*75 +FakeVector2(cfx,cfy)*-width-keyPoints[0];
    keyPoints[4]=laser.position                       +FakeVector2(cfx,cfy)*-width-keyPoints[0];
    position=position-keyPoints[0];
    keyPoints[0]=keyPoints[0]-keyPoints[0];
    int minY=min(min(keyPoints[0].y,keyPoints[1].y),min(min(keyPoints[2].y,keyPoints[3].y),keyPoints[4].y));
    int maxY=max(max(keyPoints[0].y,keyPoints[1].y),max(max(keyPoints[2].y,keyPoints[3].y),keyPoints[4].y));
    bool ok_y = minY<position.y && maxY>position.y;
    if(!ok_y)return false;
    IsBelow(position,keyPoints[0],keyPoints[1],isBelow1Above,isAbove1Below);
    IsBelow(position,keyPoints[1],keyPoints[2],isBelow1Above,isAbove1Below);
    IsBelow(position,keyPoints[2],keyPoints[3],isBelow1Above,isAbove1Below);
    IsBelow(position,keyPoints[3],keyPoints[4],isBelow1Above,isAbove1Below);
    IsBelow(position,keyPoints[4],keyPoints[0],isBelow1Above,isAbove1Below);
    return isBelow1Above && isAbove1Below;
}

bool checkCollision(FakeVector2 position, bool team, float dim = 2.0f){
    for(int i=(!team?0:MAX_BULLETS); i<MAX_BULLETS*(!team?1:2); ++i)
        if(bullets[i].extra!=0){
            if((bullets[i].extra&B_TYPE_MASK)!=B_TYPE3){
                if((position-bullets[i].position).squareHits(dim))
                    return true;
            }
            else{
                if(checkLaserCollision(position,bullets[i])) return true;
            }
        }
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
            if(particles[i].extra&F_COLOR_SWITCH) custom_draws::switchPixel(x,y);
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

//really should have made different files for all entity types... bit too lazy to do that now.
void processAbility(int i){
    auto& ab=entities[i].ability;
    if((frame&15) == 0)
        ParticleBurst(1,ab.position,FakeVector2((int)0,(int)0),F_TYPE1|F_COLOR_SWITCH,0.2f);
    //check ship collisions
    for(int i=0;i<MAX_ENTITIES;++i)
        if(entities[i].positional.type==1 && entities[i].ship.ability_state==0
         && (entities[i].ship.position-ab.position).squareHits(6.0f)){
            entities[i].ship.ability_state = 4 | ab.ability;
            ParticleBurst(10,ab.position,FakeVector2((int)0,(int)0),F_TYPE1|F_COLOR_SWITCH,0.5f);
            ab.type = 0;
        }
    int x,y;
    entities[i].ability.position.ToCamera(CameraPosition, x, y);
    custom_draws::draw_ability(x,y,ab.ability);
}

void processEntity(int i, bool move){
    auto type = entities[i].ship.type;
    switch (type)
    {
    case 1:
        processShip(i, move);
        break;
    case 2:
        if(move)processAbility(i);
        break;
    default:
        return;
    }
}

void collisionEntity(int i){
    auto type = entities[i].ship.type;
    switch (type)
    {
    case 1:
        collisionShip(i);
        break;
    default:
        return;
    }
}

void processEntities(bool move, bool particles){
    if(particles)processParticles();
    for(int i=0;i<MAX_ENTITIES;++i)
        processEntity(i, move);
    processBullets(move);
    if(move)
        for(int i=0;i<MAX_ENTITIES;++i)
            collisionEntity(i);
}

void CameraFollowEntities(float factor){
    int n=0;
    for(int i=0;i<MAX_ENTITIES;++i)
        if((entities[i].positional.type&1)!=0)
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
            if((entities[i].positional.type&1)!=0)
                averagePos=averagePos+(entities[i].positional.position-entities[c].positional.position)*weight;
        averagePos = averagePos + entities[c].positional.position;
        for(int i=0;i<MAX_ENTITIES;++i)
            if((entities[i].positional.type&1)!=0){
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

void collisionShip(int i){

    auto& shipData = entities[i].ship;
    if(shipData.ability_state<0) {
        shipData.ability_state+=1;
        if(shipData.ability_state>=0) de_emphasise_color((i&1)|2);
    }
    //TODO: move this after bullet position processing
    else if(checkCollision(shipData.position, 1^(i&1))){
        ParticleBurst(30, shipData.position, shipData.spd, ((2^(i&1))<<F_COLOR_SHIFT) | F_TYPE1);
        de_emphasise_color((i&1)|2);
        shipData.hp-=1;
        shipData.ability_state=-100;
    }
    else if (shipData.ability_state>0){
        if((frame&31) == 0)
            ParticleBurst(1,shipData.position,FakeVector2((int)0,(int)0),F_TYPE1|F_COLOR_SWITCH,0.2f);
            //drawing the ability somewhere would be nice... I'll see if I'm too lazy for it or not
    }
}

void processShip(int i, bool move){
    auto& shipData = entities[i].ship;
    if(move)
    {
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
                if(ctrl&BUTTON_LEFT) shipData.facing.rot(ROT_NORMAL_T);
                if(ctrl&BUTTON_RIGHT)shipData.facing.rot(ROT_NORMAL_C);
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
            if (shipData.ability_state>0 && (((ctrl&(BUTTON_1|BUTTON_2|BUTTON_DOWN))^(BUTTON_1|BUTTON_2|BUTTON_DOWN))==0 || (shipData.ability_state&8)!=0)){
                    drawShip(i);
                int ab = shipData.ability_state&3;
                shipData.ability_state|=8;
                if(ab==0){
                    emphasise_color((i&1)|2);
                    //ABILITY ONE: BURST
                    for(int __=0;__<10;++__)
                        addBullet(shipData.position,shipData.spd+random::randomPosition11()*(3.5f+random::random01()),(i&1?B_COLOR3:B_COLOR2)|B_TYPE2,i&1);
                    ParticleBurst(30, shipData.position, shipData.spd, ((2^(i&1))<<F_COLOR_SHIFT) | F_TYPE1);
                    shipData.ability_state = -15;
                }
                if(ab==1){
                    //ABILITY TWO: LASER
                    if(shipData.ability_state<=15){
                        shipData.ability_state |= 16*(LASER_WARMUP_FRAMES+LASER_FIRE_FRAMES+1);
                        addBullet(shipData.position,shipData.spd+random::randomPosition11()*(3.5f+random::random01()),(i&1?B_COLOR3:B_COLOR2)|B_TYPE3,i&1,true);
                    }
                    int laserIdx=0;
                    for(laserIdx=(i&1?MAX_BULLETS:0); laserIdx<MAX_BULLETS*(i&1?2:1)-1; ++laserIdx)
                        if((bullets[laserIdx].extra&B_TYPE_MASK)==B_TYPE3)
                            break;
                    shipData.ability_state -= 16;
                    if(shipData.ability_state<=15){
                        shipData.ability_state = -3;
                        bullets[laserIdx].extra=0;
                    }
                    else
                    {
                        shipData.spd.decelerate(0.93f);
                        if(!(bool)(ctrl&BUTTON_2)){
                            if(ctrl&BUTTON_RIGHT) shipData.facing.rot(ROT_NINETY_PERCENT_T);
                            if(ctrl&BUTTON_LEFT ) shipData.facing.rot(ROT_NINETY_PERCENT_C);
                        }
                        bullets[laserIdx].position=shipData.position;
                        bullets[laserIdx].speed=shipData.facing;
                        bullets[laserIdx].extra=(bullets[laserIdx].extra&((1<<B_L_LIFETIME_OFFSET)-1))|(shipData.ability_state>>4)<<B_L_LIFETIME_OFFSET|(1<<B_USER_OFFSET);
                        if(shipData.ability_state<16*(LASER_FIRE_FRAMES*1+1))
                        {
                            if((frame&3) == 0)
                                ParticleBurst(1, shipData.position, shipData.facing*2, ((2^(i&1))<<F_COLOR_SHIFT) | F_TYPE1,0.8f);
                            emphasise_color((i&1)|2);
                        }
                    }
                }
                if(ab==2){
                    emphasise_color((i&1)|2);
                    //ABILITY THREE: HOMING MISSILES
                    for(int __=0;__<2;++__)
                        addBullet(shipData.position,shipData.spd+random::randomPosition11()*(3.5f+random::random01()),(i&1?B_COLOR3:B_COLOR2)|B_TYPE1,i&1);
                    shipData.ability_state = -15;
                }
                if(ab==3){
                    //ABILITY FOUR:
                    //heal 1. (if you don't get hit)
                    if(shipData.ability_state<=15)shipData.ability_state |= 16*(60*3+1);
                    shipData.ability_state-=16;
                    if(shipData.ability_state<=15){
                        shipData.ability_state = 0;
                        shipData.hp+=1;
                        ParticleBurst(30,shipData.position,FakeVector2((int)0,(int)0),F_TYPE1|(i&1?F_COLOR3:F_COLOR2),9.0f);
                        break;
                    }
                    auto& s=entities[i].ship;
                    int x,y;
                    s.position.ToCamera(CameraPosition,x,y);
                    custom_draws::boxContour(x,y,2,COLOR_BACKGROUND_ACCENT);
                    if(shipData.ability_state<16*60*2)custom_draws::boxContour(x,y,3,(3^(i&1)));
                    if(shipData.ability_state<16*60*1)custom_draws::boxContour(x,y,4,COLOR_BACKGROUND_ACCENT);
                    if((frame&15) == 0)
                        ParticleBurst(1,shipData.position,FakeVector2((int)0,(int)0),F_TYPE1|F_COLOR_SWITCH,0.4f);
                    drawShip(i);
                }
            }
            break;
        }
    }
    drawShip(i);
}

void drawShip(int i){
    auto& s=entities[i].ship;
    custom_draws::draw_hearts(s.hp,(i&1));
    int x,y;
    s.position.ToCamera(CameraPosition,x,y);
    custom_draws::draw_ship(x,y,
              FakeVector2::real(s.facing.x),FakeVector2::real(s.facing.y),
              (char)(((2^(i&1))<<2)|(3^(i&1))),s.ability_state>=0);
}

SpecialAbility* SpecialAbility::randInit()
{
    type = 2;
    //NOTE: the randomInt cycle has too many things divisible by 4
    this->ability = (frame+random::randomInt())&3;
    this->position = random::randomPosition();
    return this;
}
