#include "lowpass.h"
#define IN1 26
#define IN2 36

LowPass lowPass;

void setup()
{
    Serial.begin(115200);
    pinMode(IN1, INPUT);
    pinMode(IN2, INPUT);
}
void loop()
{
    Serial.print(digitalRead(IN1));
    Serial.print(",");
    Serial.println(digitalRead(IN2));
    delay(1);
}
