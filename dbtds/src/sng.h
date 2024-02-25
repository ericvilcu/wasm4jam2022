
//FAIR WARNING: beyond here lies a land of programming hacks, macros, and strange code.
//              I do NOT recommend trying to understand it.

#pragma once
//NOTE: contains data despite being a header.
#include <stdint.h>
#include "wasm4.h"
#define byteAlgin 4
#define byteAlginMask ((1<<byteAlgin)-1) 
#define POSOFFULL(song) ((int)((uint8_t*)&RawNotes::song-START_NOTE))
#define POSOF(song) ((uint8_t*)&RawNotes::song!=(uint8_t*)&RawNotes::configDrum?((uint8_t)((POSOFFULL(song)>>4)+1)):(uint8_t)0)
#define FROMPOS(dat) ((uint8_t*)(START_NOTE+(((int)((dat)-1))<<4)))
#define ValidateTrackSize(ts) static_assert((sizeof(ts)&((1<<byteAlgin)-1))==0)
#define POSOFTRK(song) ((uint8_t)((uint8_t*)TrackData::song::c-(uint8_t*)START_TRACK))
#define FROMPOSTRK(dat) ((uint8_t*)((uint8_t*)START_TRACK+(dat)))

#define FREQ(freq) ((uint8_t)(freq))
#define HOLD(freq) ((uint8_t)(128|(freq)))
#define NONOTE 0

#define SOUND_FROM(f1,f2,atk,decay,sus,rel,peak,volume,flags) f1+0x10000*(uint32_t)(f2),((uint32_t)(atk)<<24)+((uint32_t)(decay)<<16)+(uint32_t)(sus)+((uint32_t)(rel)<<8),(peak<<8)+volume,flags

#define KICK_1 SOUND_FROM(100,100,0,0,0,12,40,40,TONE_NOISE),SOUND_FROM(180,40,0,10,0,0,60,60,TONE_TRIANGLE)
#define SNARE_1 SOUND_FROM(350,345,0,0,10,15,60,20,TONE_NOISE),SOUND_FROM(180,40,0,10,0,0,60,60,TONE_TRIANGLE)
#define HAT_1 SOUND_FROM(700,700,0,0,5,10,40,20,TONE_NOISE),0,0,0,0


enum SNG_FLAGS{
    f_S=1<<7,
    f_s=1<<6,
    f_P=1<<5,
    f_N=1<<4,
    f_L=1<<3,
    f_3=1<<2,
    f_2=1<<1,
    f_1=1<<0,
};
namespace RawNotes{
    enum configDrum{
        KICK=1,
        SNARE=2,
        HAT=3,
    };
    uint32_t configDrum[]={
        KICK_1,
        SNARE_1,
        HAT_1,
    };
    #define START_NOTE ((uint8_t*)&RawNotes::beep1)
    #define NO_NOTE configDrum
    //Captain's songs
    uint8_t beep1[]={
        FREQ(25),FREQ(30),FREQ(40),NONOTE,
        FREQ(25),FREQ(30),FREQ(40),NONOTE,
        FREQ(35),FREQ(40),FREQ(50),NONOTE,
        FREQ(35),FREQ(40),FREQ(60),NONOTE,
    };
    ValidateTrackSize(beep1);
    uint8_t drone1[]={
        HOLD(10),HOLD(10),HOLD(10),HOLD(10),
        HOLD(20),HOLD(20),HOLD(20),HOLD(20),
        HOLD(20),HOLD(20),HOLD(20),HOLD(20),
        HOLD( 5),HOLD( 5),HOLD( 5),HOLD( 5),
    };
    ValidateTrackSize(drone1);
    uint8_t kicking1[]={
        KICK  ,NONOTE,NONOTE,  KICK,
        KICK  ,NONOTE,NONOTE,  KICK,
        KICK  ,NONOTE,  KICK,NONOTE,
        SNARE ,  KICK,  KICK,   HAT,
    };
    ValidateTrackSize(kicking1);
    uint8_t kicking2[]={
        KICK  ,   KICK,  KICK,  KICK,
        SNARE , NONOTE,NONOTE, SNARE,
        KICK  ,  SNARE,  KICK, SNARE,
        SNARE ,    HAT,   HAT,   HAT,
    };
    ValidateTrackSize(kicking2);
    uint8_t break1[]={
        FREQ(25),FREQ(30),FREQ(40),FREQ(25),
        FREQ(35),FREQ(40),FREQ(50),FREQ(35),
        FREQ(25),FREQ(30),FREQ(40),FREQ(25),
        FREQ(35),FREQ(40),FREQ(35),FREQ(25),
    };
    ValidateTrackSize(break1);

    //PvP songs
    
    uint8_t pvp321[]={
        HOLD(35),HOLD(35),NONOTE,NONOTE,
        HOLD(40),HOLD(40),NONOTE,NONOTE,
        HOLD(50),HOLD(50),HOLD(50),NONOTE,
        HOLD(70),HOLD(70),HOLD(70),HOLD(70),
    };
    ValidateTrackSize(pvp321);
    
    uint8_t beep_humble[]={
        FREQ(25),NONOTE,FREQ(25),NONOTE,
        FREQ(45),NONOTE,FREQ(25),NONOTE,
        FREQ(25),NONOTE,FREQ(25),NONOTE,
        FREQ(55),NONOTE,NONOTE  ,NONOTE,
    };
    ValidateTrackSize(beep_humble);

    uint8_t heartbeat[]={
        KICK  ,NONOTE,NONOTE,NONOTE,
        SNARE ,NONOTE,NONOTE,NONOTE,
        KICK  ,NONOTE  ,KICK,  KICK,
        SNARE ,NONOTE,NONOTE,NONOTE,
    };
    ValidateTrackSize(heartbeat);
    
    uint8_t drone2[]={
        FREQ( 5),FREQ( 5),FREQ( 5),FREQ( 5),
        FREQ( 5),FREQ( 5),FREQ( 5),FREQ( 5),
        FREQ( 5),FREQ( 5),FREQ( 5),FREQ( 5),
        FREQ(15),FREQ(15),FREQ(15),FREQ(15),
    };
    ValidateTrackSize(drone2);
};

namespace TrackData{
    #define fork2(name,a,b,yesFlag,noFlag) namespace name{\
        uint8_t c[]={\
            0b10000000+(noFlag)+((yesFlag)<<3),\
            POSOFTRK(a),\
            POSOFTRK(b) \
            };\
    };
    #define track3NOINIT(name) namespace name{\
        extern uint8_t c[5];\
    };
    #define track3(name,s1,s2,drums,next,size) namespace name{\
        uint8_t c[]={\
            POSOF(s1),\
            POSOF(s2),\
            POSOF(drums),\
            size,\
            POSOFTRK(next)\
        };\
    };
    #define START_TRACK (&TrackData::captain_slow::c)
    
    //Captain's pride
    track3NOINIT(captain_slow);
    track3(captain_slow,beep1,drone1,kicking1,captain_slow,16);
    
    //Captain's fear/brawn
    track3NOINIT(captain_fast);
    track3NOINIT(captain_breakdown);
    fork2(intensityCheck,captain_breakdown,captain_fast,f_1,f_3);
    track3(captain_breakdown,break1,drone1,kicking2,captain_fast,16);
    track3(captain_fast,beep1,drone1,kicking1,intensityCheck,16);
    
    //PvP
    track3NOINIT(PvP_humble);
    track3NOINIT(PvP_grudge);
    fork2(PvPLoop2,PvP_grudge,PvP_grudge,0,f_2|f_3);
    fork2(PvPLoop1,PvPLoop2,PvP_humble,f_1,0);
    track3(PvP_humble,NO_NOTE,drone2,heartbeat,PvPLoop1,16);
    track3(PvP_grudge,beep_humble,drone2,heartbeat,PvP_humble,16);

    track3(PvP_start,pvp321,pvp321,NO_NOTE,PvP_humble,16);
    
    
    
    
    enum Type:uint8_t{FORK=1,TRACK3=0};
    uint8_t type(void* dat){
        //the very first bit
        return (Type)((*(int8_t*)dat)&128)>>7;
    }
    void* process_fork(void* dat, uint8_t flags){
        int8_t*data = (int8_t*) dat;
        tracef("%f %f %f %f",(float)(flags&3),(float)(7&(*(int8_t*)dat)),(float)(7&((*(int8_t*)dat)>>3)),
        (float)(int)((flags&7&(*(int8_t*)dat))==0 && (flags&(7&((*(int8_t*)dat)>>3))) != 0));
        if((flags&7&(*(int8_t*)dat))==0 && (flags&(7&((*(int8_t*)dat)>>3))) != 0)
            return FROMPOSTRK(data[1]);
        return FROMPOSTRK(data[2]);
    }
    uint32_t VOLFORFR(uint32_t data){
        return (data>100?60:40);
    }
    void* process_track3(void* dat, uint8_t flags, uint8_t& position){
        unsigned int durFlag = (flags & f_L)?10:20;
        if((unsigned int)frame%durFlag!=0) return dat;
        int8_t*data = (int8_t*) dat;
        auto len = data[3];
        //For square waves, just get the data.
        //TODO? volume adjustment?
        if(data[0]!=0){
            auto square1 = FROMPOS(data[0]);
            uint32_t realFreq = (uint32_t)(square1[position]&127)*7;
            bool hold = (square1[position]&128)>0;
            if((flags&f_s)&&realFreq!=0)tone(realFreq,durFlag+(hold?1:0),VOLFORFR(realFreq),TONE_PULSE1);
        }
        if(data[1]!=0){
            auto square2 = FROMPOS(data[1]);
            uint32_t realFreq = (uint32_t)(square2[position]&127)*7;
            bool hold = (square2[position]&128)>0;
            if((flags&f_S)&&realFreq!=0)tone(realFreq,durFlag+(hold?1:0),VOLFORFR(realFreq),TONE_PULSE2|TONE_MODE4);
        }
        //for drums, we have a segment for that.
        if(data[2]!=0){
            int idx = FROMPOS(data[2])[position];
            if(idx>0){
                idx--;
                if(flags&f_N)tone(RawNotes::configDrum[idx*8+0],RawNotes::configDrum[idx*8+1],RawNotes::configDrum[idx*8+2],RawNotes::configDrum[idx*8+3]);
                if(flags&f_P)tone(RawNotes::configDrum[idx*8+4],RawNotes::configDrum[idx*8+5],RawNotes::configDrum[idx*8+6],RawNotes::configDrum[idx*8+7]);
            }
        }
        position++;
        //tracef("%f %f %f %f",(float)POSOF(beep1),(float)data[1],(float)(int)FROMPOS(data[1]),(float)(int)RawNotes::beep1);
        //tracef("%f %f %f %f",(float)POSOF(kicking1),(float)data[2],(float)(int)FROMPOS(data[2]),(float)(int)RawNotes::kicking1);
        //tracef("%f %f %f",(float)len,(float)position,(float)FROMPOS(data[2])[0]);
        //tracef("%f %f",(float)(int)data,(float)(int)captain_slow::c);
        
        if(position>=len){
            position = 0;
            return FROMPOSTRK(data[4]);
        }
        return dat;
    }
};

namespace SongData{
    #define song(name, flags, id) \
        ((uint16_t) /*data =*/ ((flags)<<8|(POSOFTRK(id))))
    uint16_t songs[]={
        song(captain_pride,f_S|f_s|f_P|f_N,captain_slow),
        song(captain_fears,f_S|f_P|f_N|f_L,captain_fast),
        song(captain_fights,f_S|f_s|f_P|f_N|f_L,captain_fast),
        song(PvP,f_S|f_s|f_P|f_N|f_L,PvP_start),
    };
};

#define NOTE_SQ2(fr,vol) SOUND_FROM(fr,fr,0,0,5,20,1,vol,TONE_PULSE1|TONE_MODE1)
