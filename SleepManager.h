#ifndef __SLEEPMANAGER_H__
#define __SLEEPMANAGER_H__

#include "Inkplate.h"

#define uS_TO_S_FACTOR 1000000
#define TIME_TO_SLEEP  600

class SleepManager {
private:
    Inkplate& display;

public:
    SleepManager(Inkplate& display) : display(display) { }
    ~SleepManager() { }

    void gotoSleep();
    void printWakeupReason();
    bool isWakeupByButton();
};



#endif