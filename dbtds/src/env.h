#pragma once
#include "GLOB.h"
#include "utils.h"
#include "position.h"
extern FakeVector2 CameraPosition;
extern int frame;
//default is 0
#if ALLOW_EFFECTS
extern int effect_type;
#endif
void emphasise_color(uint8_t idx);
void de_emphasise_color(uint8_t idx);
