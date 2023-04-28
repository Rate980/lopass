#include "laser.hpp"
#define IN 26
#define LASER 0
#include "lowpass.h"
#include <Arduino.h.>
LowPass lowPass;
void laserTask(void *args)
{
    int old = LOW;
    while (true)
    {
        lowPass.push(digitalRead(IN));
        digitalWrite(LASER, lowPass.get());
        if (old != lowPass.get())
            Serial.println(lowPass.get());
        old = lowPass.get();
        delay(1);
    }
}
