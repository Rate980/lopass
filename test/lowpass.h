#ifndef nvaionriwo_lowpass_h
#define nvaionriwo_lowpass_h
// #ifndef THRESHOLD
#define THRESHOLD 10
// #endif
class LowPass
{
public:
    LowPass();
    bool get();
    void push(bool raw_state);

private:
    bool state = false;
    bool prev_raw_state = false;
    int counter = 0;
};

#endif
