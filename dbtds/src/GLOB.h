#pragma once
//for release build 0
#define MUSIC_TEST_MODE 0
//for release build 0
#define NO_MUSIC 0
//for itch 1, for wasm 0
#define ALLOW_EFFECTS 0

#define ENABLE_MUSIC (NO_MUSIC==0 || MUSIC_TEST_MODE==1)
#define ENABLE_GAMEPLAY MUSIC_TEST_MODE==0
#include "env.h"
