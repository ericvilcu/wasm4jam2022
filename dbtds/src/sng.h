#include <stdint.h>
#define byteAlgin 4
#define byteAlginMask ((1<<byteAlgin)-1) 
#define POSOFFULL(song) ((int)(START_NOTE-&RawNotes::song))
#define POSOF(song) (((POSOFFULL(song) >> 4)+1))
#define POSOFASSERT(song) static_assert((((int)POSOFFULL(song))&byteAlgin) == 0)
#define FROMPOS(dat) ((uint8_t*)(START_NOTE+(dat)-1))

#define POSOFTRK(song) ((int)(START_TRACK-&TrackData::song::c1))
#define FROMPOSTRK(dat) ((void*)(START_TRACK+(dat)))
#define NOSONG 0

#define FREQ(freq) ((uint8_t)(freq))
#define NONOTE 0 
namespace RawNotes{
    #define START_NOTE (&RawNotes::beep1)
    uint8_t beep1[]={
        FREQ(25),
        FREQ(30),
        FREQ(40),
        NONOTE,
        FREQ(25),
        FREQ(30),
        FREQ(40),
        NONOTE,
        FREQ(35),
        FREQ(40),
        FREQ(50),
        NONOTE,
        FREQ(35),
        FREQ(40),
        FREQ(40),
        NONOTE,
    };
};

namespace TrackData{
    #define fork(name,a,b,yesFlag,noFlag) namespace name{\
        uint8_t d=0x10000000+yesFlag+(noFlag<<3);\
        uint8_t c1=POSOFTRK(a);\
        uint8_t c2=POSOFTRK(b);\
    };
    #define track3(name,a,b,c,next,flags) namespace name{\
        uint8_t c1=POSOF(a);POSOFASSERT(a);\
        uint8_t c2=POSOF(b);POSOFASSERT(b);\
        uint8_t c3=POSOF(c);POSOFASSERT(c);\
        uint8_t c4=((flags)<<8)+POSOFTRK(next);\
    };
    #define START_TRACK (&TrackData::normal_start::c1)
    track3(normal_start,beep1,beep1,beep1,normal_start,0);
    
    enum Type:uint8_t{FORK=1,TRACK3=0};
    uint8_t type(void* dat){
        //the very first bit
        return (Type)((*(int8_t*)dat)&128)>>7;
    }
    void* process_fork(void* dat, int flags){
        int8_t*data = (int8_t*) dat;
        if((flags&(*(int8_t*)dat))==0 && (flags|((*(int8_t*)dat)>>3)) == 0b111)
            return FROMPOSTRK(data[1]);
        return FROMPOSTRK(data[2]);
    }
    
};

namespace SongData{
    #define SimpleSongData()
};

#define SOUND_FROM(f1,f2,atk,decay,sus,rel,peak,volume,flags) f1+0x10000*(uint32_t)(f2),((uint32_t)(atk)<<24)+((uint32_t)(decay)<<16)+(uint32_t)(sus)+((uint32_t)(rel)<<8),(peak<<8)+volume,flags
#define NOTE_SQ2(fr,vol) SOUND_FROM(fr,fr,0,0,5,20,1,vol,TONE_PULSE1|TONE_MODE1)
