#pragma once
#include "wasm4.h"
#define SOUND_FROM(f1,f2,atk,decay,sus,rel,peak,volume,flags) f1+0x10000*(uint32_t)(f2),((uint32_t)(atk)<<24)+((uint32_t)(decay)<<16)+(uint32_t)(sus)+((uint32_t)(rel)<<8),(peak<<8)+volume,flags
#define TRIANGLE_HUM_FLAGS(f,sus,vol,flags) SOUND_FROM(f,f,0,0,sus,0,vol,vol,TONE_TRIANGLE)
#define TRIANGLE_HUM(f,sus,vol) TRIANGLE_HUM_FLAGS(f,sus,vol,0)
#define MENACING_DRUM SOUND_FROM(60,60,5,10,0,35,30,20,TONE_NOISE)
#define NOTE_SQ_LOW(fr,vol) SOUND_FROM(fr,fr,0,0,5,20,1,vol,TONE_PULSE1|TONE_MODE3)
uint32_t waiting[]{
    TRIANGLE_HUM(30,30,90),30,
    TRIANGLE_HUM(40,30,90),30,
    TRIANGLE_HUM(20,30,90),30,
    TRIANGLE_HUM(80,30,60),0,MENACING_DRUM,30,
    TRIANGLE_HUM(30,30,90),30,
    TRIANGLE_HUM(40,30,90),30,
    TRIANGLE_HUM(20,30,90),30,
    TRIANGLE_HUM(80,60,60),0,MENACING_DRUM,60,
    TRIANGLE_HUM(80,60,60),0,MENACING_DRUM,60,
    TRIANGLE_HUM(30,30,90),0,NOTE_SQ_LOW(100,30),
    30,
    TRIANGLE_HUM(40,30,90),0,NOTE_SQ_LOW(30,30),
    30,
    TRIANGLE_HUM(20,30,90),0,NOTE_SQ_LOW(60,30),
    30,
    TRIANGLE_HUM(80,30,60),0,MENACING_DRUM,
    30,
    TRIANGLE_HUM(30,30,90),0,NOTE_SQ_LOW(100,30),
    30,
    TRIANGLE_HUM(40,30,90),0,NOTE_SQ_LOW(30,30),
    30,
    TRIANGLE_HUM(20,30,90),0,NOTE_SQ_LOW(60,30),
    30,
    TRIANGLE_HUM(80,60,60),0,MENACING_DRUM,0,NOTE_SQ_LOW(60,30),
    60,
    TRIANGLE_HUM(80,60,60),0,MENACING_DRUM,0,NOTE_SQ_LOW(60,30),
};

#define KICK_1 SOUND_FROM(100,100,0,0,0,12,40,40,TONE_NOISE),0,SOUND_FROM(180,40,0,10,0,0,60,60,TONE_TRIANGLE)
#define SNARE_1 SOUND_FROM(350,345,0,0,10,15,60,20,TONE_NOISE),0,SOUND_FROM(180,40,0,10,0,0,60,60,TONE_TRIANGLE)
#define HAT_1 SOUND_FROM(700,700,0,0,5,10,40,20,TONE_NOISE)
/*#define SNARE_1 SOUND_FROM(400,380,0,0,5,10,40,10,TONE_NOISE)
#define SNARE_1_ACC SOUND_FROM(200,100,0,3,5,10,70,1,TONE_PULSE1|TONE_MODE3)
#define SNARE_1_FULL SNARE_1,0,SNARE_1_ACC*/

#define SQ_STRAIGHT(fr,sus,vol,chn) SOUND_FROM(fr,fr,0,0,sus,0,0,vol,(chn==0?TONE_PULSE1:TONE_PULSE2)|TONE_MODE2)
uint32_t smithereens[]={//70 frames per 
                        //Used for silly fights/fights that you easily win.
/*Basic drums
    KICK_1,
    20,
    HAT_1,
    10,
    SNARE_1,
    20,
    HAT_1,
    20,

    KICK_1,
    20,
    HAT_1,
    10,
    HAT_1,
    10,
    SNARE_1,
    10,
    HAT_1,
    20,
*/
/*more intense drums
    HAT_1,
    10,
    SNARE_1,
    10,
    HAT_1,
    10,
    KICK_1,
    10,
    HAT_1,
    10,
    SNARE_1,
    10,
    HAT_1,
    10,
*/

    KICK_1,
    20,
    HAT_1,
    10,
    SNARE_1,
    20,
    HAT_1,
    20,
    KICK_1,
    20,
    HAT_1,
    10,
    HAT_1,
    10,
    SNARE_1,
    10,
    HAT_1,
    20,
    KICK_1,
    20,
    HAT_1,
    10,
    SNARE_1,
    20,
    HAT_1,
    20,
    KICK_1,
    20,
    HAT_1,
    10,
    HAT_1,
    10,
    SNARE_1,
    10,
    HAT_1,
    20,
//end-> 2 loops without any other noise.
    KICK_1,0,SQ_STRAIGHT(450,70,20,1),
    20,
    HAT_1,
    10,
    SNARE_1,
    20,
    HAT_1,
    20,
    KICK_1,0,SQ_STRAIGHT(400,70,20,1),
    20,
    HAT_1,
    10,
    HAT_1,
    10,
    SNARE_1,
    10,
    HAT_1,
    20,
    KICK_1,0,SQ_STRAIGHT(350,70,20,1),
    20,
    HAT_1,
    10,
    SNARE_1,
    20,
    HAT_1,
    20,
    KICK_1,0,SQ_STRAIGHT(400,70,20,1),
    20,
    HAT_1,
    10,
    HAT_1,
    10,
    SNARE_1,
    10,
    HAT_1,
    20,
    KICK_1,0,SQ_STRAIGHT(450,70,20,1),
    20,
    HAT_1,
    10,
    SNARE_1,
    20,
    HAT_1,
    20,
    KICK_1,0,SQ_STRAIGHT(400,70,20,1),
    20,
    HAT_1,
    10,
    HAT_1,
    10,
    SNARE_1,
    10,
    HAT_1,
    20,
    KICK_1,0,SQ_STRAIGHT(350,70,20,1),
    20,
    HAT_1,
    10,
    SNARE_1,
    20,
    HAT_1,
    20,
    KICK_1,0,SQ_STRAIGHT(500,70,20,1),
    20,
    HAT_1,
    10,
    HAT_1,
    10,
    SNARE_1,
    10,
    HAT_1,
    20,
//the second square row enters.
    KICK_1,0,SQ_STRAIGHT(450,70,20,1),0,SQ_STRAIGHT(200,140,20,0),
    20,
    HAT_1,
    10,
    SNARE_1,
    20,
    HAT_1,
    20,
    KICK_1,0,SQ_STRAIGHT(400,70,20,1),
    20,
    HAT_1,
    10,
    HAT_1,
    10,
    SNARE_1,
    10,
    HAT_1,
    20,
    KICK_1,0,SQ_STRAIGHT(350,70,20,1),0,SQ_STRAIGHT(150,140,20,0),
    20,
    HAT_1,
    10,
    SNARE_1,
    20,
    HAT_1,
    20,
    KICK_1,0,SQ_STRAIGHT(400,70,20,1),
    20,
    HAT_1,
    10,
    HAT_1,
    10,
    SNARE_1,
    10,
    HAT_1,
    20,
    KICK_1,0,SQ_STRAIGHT(450,70,20,1),0,SQ_STRAIGHT(200,140,20,0),
    20,
    HAT_1,
    10,
    SNARE_1,
    20,
    HAT_1,
    20,
    KICK_1,0,SQ_STRAIGHT(400,70,20,1),
    20,
    HAT_1,
    10,
    HAT_1,
    10,
    SNARE_1,
    10,
    HAT_1,
    20,
    KICK_1,0,SQ_STRAIGHT(350,70,20,1),0,SQ_STRAIGHT(250,140,20,0),
    20,
    HAT_1,
    10,
    SNARE_1,
    20,
    HAT_1,
    20,
    KICK_1,0,SQ_STRAIGHT(500,70,20,1),
    20,
    HAT_1,
    10,
    HAT_1,
    10,
    SNARE_1,
    10,
    HAT_1,
    20,
    
    KICK_1,
    20,
    HAT_1,
    10,
    SNARE_1,
    20,
    HAT_1,
    20,

    KICK_1,
    20,
    HAT_1,
    10,
    HAT_1,
    10,
    SNARE_1,
    10,
    HAT_1,
    20,
    
    HAT_1,0,SQ_STRAIGHT(450,70,20,1),
    10,
    SNARE_1,
    10,
    HAT_1,
    10,
    KICK_1,
    10,
    HAT_1,
    10,
    SNARE_1,
    10,
    HAT_1,
    10,
    
    HAT_1,0,SQ_STRAIGHT(500,70,20,1),
    10,
    SNARE_1,
    10,
    HAT_1,
    10,
    KICK_1,
    10,
    HAT_1,
    10,
    SNARE_1,
    10,
    HAT_1,
    10,
};
#define KICK_2 SOUND_FROM(150,150,0,0,0,12,40,40,TONE_NOISE),0,SOUND_FROM(180,10,0,10,0,0,60,60,TONE_TRIANGLE)
#define KICK_2_NO_TRI SOUND_FROM(150,150,0,0,0,12,40,40,TONE_NOISE)
#define SNARE_1_NO_TRI SOUND_FROM(350,345,0,0,10,15,60,20,TONE_NOISE)
#define BECKON_BPM_SLOW 16
//I realize this is actually 1/BPM but shush
#define BECKON_BPM 12
#define SQ_BACK(fr,sus,vol,chn) SOUND_FROM(fr,fr,0,sus,sus/3,0,1,vol,(chn==0?TONE_PULSE1:TONE_PULSE2)|TONE_MODE3)
uint32_t beckon_slow[]={//4*8 per.

    KICK_2,0,SQ_BACK(50,30,80,1),BECKON_BPM_SLOW,
    KICK_2,BECKON_BPM_SLOW,
    SNARE_1,BECKON_BPM_SLOW,
    KICK_2,BECKON_BPM_SLOW,

    KICK_2,BECKON_BPM_SLOW,
    KICK_2,BECKON_BPM_SLOW,
    SNARE_1,BECKON_BPM_SLOW,
    HAT_1,BECKON_BPM_SLOW,

};
uint32_t beckon[]{
    KICK_2,0,SQ_BACK(50,30,80,1),BECKON_BPM,
    KICK_2/*,0,SQ_BACK(100,60,80,0)*/,BECKON_BPM,
    SNARE_1,BECKON_BPM,
    KICK_2,BECKON_BPM,
    KICK_2,BECKON_BPM,
    KICK_2,BECKON_BPM,
    SNARE_1,BECKON_BPM,
    HAT_1,BECKON_BPM,

    KICK_2,0,SQ_BACK(50,30,80,1),BECKON_BPM,
    KICK_2/*,0,SQ_BACK(180,60,80,0)*/,BECKON_BPM,
    SNARE_1,BECKON_BPM,
    KICK_2,BECKON_BPM,
    KICK_2,BECKON_BPM,
    KICK_2,BECKON_BPM,
    SNARE_1,BECKON_BPM,
    HAT_1,BECKON_BPM,

    KICK_2,0,SQ_BACK(50,30,80,1),BECKON_BPM,
    KICK_2,0,SQ_BACK(100,60,80,0),BECKON_BPM,
    SNARE_1,BECKON_BPM,
    KICK_2,BECKON_BPM,
    KICK_2,BECKON_BPM,
    KICK_2,BECKON_BPM,
    SNARE_1,BECKON_BPM,
    HAT_1,BECKON_BPM,

    KICK_2,0,SQ_BACK(90,30,80,1),BECKON_BPM,
    KICK_2,0,SQ_BACK(180,60,80,0),BECKON_BPM,
    SNARE_1,BECKON_BPM,
    KICK_2,BECKON_BPM,
    KICK_2,BECKON_BPM,
    KICK_2,BECKON_BPM,
    SNARE_1,BECKON_BPM,
    HAT_1,BECKON_BPM,

    KICK_2,0,SQ_BACK(50,30,80,1),BECKON_BPM,
    KICK_2,0,SQ_BACK(100,60,80,0),BECKON_BPM,
    SNARE_1,BECKON_BPM,
    KICK_2,BECKON_BPM,
    KICK_2,BECKON_BPM,
    KICK_2,BECKON_BPM,
    SNARE_1,BECKON_BPM,
    HAT_1,BECKON_BPM,

    KICK_2,0,SQ_BACK(90,30,80,1),BECKON_BPM,
    KICK_2,0,SQ_BACK(120,60,80,0),BECKON_BPM,
    SNARE_1,BECKON_BPM,
    KICK_2,BECKON_BPM,
    KICK_2,BECKON_BPM,
    KICK_2,BECKON_BPM,
    SNARE_1,BECKON_BPM,
    HAT_1,BECKON_BPM,
    
    KICK_2,0,SQ_BACK(50,30,80,1),BECKON_BPM,
    KICK_2/*,0,SQ_BACK(100,60,80,0)*/,BECKON_BPM,
    SNARE_1,BECKON_BPM,
    KICK_2,BECKON_BPM,
    KICK_2,BECKON_BPM,
    KICK_2,BECKON_BPM,
    SNARE_1,BECKON_BPM,
    HAT_1,BECKON_BPM,

    KICK_2,0,SQ_BACK(50,30,80,1),BECKON_BPM,
    KICK_2/*,0,SQ_BACK(180,60,80,0)*/,BECKON_BPM,
    SNARE_1,BECKON_BPM,
    KICK_2,BECKON_BPM,
    KICK_2,BECKON_BPM,
    KICK_2,BECKON_BPM,
    SNARE_1,BECKON_BPM,
    HAT_1,BECKON_BPM,

    KICK_2,0,SQ_BACK(50,30,80,1),BECKON_BPM,
    KICK_2,0,SQ_BACK(100,60,80,0),BECKON_BPM,
    SNARE_1,BECKON_BPM,
    KICK_2,BECKON_BPM,
    KICK_2,BECKON_BPM,
    KICK_2,BECKON_BPM,
    SNARE_1,BECKON_BPM,
    HAT_1,BECKON_BPM,

    KICK_2,0,SQ_BACK(90,30,80,1),BECKON_BPM,
    KICK_2,0,SQ_BACK(180,60,80,0),BECKON_BPM,
    SNARE_1,BECKON_BPM,
    KICK_2,BECKON_BPM,
    KICK_2,BECKON_BPM,
    KICK_2,BECKON_BPM,
    SNARE_1,BECKON_BPM,
    HAT_1,BECKON_BPM,

    KICK_2,0,SQ_BACK(50,30,80,1),BECKON_BPM,
    KICK_2,0,SQ_BACK(100,60,80,0),BECKON_BPM,
    SNARE_1,BECKON_BPM,
    KICK_2,BECKON_BPM,
    KICK_2,BECKON_BPM,
    KICK_2,BECKON_BPM,
    SNARE_1,BECKON_BPM,
    HAT_1,BECKON_BPM,

    KICK_2,0,SQ_BACK(90,30,80,1),BECKON_BPM,
    KICK_2,0,SQ_BACK(120,60,80,0),BECKON_BPM,
    SNARE_1,BECKON_BPM,
    KICK_2,BECKON_BPM,
    KICK_2,BECKON_BPM,
    KICK_2,BECKON_BPM,
    SNARE_1,BECKON_BPM,
    HAT_1,BECKON_BPM,
};
#define MARK_BPM 7
#define MARK_BPM2 14
#define HAT_2 SOUND_FROM(700,700,0,0,5,10,40,20,TONE_NOISE)
#define HAT_2_alt1 SOUND_FROM(600,600,0,0,5,10,40,20,TONE_NOISE)
#define HAT_2_alt2 SOUND_FROM(800,800,0,0,5,10,40,20,TONE_NOISE)
#define WAVES_1 SOUND_FROM(570,340,120,0,0,40,10,10,TONE_NOISE|TONE_MODE3)
#define SQ_BLEEP(fr,sus,vol,chn) SOUND_FROM(fr,fr,sus,sus/3,0,sus,1,vol,(chn==0?TONE_PULSE1:TONE_PULSE2)|TONE_MODE3)

uint32_t mark[]={
    SQ_STRAIGHT(300,15,60,0),0,KICK_2,MARK_BPM2,
    SQ_STRAIGHT(200,15,60,0),0,HAT_2,MARK_BPM,
    SQ_STRAIGHT(100,15,60,0),0,KICK_2,MARK_BPM,
    SQ_STRAIGHT(400,15,60,0),0,SNARE_1,MARK_BPM2,
    SQ_STRAIGHT(300,15,60,0),0,SNARE_1,MARK_BPM2,

    SQ_STRAIGHT(300,15,60,0),0,KICK_2,MARK_BPM2,
    SQ_STRAIGHT(200,15,60,0),0,HAT_2,MARK_BPM,
    SQ_STRAIGHT(100,15,60,0),0,KICK_2,MARK_BPM,
    SQ_STRAIGHT(400,15,60,0),0,SNARE_1,MARK_BPM2,
    SQ_STRAIGHT(200,15,60,0),0,SNARE_1,MARK_BPM2,

    SQ_STRAIGHT(200,15,60,0),0,KICK_2,MARK_BPM2,
    SQ_STRAIGHT(100,15,60,0),0,HAT_2,MARK_BPM,
    SQ_STRAIGHT(400,15,60,0),0,KICK_2,MARK_BPM,
    SQ_STRAIGHT(300,15,60,0),0,SNARE_1,MARK_BPM2,
    SQ_STRAIGHT(100,15,60,0),0,SNARE_1,MARK_BPM2,

    SQ_STRAIGHT(200,15,60,0),0,KICK_2,MARK_BPM2,
    SQ_STRAIGHT(200,15,60,0),0,HAT_2,MARK_BPM,
    SQ_STRAIGHT(200,15,60,0),0,SNARE_1,MARK_BPM2,
    SQ_STRAIGHT(200,15,60,0),0,HAT_2,MARK_BPM,
    SQ_STRAIGHT(200,15,60,0),0,SNARE_1,MARK_BPM2,

    //rd 2
    SQ_STRAIGHT(100,15,60,0),0,KICK_2,MARK_BPM2,
    SQ_STRAIGHT(400,15,60,0),0,HAT_2,MARK_BPM,
    SQ_STRAIGHT(100,15,60,0),0,KICK_2,MARK_BPM,
    SQ_STRAIGHT(400,15,60,0),0,SNARE_1,MARK_BPM2,
    SQ_STRAIGHT(100,15,60,0),0,SNARE_1,MARK_BPM2,

    SQ_STRAIGHT(400,15,60,0),0,KICK_2,MARK_BPM2,
    SQ_STRAIGHT(100,15,60,0),0,HAT_2,MARK_BPM,
    SQ_STRAIGHT(400,15,60,0),0,KICK_2,MARK_BPM,
    SQ_STRAIGHT(100,15,60,0),0,SNARE_1,MARK_BPM2,
    SQ_STRAIGHT(400,15,60,0),0,SNARE_1,MARK_BPM2,

    SQ_STRAIGHT(150,15,60,0),0,KICK_2,MARK_BPM2,
    SQ_STRAIGHT(500,15,60,0),0,HAT_2,MARK_BPM,
    SQ_STRAIGHT(150,15,60,0),0,KICK_2,MARK_BPM,
    SQ_STRAIGHT(500,15,60,0),0,SNARE_1,MARK_BPM2,
    SQ_STRAIGHT(150,15,60,0),0,SNARE_1,MARK_BPM2,

    SQ_STRAIGHT(500,15,60,0),0,KICK_2,MARK_BPM2,
    SQ_STRAIGHT(150,15,60,0),0,HAT_2,MARK_BPM,
    SQ_STRAIGHT(500,15,60,0),0,SNARE_1,MARK_BPM2,
    SQ_STRAIGHT(150,15,60,0),0,HAT_2,MARK_BPM,
    SQ_STRAIGHT(500,15,60,0),0,SNARE_1,MARK_BPM2,

    SQ_STRAIGHT(100,15,60,0),0,KICK_2,MARK_BPM2,
    SQ_STRAIGHT(400,15,60,0),0,HAT_2,MARK_BPM,
    SQ_STRAIGHT(100,15,60,0),0,KICK_2,MARK_BPM,
    SQ_STRAIGHT(400,15,60,0),0,SNARE_1,MARK_BPM2,
    SQ_STRAIGHT(100,15,60,0),0,SNARE_1,MARK_BPM2,

    SQ_STRAIGHT(400,15,60,0),0,KICK_2,MARK_BPM2,
    SQ_STRAIGHT(100,15,60,0),0,HAT_2,MARK_BPM,
    SQ_STRAIGHT(400,15,60,0),0,SNARE_1,MARK_BPM2,
    SQ_STRAIGHT(100,15,60,0),0,HAT_2,MARK_BPM,
    SQ_STRAIGHT(400,15,60,0),0,SNARE_1,MARK_BPM2,

    SQ_STRAIGHT(50,15,60,0),0,KICK_2,MARK_BPM2,
    SQ_STRAIGHT(300,15,60,0),0,HAT_2,MARK_BPM,
    SQ_STRAIGHT(50,15,60,0),0,KICK_2,MARK_BPM,
    SQ_STRAIGHT(300,15,60,0),0,SNARE_1,MARK_BPM2,
    SQ_STRAIGHT(50,15,60,0),0,SNARE_1,MARK_BPM2,

    SQ_STRAIGHT(300,15,60,0),0,KICK_2,MARK_BPM2,
    SQ_STRAIGHT(50,15,60,0),0,HAT_2,MARK_BPM,
    SQ_STRAIGHT(300,15,60,0),0,SNARE_1,MARK_BPM2,
    SQ_STRAIGHT(50,15,60,0),0,HAT_2,MARK_BPM,
    SQ_STRAIGHT(300,15,60,0),0,SNARE_1,MARK_BPM2,

    //rd 3
    SQ_STRAIGHT(300,15,60,0),0,KICK_2,MARK_BPM2,
    SQ_STRAIGHT(200,15,60,0),0,HAT_2,MARK_BPM,
    SQ_STRAIGHT(100,15,60,0),0,KICK_2,MARK_BPM,
    SQ_STRAIGHT(400,15,60,0),0,SNARE_1,MARK_BPM2,
    SQ_STRAIGHT(300,15,60,0),0,SNARE_1,MARK_BPM2,

    SQ_STRAIGHT(300,15,60,0),0,KICK_2,MARK_BPM2,
    SQ_STRAIGHT(200,15,60,0),0,HAT_2,MARK_BPM,
    SQ_STRAIGHT(100,15,60,0),0,KICK_2,MARK_BPM,
    SQ_STRAIGHT(400,15,60,0),0,SNARE_1,MARK_BPM2,
    SQ_STRAIGHT(200,15,60,0),0,SNARE_1,MARK_BPM2,

    SQ_STRAIGHT(200,15,60,0),0,KICK_2,MARK_BPM2,
    SQ_STRAIGHT(100,15,60,0),0,HAT_2,MARK_BPM,
    SQ_STRAIGHT(400,15,60,0),0,KICK_2,MARK_BPM,
    SQ_STRAIGHT(300,15,60,0),0,SNARE_1,MARK_BPM2,
    SQ_STRAIGHT(100,15,60,0),0,SNARE_1,MARK_BPM2,
    
    SQ_STRAIGHT(200,15,60,0),0,KICK_2,MARK_BPM2,
    SQ_STRAIGHT(200,15,60,0),0,HAT_2,MARK_BPM,
    SQ_STRAIGHT(200,15,60,0),0,SNARE_1,MARK_BPM2,
    SQ_STRAIGHT(200,15,60,0),0,HAT_2,MARK_BPM,
    SQ_STRAIGHT(200,15,60,0),0,SNARE_1,MARK_BPM2,

    //rd 4
    SQ_STRAIGHT(100,15,60,0),0,KICK_2,0,SQ_STRAIGHT(600,3*MARK_BPM2,40,1),MARK_BPM2,
    SQ_STRAIGHT(400,15,60,0),0,HAT_2,MARK_BPM,
    SQ_STRAIGHT(100,15,60,0),0,KICK_2,MARK_BPM,
    SQ_STRAIGHT(400,15,60,0),0,SNARE_1,MARK_BPM2,
    SQ_STRAIGHT(100,15,60,0),0,SNARE_1,MARK_BPM2,

    SQ_STRAIGHT(400,15,60,0),0,KICK_2,0,SQ_STRAIGHT(500,3*MARK_BPM2,40,1),MARK_BPM2,
    SQ_STRAIGHT(100,15,60,0),0,HAT_2,MARK_BPM,
    SQ_STRAIGHT(400,15,60,0),0,KICK_2,MARK_BPM,
    SQ_STRAIGHT(100,15,60,0),0,SNARE_1,MARK_BPM2,
    SQ_STRAIGHT(400,15,60,0),0,SNARE_1,MARK_BPM2,

    SQ_STRAIGHT(150,15,60,0),0,KICK_2,0,SQ_STRAIGHT(700,3*MARK_BPM2,40,1),MARK_BPM2,
    SQ_STRAIGHT(500,15,60,0),0,HAT_2,MARK_BPM,
    SQ_STRAIGHT(150,15,60,0),0,KICK_2,MARK_BPM,
    SQ_STRAIGHT(500,15,60,0),0,SNARE_1,MARK_BPM2,
    SQ_STRAIGHT(150,15,60,0),0,SNARE_1,MARK_BPM2,

    SQ_STRAIGHT(500,15,60,0),0,KICK_2,0,SQ_STRAIGHT(800,3*MARK_BPM2,40,1),MARK_BPM2,
    SQ_STRAIGHT(150,15,60,0),0,HAT_2,MARK_BPM,
    SQ_STRAIGHT(500,15,60,0),0,SNARE_1,MARK_BPM2,
    SQ_STRAIGHT(150,15,60,0),0,HAT_2,MARK_BPM,
    SQ_STRAIGHT(500,15,60,0),0,SNARE_1,MARK_BPM2,

    SQ_STRAIGHT(100,15,60,0),0,KICK_2,0,SQ_STRAIGHT(600,3*MARK_BPM2,40,1),MARK_BPM2,
    SQ_STRAIGHT(400,15,60,0),0,HAT_2,MARK_BPM,
    SQ_STRAIGHT(100,15,60,0),0,KICK_2,MARK_BPM,
    SQ_STRAIGHT(400,15,60,0),0,SNARE_1,MARK_BPM2,
    SQ_STRAIGHT(100,15,60,0),0,SNARE_1,MARK_BPM2,

    SQ_STRAIGHT(400,15,60,0),0,KICK_2,0,SQ_STRAIGHT(400,3*MARK_BPM2,40,1),MARK_BPM2,
    SQ_STRAIGHT(100,15,60,0),0,HAT_2,MARK_BPM,
    SQ_STRAIGHT(400,15,60,0),0,SNARE_1,MARK_BPM2,
    SQ_STRAIGHT(100,15,60,0),0,HAT_2,MARK_BPM,
    SQ_STRAIGHT(400,15,60,0),0,SNARE_1,MARK_BPM2,

    SQ_STRAIGHT(50,15,60,0),0,KICK_2,0,SQ_STRAIGHT(800,3*MARK_BPM2,40,1),MARK_BPM2,
    SQ_STRAIGHT(300,15,60,0),0,HAT_2,MARK_BPM,
    SQ_STRAIGHT(50,15,60,0),0,KICK_2,MARK_BPM,
    SQ_STRAIGHT(300,15,60,0),0,SNARE_1,MARK_BPM2,
    SQ_STRAIGHT(50,15,60,0),0,SNARE_1,MARK_BPM2,

    SQ_STRAIGHT(300,15,60,0),0,KICK_2,0,SQ_STRAIGHT(600,3*MARK_BPM2,40,1),MARK_BPM2,
    SQ_STRAIGHT(50,15,60,0),0,HAT_2,MARK_BPM,
    SQ_STRAIGHT(300,15,60,0),0,SNARE_1,MARK_BPM2,
    SQ_STRAIGHT(50,15,60,0),0,HAT_2,MARK_BPM,
    SQ_STRAIGHT(300,15,60,0),0,SNARE_1,MARK_BPM2,
};
