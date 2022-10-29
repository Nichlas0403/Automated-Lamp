#ifndef MathService_h
#define MathService_h
#include "Arduino.h"

class MathService
{
    public:
        unsigned int ConvertMillisToSeconds(unsigned long millis);
        unsigned long ConvertSecondsToMillis(unsigned int seconds);
};

#endif