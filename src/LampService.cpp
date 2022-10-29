#include "LampService.h"
#include "Arduino.h"

//NOTE: The 5V relay being used works in some sort of reversed way. When it's drawn towards ground, it turns on.
//Therefore the inverted logic below.

void LampService::TurnLampOn(int gpio)
{
    digitalWrite(gpio, LOW);
}

void LampService::TurnLampOff(int gpio)
{
    digitalWrite(gpio, HIGH);
}