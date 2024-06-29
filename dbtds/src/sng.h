
//FAIR WARNING: beyond here lies a land of programming hacks, macros, and strange code.
//              I do NOT recommend trying to understand it.

#pragma once
//NOTE: contains data despite being a header.
#include <stdint.h>
#include "wasm4.h"
namespace song
{
    extern uint8_t occupancy[4];
} // namespace song

#define byteAlgin 4
#define byteAlginMask ((1<<byteAlgin)-1) 
#define POSOFFULL(song) ((int)((uint8_t*)&RawNotes::song-START_NOTE))
#define POSOF(song) ((uint8_t*)&RawNotes::song!=(uint8_t*)&RawNotes::configDrum?((uint8_t)((POSOFFULL(song)>>4)+1)):(uint8_t)0)
#define FROMPOS(dat) ((uint8_t*)(START_NOTE+(((int)((dat)-1))<<4)))
#define ValidateTrackSize(ts) static_assert((sizeof(ts)&((1<<byteAlgin)-1))==0)
#define POSOFTRK(song) ((uint8_t)((uint8_t*)TrackData::song::c-(uint8_t*)START_TRACK))
#define FROMPOSTRK(dat) ((uint8_t*)((uint8_t*)START_TRACK+(dat)))

#define FREQ(freq) ((uint8_t)(freq/5))
#define HOLD(freq) ((uint8_t)(128|FREQ(freq)))
#define NONOTE 0

#define SOUND_FROM(f1,f2,atk,decay,sus,rel,peak,volume,flags) f1+0x10000*(uint32_t)(f2),((uint32_t)(atk)<<24)+((uint32_t)(decay)<<16)+(uint32_t)(sus)+((uint32_t)(rel)<<8),(peak<<8)+volume,flags

#define KICK_1 SOUND_FROM(100,100,0,0,0,12,40,40,TONE_NOISE),SOUND_FROM(180,40,0,10,0,0,60,60,TONE_TRIANGLE)
#define KICK_2 SOUND_FROM(150,150,0,0,0,12,40,40,TONE_NOISE),SOUND_FROM(180,10,0,10,0,0,60,60,TONE_TRIANGLE)
#define SNARE_1 SOUND_FROM(350,345,0,0,10,15,60,20,TONE_NOISE),SOUND_FROM(180,40,0,10,0,0,60,60,TONE_TRIANGLE)
#define HAT_1 SOUND_FROM(700,700,0,0,5,10,40,20,TONE_NOISE),SOUND_FROM(700,700,0,0,5,10,40,20,TONE_NOISE)
#define SNARE_2 SOUND_FROM(350,345,0,0,10,15,60,20,TONE_NOISE),SOUND_FROM(180,40,0,10,0,0,60,60,TONE_TRIANGLE)
#define HAT_2 SOUND_FROM(700,700,0,0,5,10,40,20,TONE_NOISE),SOUND_FROM(700,700,0,0,5,10,40,20,TONE_NOISE)


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
        KICK2=4,
    };
    uint32_t configDrum[]={
        KICK_1,
        SNARE_1,
        HAT_1,
        KICK_2,
        SNARE_2,
        HAT_2,
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
        FREQ(35),FREQ(40),FREQ(60),FREQ(35),
    };
    ValidateTrackSize(break1);

    //PvP songs
    
    uint8_t pvp321[]={
        HOLD(35),FREQ(35),NONOTE,NONOTE,
        HOLD(40),FREQ(40),NONOTE,NONOTE,
        HOLD(50),HOLD(50),FREQ(50),NONOTE,
        HOLD(70),HOLD(70),HOLD(70),FREQ(70),
    };
    ValidateTrackSize(pvp321);

    uint8_t heartbeat[]={
        KICK  ,NONOTE,NONOTE,NONOTE,
        SNARE ,NONOTE,NONOTE,NONOTE,
        KICK  ,NONOTE  ,KICK,  KICK,
        SNARE ,NONOTE,NONOTE,NONOTE,
    };
    ValidateTrackSize(heartbeat);
    
    uint8_t heartbeat2[]={
        KICK  ,NONOTE,NONOTE,  KICK,
        SNARE ,NONOTE,SNARE ,  KICK,
        KICK  ,KICK  ,SNARE ,  KICK,
        SNARE ,NONOTE,NONOTE,NONOTE,
    };
    ValidateTrackSize(heartbeat2);

    uint8_t drone2[]={
        FREQ( 5),FREQ( 5),FREQ( 5),FREQ( 5),
        FREQ( 5),FREQ( 5),FREQ( 5),FREQ( 5),
        FREQ( 5),FREQ( 5),FREQ( 5),FREQ( 5),
        FREQ(15),FREQ(15),FREQ(15),FREQ(15),
    };
    ValidateTrackSize(drone2);
    uint8_t drone2Variant[]={
        FREQ( 5),FREQ( 5),FREQ(15),FREQ(15),
        FREQ( 5),FREQ( 5),FREQ( 5),FREQ( 5),
        FREQ(15),FREQ(15),FREQ( 5),FREQ( 5),
        FREQ(15),FREQ(15),FREQ(15),FREQ(15),
    };
    ValidateTrackSize(drone2Variant);
    uint8_t drone2Variant2[]={
        FREQ( 5),FREQ( 5),HOLD(15),FREQ(15),
        FREQ( 5),FREQ( 5),FREQ( 5),FREQ( 5),
        HOLD(15),FREQ(15),FREQ( 5),FREQ( 5),
        FREQ(20),FREQ(20),FREQ(20),FREQ(20),
    };
    ValidateTrackSize(drone2Variant2);
    

    uint8_t heartbeat_alt[]={
        KICK  ,NONOTE ,HAT,NONOTE,
        KICK  ,NONOTE ,HAT,NONOTE,
        KICK  ,KICK2  ,HAT,HAT,
        KICK  ,KICK2  ,HAT,HAT,
    };
    ValidateTrackSize(heartbeat_alt);

    uint8_t heartbeat_stroke[]={
        KICK  ,NONOTE ,HAT   ,NONOTE,
        KICK  ,NONOTE ,HAT   ,NONOTE,
        KICK  ,KICK2  ,SNARE ,HAT   ,
        KICK  ,KICK2  ,HAT   ,SNARE ,
    };
    ValidateTrackSize(heartbeat_stroke);
    
    uint8_t skyMe[]={
        HOLD(35),FREQ(35),NONOTE,NONOTE,
        HOLD(40),FREQ(40),NONOTE,NONOTE,
        HOLD(35),FREQ(35),NONOTE,NONOTE,
        HOLD(40),FREQ(40),NONOTE,NONOTE,
        HOLD(35),FREQ(35),NONOTE,NONOTE,
        HOLD(50),HOLD(50),FREQ(50),NONOTE,
        HOLD(35),HOLD(35),NONOTE,NONOTE,
        HOLD(70),HOLD(70),HOLD(70),FREQ(70),
    };
    ValidateTrackSize(skyMe);
};

namespace TrackData{
    enum segmentFlags{
        F_MUTED =1,//lowers volume for everything
        F_MIXIN =2,//bumps down the frequency of square 2
        F_LENGTH=4,//doubles length. this can jump boundaries.
        TODO2=8,   //
        TODO3=16,  //
        TODO4=32,  //
        TODO5=64,  //
        TODO6=128, //
    };

    #define fork2(name,a,b,yesFlag,noFlag) namespace name{\
        uint8_t c[]={\
            0b10000000|(noFlag)|((yesFlag)<<3),\
            POSOFTRK(a),\
            POSOFTRK(b) \
            };\
    };
    #define track3NOINIT(name) namespace name{\
        extern uint8_t c[5];\
    };
    #define track3(name,s1,s2,drums,next,flags) namespace name{\
        uint8_t c[]={\
            POSOF(s1),\
            POSOF(s2),\
            POSOF(drums),\
            flags,\
            POSOFTRK(next)\
        };\
    };
    #define START_TRACK (&TrackData::captain_slow::c)
    
    //Captain's pride
    track3NOINIT(captain_slow);
    track3(captain_slow,beep1,drone1,kicking1,captain_slow,0);
    
    //Captain's fear/brawn
    track3NOINIT(captain_fast);
    track3NOINIT(captain_breakdown);
    fork2(intensityCheck,captain_breakdown,captain_fast,f_2,0);
    track3(captain_breakdown,break1,drone1,kicking2,captain_fast,0);
    track3(captain_fast,beep1,drone1,kicking1,intensityCheck,0);
    
    //PvP
    track3NOINIT(PvP_humble1);
    track3NOINIT(PvP_humble2);
    track3NOINIT(PvP_transition);
    track3NOINIT(PvP_grudge);
    track3NOINIT(PvP_waning);
    track3NOINIT(PvP_grudge2);
    track3NOINIT(PvP_waning2);
    
    fork2(PvPLoopGrudge1,PvP_waning,PvP_grudge,0,f_2|f_3);
    fork2(PvPLoopGrudge2,PvP_waning2,PvP_grudge2,0,f_2|f_3);
    fork2(PvPLoopHumble1,PvP_transition,PvP_humble1,f_1,0);
    fork2(PvPLoopHumble2,PvP_transition,PvP_humble2,f_1,0);

    track3(PvP_humble1,NO_NOTE,drone2,heartbeat,PvPLoopHumble2,0);
    track3(PvP_humble2,NO_NOTE,drone2Variant,heartbeat,PvPLoopHumble1,0);
    track3(PvP_transition,NO_NOTE,NO_NOTE,heartbeat2,PvPLoopGrudge1,0);
    track3(PvP_grudge,drone2Variant,drone2,heartbeat,PvPLoopGrudge2,0);
    track3(PvP_waning,drone2,drone2Variant,NO_NOTE,PvPLoopGrudge2,0);
    track3(PvP_grudge2,drone2Variant2,drone2,heartbeat,PvPLoopGrudge1,0);
    track3(PvP_waning2,drone2,drone2Variant2,NO_NOTE,PvPLoopGrudge1,0);

    track3(PvP_start,pvp321,pvp321,NO_NOTE,PvP_humble1,F_MIXIN);
    
    //PvP 2
    track3NOINIT(PvP2_humble1);
    track3NOINIT(PvP2_humble2);
    track3NOINIT(PvP2_focus);
    track3NOINIT(PvP2_panic);
    track3NOINIT(PvP2_transition);

    fork2(PvP2LoopFocus,PvP2_focus,PvP2_panic,0,f_2|f_3);
    fork2(PvP2LoopHumble,PvP2_transition,PvP2_humble1,f_1,0);

    track3(PvP2_humble1,drone2,NO_NOTE,heartbeat_alt,PvP2_humble2,0);
    track3(PvP2_humble2,drone2Variant,NO_NOTE,heartbeat_stroke,PvP2LoopHumble,0);

    track3(PvP2_transition,NO_NOTE,NO_NOTE,drone2Variant,PvP2LoopFocus,0);
    
    track3(PvP2_focus,skyMe,drone2,heartbeat_alt,PvP2LoopFocus,F_LENGTH);
    track3(PvP2_panic,drone2,drone2,heartbeat_alt,PvP2LoopFocus,F_LENGTH|F_MIXIN);
    
    //track3(PvP2_humble1,NO_NOTE,NO_NOTE,heartbeat2,PvP2_humble2,0);
    //track3(PvP2_humble2,NO_NOTE,NO_NOTE,heartbeat_alt,PvP2_transition1,0);
    track3(PvP2_start,pvp321,pvp321,NO_NOTE,PvP2_humble1,F_MIXIN);
    
    enum Type:uint8_t{FORK=1,TRACK3=0};

    uint8_t type(void* dat){
        //the very first bit
        return (Type)((*(int8_t*)dat)&128)>>7;
    }

    void* process_fork(void* dat, uint8_t flags){
        int8_t*data = (int8_t*) dat;
        int8_t yesFlags = 7&(data[0]>>3);
        int8_t noFlags = 7&data[0];
        if((flags&noFlags)==0 && (flags&yesFlags)==yesFlags)
            return FROMPOSTRK(data[1]);
        return FROMPOSTRK(data[2]);
    }

    uint32_t VOLUME_FOR(uint32_t data){
        //TODO: do properly
        return (data>100?60:40);
    }

    void* process_track3(void* dat, uint8_t flags, uint8_t& position){
        unsigned int durFlag = (flags & f_L)?10:20;
        if((unsigned int)frame%durFlag!=0) return dat;
        int8_t*data = (int8_t*) dat;
        int len = 16;
        auto segment_flags = data[3];
        int mul2 = 5*7;
        if(segment_flags&F_MIXIN) mul2 = 5*7/2;
        if(segment_flags&F_LENGTH) len=32;


        //For square waves, just get the data.
        //TODO? volume adjustment?
        if(data[0]!=0){
            auto square1 = FROMPOS(data[0]);
            uint32_t realFreq = (uint32_t)(square1[position]&127)*5*7;
            bool hold = (square1[position]&128)>0;
            if((flags&f_s)&&realFreq!=0&&song::occupancy[0]==0)
                tone(realFreq,durFlag+(hold?2:0),VOLUME_FOR(realFreq),TONE_PULSE1);
        }
        if(data[1]!=0){
            auto square2 = FROMPOS(data[1]);
            uint32_t realFreq = (uint32_t)((square2[position]&127)*mul2);
            bool hold = (square2[position]&128)>0;
            if((flags&f_S)&&realFreq!=0&&song::occupancy[1]==0)
                tone(realFreq,durFlag+(hold?2:0),VOLUME_FOR(realFreq),TONE_PULSE2|TONE_MODE4);
        }
        //for drums, we have a segment for that.
        if(data[2]!=0){
            int idx = FROMPOS(data[2])[position];
            if(idx>0){
                idx--;
                if((flags&f_N)&&song::occupancy[2]==0)
                    tone(RawNotes::configDrum[idx*8+0],RawNotes::configDrum[idx*8+1],RawNotes::configDrum[idx*8+2],RawNotes::configDrum[idx*8+3]);
                if((flags&f_P)&&song::occupancy[3]==0)
                    tone(RawNotes::configDrum[idx*8+4],RawNotes::configDrum[idx*8+5],RawNotes::configDrum[idx*8+6],RawNotes::configDrum[idx*8+7]);
            }
        }
        position++;

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
        song(PvP2,f_S|f_s|f_P|f_N|f_L,PvP2_start),
    };
};

#define NOTE_SQ2(fr,vol) SOUND_FROM(fr,fr,0,0,5,20,1,vol,TONE_PULSE1|TONE_MODE1)
