#ifndef timer_hpp
#define timer_hpp
void drawTime(int time);
bool isTaskRunning(TaskHandle_t task);
void BtnA_Task(void *args);
int getSeconds();
void TimerTask(void *args);
#endif
