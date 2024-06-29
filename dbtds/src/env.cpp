#include "env.h"
#include "colors.h"

FakeVector2 CameraPosition;
#if ALLOW_EFFECTS
int effect_type=1;//1<<24;
#endif
int frame;

uint32_t emphasise(uint32_t c){
    switch (c)
    {
        case CAPTAIN_COLOR: return CAPTAIN_INTENSE;
        case SACRIFICE: return SACRIFICE_INTENSE;
        default: return c;
    }
} 

uint32_t de_emphasise(uint32_t c){
    switch (c)
    {
        case CAPTAIN_INTENSE: return CAPTAIN_COLOR;
        case SACRIFICE_INTENSE: return SACRIFICE;
        default: return c;
    }
} 

void emphasise_color(uint8_t idx)
{
    PALETTE[idx] = emphasise(PALETTE[idx]);
}

void de_emphasise_color(uint8_t idx)
{
    PALETTE[idx] = de_emphasise(PALETTE[idx]);
}
