#include "MathService.h"
#include "Arduino.h"

unsigned int MathService::ConvertMillisToSeconds(unsigned long millis)
{
    return millis / 1000;
}

unsigned long MathService::ConvertSecondsToMillis(unsigned int seconds)
{
    return seconds * 1000;
}
