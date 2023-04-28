#include "lowpass.h"

LowPass::LowPass()
{
}

bool LowPass::get()
{
    return this->state;
}

void LowPass::push(bool raw_state)
{
    if (raw_state != prev_raw_state)
    {
        this->counter = 0;
    }
    else
    {
        this->counter++;
        if (this->counter > THRESHOLD)
        {
            this->state = raw_state;
        }
    }
    this->prev_raw_state = raw_state;
}
