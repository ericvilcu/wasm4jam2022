#pragma once
#include "utils.h"
#include "wasm4.h"

#include "sng.h"
namespace sound
{
    bool shuffle=false;
    int tone_time_occupied[4]{0,0,0,0};
    int tone_occupied_priority[4]{0,0,0,0};
    enum {
        SOUND_DEATH=0,
        SOUND_PLAYER_HIT=1,
        SOUND_ENEMY_HIT=2,
        WATER_DEATH=3,
    };
    uint32_t basic_sounds[]={
        //freq 1/2,          ,time    (all in seconds*60)                  ,
        //freq1+(freq2<<16),(attack<<24)+(decay<<16)+sustain+(release<<8),(peak<<8)+volume,flags
        4,SOUND_FROM(250,10,0,30,60,200,60,12,TONE_NOISE),//death
        3,SOUND_FROM(300,80,0,0,15,0,60,40,TONE_NOISE),//player hit
        3,SOUND_FROM(150,170,5,10,0,35,60,20,TONE_NOISE),//enemy hit
        5,SOUND_FROM(700,50,0,0,0,100,0,75,TONE_NOISE),//water hit
    };
    const uint32_t*track_list[]={
        waiting,
        smithereens,
        beckon,
        beckon_slow,
        mark,
        chair_spin,
        downloading,
        rising,
    };
    #define track_size(ptr) sizeof(ptr)/(sizeof(uint32_t))
    const int track_lengths[]={
        //I gave these songs their names before starting to write them
        //Their names are not that fitting.
        track_size(waiting),
        track_size(smithereens),
        track_size(beckon),
        track_size(beckon_slow),
        track_size(mark),
        track_size(chair_spin),
        track_size(downloading),
        track_size(rising),
    };
    int current_track=-1;
    int player_pos=0;
    int wait=0;
    void tone_respect_priority(const uint32_t*data,int priority){
        int instrument=(int)(data[3]&3);
        //tracef("%d %d %d",tone_time_occupied[instrument],tone_occupied_priority[instrument],priority);
        if(tone_time_occupied[instrument]<=0||tone_occupied_priority[instrument]<=priority){
            uint8_t*bm=(uint8_t*)(&data[1]);//I need to stop coding like this.
            tone_time_occupied[instrument]=(int)bm[0]+(int)bm[1]/*+(int)bm[2]+(int)bm[3]*/;
            tone_occupied_priority[instrument]=priority;
            tone(data[0],data[1],data[2],data[3]);
        }
    }
    void tick_play(){//continues playing music
        if(current_track!=-1){
            while(wait==0){
                tone_respect_priority(&track_list[current_track][player_pos],2);
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
        for(int i=0;i<=3;++i){
            tone_time_occupied[i]=max(0,tone_time_occupied[i]-1);
        }
    }
    void switch_track(int id,int start_pos=0){
        if(id!=current_track||start_pos!=0){
            current_track=id%8;
            player_pos=start_pos;
        }
    }
    /**
     * priority 0 -> none
     */
    void sound_effect(int sound){
        //tracef("%d %d",sound,(int)basic_sounds[5*sound]);
        tone_respect_priority(&basic_sounds[5*sound+1],(int)basic_sounds[5*sound]);
    }

}