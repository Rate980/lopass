#include "timer.hpp"
#include <M5StickC.h>
bool deletable = true;
RTC_TimeTypeDef timeStrict;
void drawTime(int time)
{
    // M5.Lcd.fillScreen(TFT_BLACK);
    int minutes = time / 60;
    int seconds = time % 60;
    char str[6];
    sprintf(str, "%02d:%02d", minutes, seconds);
    deletable = false;
    M5.Lcd.drawString(str, 10, M5.Lcd.height() / 2, 7);
    deletable = true;
}
bool isTaskRunning(TaskHandle_t task)
{
    if (task == NULL)
    {
        return false;
    }
    auto taskState = eTaskGetState(task);
    return taskState == eRunning || taskState == eBlocked;
}

void BtnA_Task(void *args)
{
    M5.Lcd.setRotation(3);
    M5.Lcd.setTextDatum(3);
    drawTime(0);
    // int BtnAStateOld = LOW;
    TaskHandle_t timerTask = NULL;
    RTC_TimeTypeDef RTC_TimeStruct;
    int startTime = 0;
    int stopDiff = -1;
    while (true)
    {
        M5.update();
        // int BtnAState = M5.BtnA.read();
        // if (BtnAStateOld != BtnAState)
        // {
        if (M5.BtnA.wasReleased())
        {
            if (isTaskRunning(timerTask))
            {
                while (!deletable)
                {
                    delay(1);
                }
                vTaskDelete(timerTask);
                stopDiff = getSeconds();
            }
            else
            {
                if (stopDiff < 0)
                {
                    RTC_TimeStruct.Hours = 0;
                    RTC_TimeStruct.Minutes = 0;
                    RTC_TimeStruct.Seconds = 0;
                    stopDiff = 0;
                }

                startTime = getSeconds() - stopDiff + startTime;
                xTaskCreatePinnedToCore(TimerTask, "TimerTask", 2048, &startTime, 1, &timerTask, 1);
            }
        }
        if (M5.BtnB.wasReleased())
        {
            if (!isTaskRunning(timerTask))
            {
                startTime = 0;
                stopDiff = -1;
                drawTime(0);
            }
        }
        // BtnAStateOld = BtnAState;
        delay(10);
    }
}
int getSeconds()
{
    M5.Rtc.GetTime(&timeStrict);
    return ((int)timeStrict.Hours * 60 + (int)timeStrict.Minutes) * 60 + (int)timeStrict.Seconds;
}

void TimerTask(void *args)
{
    int *startTime = (int *)args;
    while (true)
    {
        drawTime(getSeconds() - *startTime);
        delay(100);
    }
}
