#include <stdint.h>

namespace song
{
    enum library:uint8_t{
        SILENCE=0,
        CAPTAIN=1,
        CAPTAIN_TENSE=2,
        CAPTAIN_FULL=3,
        PvP=4,
        PvP_2=5,
        NUM_SONGS
    };
    enum flags:uint8_t{
        FLAGNO_SQR_MAJOR = 7,
        FLAGNO_SQR_MINOR = 6,
        FLAGNO_TRIANGLE = 5,
        FLAGNO_NOISE = 4,
        FLAGNO_LOW = 3,
        FLAGNO_DECISION1 = 2,
        FLAGNO_DECISION2 = 1,
        FLAGNO_DECISION3 = 0,
        
        FLAG_SQR_MAJOR = (1<<7),
        FLAG_SQR_MINOR = (1<<6),
        FLAG_TRIANGLE = (1<<5),
        FLAG_NOISE = (1<<4),
        FLAG_LOW = (1<<3),
        FLAG_DECISION1 = (1<<2),
        FLAG_DECISION2 = (1<<1),
        FLAG_DECISION3 = (1<<0),
    };
    //NOTE: use FLAG_*
    uint8_t* flag();
    //NOTE: use FLAGNO_*
    void setFlag(int flagno);
    //NOTE: use FLAGNO_*
    void unSetFlag(int flagno);
    void switchTrack(int id);
    void process();
    void play_sound(uint8_t idx);
} // namespace song