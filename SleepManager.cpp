#include "SleepManager.h"
#include "driver/rtc_io.h"

void SleepManager::gotoSleep() {
    Serial.println("Going to sleep...");
    delay(100);

    // Turn off touchscreen and frontlight to save energy duiring deep sleep
    display.tsShutdown();  // Turn off the display touchscreen
    display.frontlight(0); // Turn off the frontlight

    rtc_gpio_isolate(GPIO_NUM_12); // Isolate GPIO12 to reduce power consumption related to pull-up/downs.
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_36, 0);   // wakeup from touch int and wakeup btn
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