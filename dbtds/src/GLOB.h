#pragma once
//for release build 0
#define MUSIC_TEST_MODE 1
//for release build 1
#define NO_MUSIC 1

#define ENABLE_MUSIC (NO_MUSIC==0 || MUSIC_TEST_MODE==1)
#define ENABLE_GAMEPLAY MUSIC_TEST_MODE==0
