#include "utils.h"

constexpr float COS_ROT=0.99452189536f;
constexpr float SIN_ROT=0.10452846326f;
constexpr float COS_ROT_HALF=0.99862953475f;
constexpr float SIN_ROT_HALF=0.05233595624f;
constexpr float SIN_ROT_NINETY=0.06661858156f;
constexpr float COS_ROT_NINETY=0.99556196459f;

float SIN_COS_ARRAY[] = {
    SIN_ROT,COS_ROT,
    SIN_ROT_HALF,COS_ROT_HALF,
    SIN_ROT_NINETY,COS_ROT_NINETY,
};
constexpr uint8_t MASK_AND = 0b11111110;

void rotate(float&fx, float&fy, uint8_t idx){
    
    float ox=fx;
    float oy=fy;
    bool parity = (idx&1);

    fx=ox*SIN_COS_ARRAY[idx|1]+(parity?oy:-oy)*SIN_COS_ARRAY[idx&0b1110];
    fy=(parity?-ox:ox)*SIN_COS_ARRAY[idx&0b1110]+oy*SIN_COS_ARRAY[idx|1];
    normalize(fx,fy);
}