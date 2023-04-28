#ifndef nvaionriwo_lowpass_h
#define nvaionriwo_lowpass_h
// #ifndef THRESHOLD
#define THRESHOLD 10
// #endif
#include <Arduino.h>
class LowPass
{
public:
    LowPass();
    int get();
    void push(int raw_state);

private:
    int state = LOW;
    int prev_raw_state = LOW;
    int counter = 0;
};

#endif
