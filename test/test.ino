#include "lowpass.h"
#define IN1 26
#define IN2 36
#define LASER 0
#include <M5StickC.h>
#include "laser.hpp"
#include "timer.hpp"
#include "move.hpp"

LowPass lowPass1;
LowPass lowPass2;
// int states[2] = {LOW, LOW};
// int odl_states[2] = {LOW, LOW};

void setup()
{
    M5.begin();
    // M5.Lcd.drawString("00:00", 10, M5.Lcd.height() / 2, 7);
    // M5.Lcd.print("01:23");
    Serial.begin(115200);
    pinMode(IN1, INPUT);
    pinMode(IN2, INPUT);
    pinMode(LASER, OUTPUT);
    // digitalWrite(LASER, HIGH);
    M5.MPU6886.Init();
    xTaskCreatePinnedToCore(BtnA_Task, "BtnA_Task", 2048, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(laserTask, "laserTask", 1024, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(moveTask, "moverTask", 2048, NULL, 1, NULL, 1);
}
void loop()
{
    // lowPass1.push(digitalRead(IN1));
    // lowPass2.push(digitalRead(IN2));
    // states[0] = lowPass1.get();
    // states[1] = lowPass2.get();
    // if (states[0] != odl_states[0])
    // {
    //     Serial.printf("IN1: %d\n", lowPass1.get());
    //     Serial.println();
    // }
    // if (states[1] != odl_states[1])
    // {
    //     Serial.printf("IN2: %d\n", lowPass2.get());
    //     Serial.println();
    // }
    // if (lowPass1.get() == HIGH)
    // {
    //     digitalWrite(LASER, HIGH);
    // }
    // else
    // {
    //     digitalWrite(LASER, LOW);
    // }
    // odl_states[0] = states[0];
    // odl_states[1] = states[1];
}
