#pragma once
#include "custom_draws.h"
//Note to self: max 19 characters per line.
//Could be 20 if I didn't leave some space on the sides.
const char* dialogues_unmodifiable[] = 
{
//"2My naym dave",
//"f",
//"2dave died :<",
//"f",
"1Press x to advance\ndialogue.",
"2Far away, a poor\nship is trying its\nhardest to survive.",
"2268 seconds to\nimpact...",
"2Its only hope is \nthe admin codes I\nhave.",
"2But I can't deliver\nthem directly.\nI must talk to this\nplace's deity.",
"2Only one of us can\nproceed. Please\ngive up for me.",
"1I'm sorry.\nThat's not how this\nplace works.",
"f",
"1Don't feel too bad.\nIt takes no time to\nget back here",
"2265 seconds to\nimpact...\nSo time goes slower\nhere?",
"f",
"216 max bullets.\nAttacking dive.\nYou?",
"15. Dashing.\nHave fun dying to\nthe thing in the\nwater.",
"2If you were given\nsuch a handicap...\nYou must be quite\nskilled.",
"f",
"2Hoh!Skilled indeed!\nI hope we meet\nagain...",
"1Thanks.",
"f",
"2People that can\nget this far always\nhave something good\nto say.",
"2How did you get\nhere?\nHow did you die?",
"1Suffocation.\nPillow.\nAccident.\nYou?",
"2Dragon.\nI want a rematch.",
"1The fifth cannot\nrevive you.",
"2I know.\nBut these waters\ncontain our fears.",
"1I see.\nFirst you must earn\nthe right to dive.",
"f",
"1Better luck\nnext time.",
"2I'll find him one\nday.",
"f",
"2I want to meet five\nbecause I fell in\nlove with them.\n^w^",
"1???????????????????",
"2OwO\n(it is a surprised\nface)",
"1Shut",
"f",
"2;w;",
"1For once I feel\nlike I'm helping by\ndefeating someone.",
"1I am now leaving\nrapidly.",
"f",
"2Hello traveler!",
"1Greetings.",
"2I have to wonder\n...\nWhy are you here?",
"2Why do you want\nto meet the fifth\ndeity?",
"1I don't know.",
"1I'm here because\nthe sea was the\nfirst thing I saw\nafter dying.",
"2Same.",
"2I guess you could\nsay both of us are\nseeking purpose?",
"1I suppose so.",
"f",
"2I think you have\none more fight.",
"2Before you're done\nwith this track.",
"2And can move on to\nthe next.",
"1If you know that,\ndid you ever get\nto the fifth?",
"2Last time I did, I\nbuilt this track.",
"1I see.",
"f",
"2You were right!\nThis took no time\nat all!",
"1You know the fifth\ncan't commune with\nthe living, right?",
"2WHAT?",
"2The situation got\nresolved without me\n",
"2I'm here because I\nwish to have\na grudge match.",
"1Wish granted.",
"f",
"2Don't think this is\nover!",
"2I will find you\nin the next track.",
"1I'm looking\nforward to it.",
"1The dev is\ntalking now.",
"1From here on out\nThe game will read\nrandom-ish data to\nmake opponents.",
"1I'm leaving it\nin because it is\nfunny.",
"1This is basically\nthe end.",
"1Until next time,\nGoodbye!",
"f",
"q",
};
//Silly cpp standards...
//This is incredibly cursed.
#define dialogues ((char**)(void*)dialogues_unmodifiable)


void tutorial(){
    *DRAW_COLORS=4;
    text("Use arrow keys.", 3, 5);
    text("Up to move forward.", 3, 15);
    text("Down to brake.", 3, 25);
    text("Left/Right to steer", 3, 35);
    text("X to shoot.", 3, 45);
    text("Z to lock angle", 3, 55);
    text(" and dash with", 3, 65);
    text(" arrow keys.", 3, 75);
    
    text("Going below the", 3, 85);
    text(" waves gives tempo-", 3, 95);
    text(" -rary immunity", 3, 105);
    text("<-PvP        story->", 0, 125);
    text("GOOD LUCK!", 3, 145);
};
int next_dialogue=0;//should have used a char*. May have lessened the cursed stuff too. a little late now.
float dialogue_progress=0;

int talk(){
    if(dialogues[next_dialogue][0]>='1'&&dialogues[next_dialogue][0]<='2')
        *DRAW_COLORS=(uint16_t)(dialogues[next_dialogue][0]-'0'+2);
    else
        return 2;
    int i_progress=floor(dialogue_progress);
    if(dialogues[next_dialogue][i_progress]=='\0'){
        if(*GAMEPAD1 & BUTTON_1){
            next_dialogue+=1;
            dialogue_progress=0.0f;
            if(dialogues[next_dialogue][0]=='f'){
                next_dialogue+=1;
                return 2;
            }
            return 1;
        }
        else
            dialogue_progress-=1.0f;//Like, who holds a button down for only 1 frame? This will work ok.
    }

    //Doing some real cursed stuff to display partial text
    char tmp=dialogues[next_dialogue][i_progress+1];
    dialogues[next_dialogue][i_progress+1]='\0';
    text(dialogues[next_dialogue]+1,4,4);
    dialogues[next_dialogue][i_progress+1]=tmp;

    if(*GAMEPAD1 & BUTTON_2){
        int max_skips=10;
        while (dialogues[next_dialogue][i_progress]!='\0'&& max_skips-->0){
            dialogue_progress+=1.0f;
            i_progress=floor(dialogue_progress);
        }
    }
    else dialogue_progress+=0.33f;
    return 0;
}
