#include "random.h"
namespace random
{
    //period seems to be 4924 with this and 0/1/4/8/13, with 80676 one-time numbers at the start.
    unsigned int acc=0b11000001100010000000000000001100;
    FakeVector2 randomPosition(){
        return FakeVector2(randomInt(),randomInt());
    }
    float random11(){//11 -> between -1 and 1
        return (float) randomInt() * 1.0f/2147483647.0f;
    };
    float random01(){//01 -> between 0 and 1
        return abs((float)randomInt() * 1.0f/2147483647.0f);
    }
    FakeVector2 randomPosition11(){//11 -> between -1 and 1
        auto p= FakeVector2(random11(),random11());
        return p;
    }
    int randomInt(){
        int steps = 9+(acc&15);
        acc = (acc>>steps) | ((acc^(acc>>1)^(acc>>4)^(acc>>8)^(acc>>13))<<steps);
        return (int)acc;
    }
} // namespace random
