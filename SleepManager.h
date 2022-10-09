#ifndef __SLEEPMANAGER_H__
#define __SLEEPMANAGER_H__

#include "Inkplate.h"

#define US_TO_MIN_FACTOR 60000000ULL    // unsigned long long == uint_64
#define SLEEP_TIME_SHORT 20
#define SLEEP_TIME_LONG 300

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
