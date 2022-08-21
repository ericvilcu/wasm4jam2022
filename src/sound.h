#pragma once
#include "utils.h"
#include "wasm4.h"

#include "sng.h"
namespace sound
{
    bool shuffle=false;
    int tone_time_occupied[4]{0,0,0,0};
    int tone_occupied_priority[4]{0,0,0,0};
    int32_t basic_sounds[]={
        //freq 1/2,          ,time    (all in seconds*60)                  ,
        //freq1+(freq2<<16),(attack<<24)+(decay<<16)+sustain+(release<<8),(peak<<8)+volume,flags
        SOUND_FROM(250,10,0,30,60,200,40,6,TONE_NOISE),//death
        SOUND_FROM(300,80,0,0,15,0,40,20,TONE_NOISE),//player hit
        SOUND_FROM(300,512,5,10,0,35,40,10,TONE_NOISE),//enemy hit
        SOUND_FROM(100,100,0,0,100,0,0, 5,TONE_NOISE),//laser
    };
    uint32_t*track_list[]={
        waiting,
        smithereens,
        beckon,
        beckon_slow,
        mark,
    };
    #define track_size(ptr) sizeof(ptr)/(sizeof(uint32_t))
    int track_lengths[]={
        //I gave these songs their names before starting to write them
        //Their names are not that fitting.
        track_size(waiting),
        track_size(smithereens),
        track_size(beckon),
        track_size(beckon_slow),
        track_size(mark),
    };
    int current_track=-1;
    int player_pos=0;
    int wait=0;
    void tone_respect_priority(uint32_t*data,int priority){
        int instrument=(int)(data[3]&3);
        if(tone_time_occupied[instrument]<0||tone_occupied_priority[instrument<=priority]){
            uint8_t*bm=(uint8_t*)(&tone_time_occupied[instrument]);//I need to stop coding like this.
            tone_time_occupied[instrument]=(int)bm[0]+(int)bm[1]+(int)bm[2]+(int)bm[3];
            tone(data[0],data[1],data[2],data[3]);
        }
    }
    void tick_play(){//continues playing music
        if(current_track!=-1){
            while(wait==0){
                tone(track_list[current_track][player_pos+0],track_list[current_track][player_pos+1],
                    track_list[current_track][player_pos+2],track_list[current_track][player_pos+3]);
                wait=(int)track_list[current_track][player_pos+4];
                player_pos+=5;
                if(player_pos>=track_lengths[current_track]){
                    player_pos=0;
                    if(shuffle){
                        current_track=(current_track+(int)1)%(int)(sizeof(track_lengths)/sizeof(int));
                    }
                }
            }
            wait=wait-1;
        }
    }
    void switch_track(int id,int start_pos=0){
        if(id!=current_track||start_pos!=0){
            current_track=id;
            player_pos=start_pos;
        }
    }
    /**
     * priority 0 -> none
     */
    /*void play(int sound,int priority){
        tone();
        
    }*/

}