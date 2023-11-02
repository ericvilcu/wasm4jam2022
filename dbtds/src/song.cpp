#include "GLOB.h"
#include "song.h"
#include "wasm4.h"
#include "sng.h"


namespace song
{
#if MUSIC_TEST_MODE
    void musicChosoer();
#endif

    uint8_t flags=0b11110000;
    int corSng=0;
    uint8_t* flag(){
        return &flags;
    }
    void setFlag(int i){
        flags|=(1<<i);
    }
    void unSetFlag(int i){
        flags&=~(1<<i);
    }
    bool getFlag(int i){
        return ((flags>>i)&1)==1;
    }
    void switchTrack(int id){
        corSng=id;
    }
    void process()
    {
#if MUSIC_TEST_MODE
        musicChosoer();
#endif
        if(corSng==0)
            return;
    }

#if MUSIC_TEST_MODE
    uint8_t lastGP=0;
    int cflag=0;
    void musicChosoer(){
        uint8_t DOWN = (lastGP ^ *GAMEPAD1) & *GAMEPAD1;
        bool Z=(DOWN&BUTTON_1    )!=0;
        bool X=(DOWN&BUTTON_2    )!=0;
        bool W=(DOWN&BUTTON_UP   )!=0;
        bool A=(DOWN&BUTTON_LEFT )!=0;
        bool S=(DOWN&BUTTON_DOWN )!=0;
        bool D=(DOWN&BUTTON_RIGHT)!=0;
        if(W)switchTrack((corSng + 1)%NUM_SONGS);
        if(S)switchTrack((corSng + NUM_SONGS - 1)%NUM_SONGS);
        if(D)cflag = (cflag+7)%8;
        if(A)cflag = (cflag+1)%8;
        if(Z)setFlag(cflag);
        if(X)unSetFlag(cflag);
        char msg[]="SONG  = 00\n"
                   "FLAGS = 00000000\n"
                   "SELECT=         \n"
                   "LEGEND  SsPNL123";
        msg[ 8]+=(corSng/10);
        msg[ 9]+=(corSng%10);
        msg[26]+=((flags>>0)&1);
        msg[25]+=((flags>>1)&1);
        msg[24]+=((flags>>2)&1);
        msg[23]+=((flags>>3)&1);
        msg[22]+=((flags>>4)&1);
        msg[21]+=((flags>>5)&1);
        msg[20]+=((flags>>6)&1);
        msg[19]+=((flags>>7)&1);
        msg[43-cflag]='A';
        text(msg,15,15);
        lastGP=*GAMEPAD1;
    }
#endif

} // namespace song

