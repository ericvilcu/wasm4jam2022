
#include "entities.h"
#include "env.h"

entity entities[MAX_ENTITIES];
bullet bullets1[MAX_BULLETS];
bullet bullets2[MAX_BULLETS];
bullet particles[MAX_PARTICLES];
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

void processShip(int i){
    auto& shipData = entities[i].ship;
    shipData.position=shipData.position+shipData.acc;
    shipData.acc.decelerate(0.95f);
    switch (shipData.control)
    {
    case 1: case 2:
        auto ctrl = *(shipData.control==1?GAMEPAD1:GAMEPAD2);
        if(ctrl&BUTTON_LEFT) shipData.facing.rot1();
        if(ctrl&BUTTON_RIGHT)shipData.facing.rotMinus1();
        if(ctrl&BUTTON_UP)   shipData.acc=shipData.acc+(shipData.facing*1.2f);
        if(ctrl&BUTTON_DOWN) shipData.acc.decelerate(0.90f);
        break;
    }
    drawShip(i);
}

void drawShip(int i){
    auto& s=entities[i].ship;
    draw_ship(CameraPosition.real(CameraPosition.x),CameraPosition.real(CameraPosition.y),
              CameraPosition.real(s.position.x),CameraPosition.real(s.position.y),
              CameraPosition.real(s.facing.x),CameraPosition.real(s.facing.y),
              (char)(1+s.control&30));
}