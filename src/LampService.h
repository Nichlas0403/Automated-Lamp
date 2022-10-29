#ifndef LampService_h
#define LampService_h
#include "Arduino.h"

class LampService
{
    public:
        void TurnLampOn(int gpio);
        void TurnLampOff(int gpio);
};

#endif