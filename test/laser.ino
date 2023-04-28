#include "laser.hpp"
#define IN 26
#define LASER 0
#include "lowpass.h"
LowPass lowPass;
void laserTask(void *args)
{
    while (true)
    {
        lowPass.push(digitalRead(IN));
        digitalWrite(LASER, lowPass.get());
    }
}
