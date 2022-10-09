#include "SleepManager.h"
#include "driver/rtc_io.h"
#include "driver/adc.h"
#include <esp_wifi.h>
#include <esp_bt.h>

void SleepManager::gotoSleep() {
    display.rtcGetRtcData();
    auto hour = display.rtcGetHour();
    auto min = display.rtcGetMinute();
    auto sec = display.rtcGetSecond();
    auto sleepDuration = hour == 0 ? SLEEP_TIME_LONG : SLEEP_TIME_SHORT;
    Serial.println("Going to sleep. Current RTC time: " + String(hour) + ":" + String(min) + ":" + String(sec) + ". Sleeping for " + String(sleepDuration) + " minutes");

    // Turn off touchscreen and frontlight to save energy duiring deep sleep
    display.tsShutdown();  // Turn off the display touchscreen
    display.frontlight(0); // Turn off the frontlight

    // adc_power_release();
    // adc_power_off
    rtc_gpio_isolate(GPIO_NUM_12);                                      // Isolate GPIO12 to reduce power consumption related to pull-up/downs.
    esp_sleep_enable_timer_wakeup(sleepDuration * US_TO_MIN_FACTOR);    // wakeup from timer
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_36, 0);                       // wakeup from touch int and wakeup btn
    Serial.println("Sleeping");
    esp_deep_sleep_start();
}

void SleepManager::printWakeupReason() {
  auto wakeupReason = esp_sleep_get_wakeup_cause();
  switch(wakeupReason) {
    case 1  : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case 2  : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case 3  : Serial.println("Wakeup caused by timer"); break;
    case 4  : Serial.println("Wakeup caused by touchpad"); break;
    case 5  : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.println("Wakeup was not caused by deep sleep"); break;
  }
}

bool SleepManager::isWakeupByButton() {
    return esp_sleep_get_wakeup_cause() == 2;
}
