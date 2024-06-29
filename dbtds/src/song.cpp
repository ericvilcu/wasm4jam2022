#include "GLOB.h"
#include "song.h"
#include "wasm4.h"
#include "sng.h"


namespace song
{
#if MUSIC_TEST_MODE
    void musicChooser();
#endif

    uint8_t flags=0b11110000;
    int corSng=0;
    void* currentSegment;
    uint8_t segmentProgress;
    uint8_t occupancy[4]={0,0,0,0};
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
        if(corSng!=id){
            corSng=id;
            if(id>0){
                flags = SongData::songs[id-1]>>8;
                currentSegment = FROMPOSTRK(SongData::songs[id-1]&255);
                segmentProgress = 0;
            }
        }
    }
    
    
    void play_sound(uint8_t idx){
        uint32_t* config = &RawNotes::configDrum[idx*8];
        int id = config[3]&3;
        //NOTE: ((uint32_t)(atk)<<24)+((uint32_t)(decay)<<16)+(uint32_t)(sus)+((uint32_t)(rel)<<8)
        occupancy[config[3]&3] = (config[2]&255)+((config[2]>>24)&255)+((config[2]>>16)&255);//atk + sustain + decay
        tone(config[0],config[1],config[2],config[3]);
    }

    void process()
    {
        for(int i=0;i<4;++i)
            occupancy[i]=min(occupancy[i],(uint8_t)(occupancy[i]-1));
#if MUSIC_TEST_MODE
        musicChooser();
#endif
        if(corSng==0||currentSegment==(void*)0)
            return;
        
        bool playedFrame=false;
        do
        {
            auto type = TrackData::type(currentSegment);
            if(type==TrackData::FORK){
                currentSegment = TrackData::process_fork(currentSegment,flags);
            } else /*if(type==TrackData::TRACK3)*/{
                playedFrame = true;
                currentSegment = TrackData::process_track3(currentSegment,flags,segmentProgress);
            }
        } while (!playedFrame&&(currentSegment!=(void*)0));
    }

#if MUSIC_TEST_MODE
    uint8_t lastGP=0;
    int cflag=0;
    void musicChooser(){
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
        char msg0[]="SONG  = 00\0";
        char msg1[]="FLAGS = 00000000\0";
        char msg2[]="SELECT=         \0";
        char msg3[]="LEGEND  SsPNL321\0";
        msg0[ 8]+=(corSng/10);
        msg0[ 9]+=(corSng%10);
        msg1[15]+=((flags>>0)&1);
        msg1[14]+=((flags>>1)&1);
        msg1[13]+=((flags>>2)&1);
        msg1[12]+=((flags>>3)&1);
        msg1[11]+=((flags>>4)&1);
        msg1[10]+=((flags>>5)&1);
        msg1[ 9]+=((flags>>6)&1);
        msg1[ 8]+=((flags>>7)&1);
        msg2[15-cflag]='A';
        text(msg0,15,15+8*1);
        text(msg1,15,15+8*2);
        text(msg2,15,15+8*3);
        text(msg3,15,15+8*4);
        if(song::corSng==0)text("(silence)",15,12);
        if(song::corSng==1)text("Captain's pride",15,12);
        if(song::corSng==2)text("Captain's fears",15,12);
        if(song::corSng==3)text("Captain's brawn",15,12);
        if(song::corSng==4)text("PvP",15,12);
        if(song::corSng==5)text("PvP (alt)",15,12);
        if(cflag==0)text("Flag 1 (f_1)\nUser flag.\nControls song flow\n\nTypically,\nrepresents how \nintense the\nbattle is.",15,15+8*5+5);
        if(cflag==1)text("Flag 2 (f_2)\nUser flag.\nControls song flow\n\nTypically,\n1 if player 1 is \nclose to winning,\n0 otherwise.",15,15+8*5+5);
        if(cflag==2)text("Flag 3 (f_3)\nUser flag.\nControls song flow\n\nTypically,\n1 if player 2 (or\nan enemy) is\nclose to winning,\n0 otherwise.",15,15+8*5+5);
        if(cflag==3)text("Flag 4 (f_L)\nStrength flag.\n\nSet to 1 for\nhigher bpm.",15,15+8*5+5);
        if(cflag==4)text("Flag 5 (f_N)\nNoise flag.\n\nSet to 0 to\ndisable the\nnoise channel.\n(used for drums)",15,15+8*5+5);
        if(cflag==5)text("Flag 6 (f_P)\nPulse flag.\n\nSet to 0 to\ndisable the\npulse channel.\n(used for drums)",15,15+8*5+5);
        if(cflag==6)text("Flag 7 (f_s)\nSquare 1 flag.\n\nSet to 0 to\ndisable the\nsquare 1 channel.\n(usually main\nmotif)",15,15+8*5+5);
        if(cflag==7)text("Flag 8 (f_S)\nSquare 2 flag.\n\nSet to 0 to\ndisable the\nsquare 2 channel.\n(usually\ndroning/ambient)",15,15+8*5+5);

        lastGP=*GAMEPAD1;
    }
#endif

} // namespace song

