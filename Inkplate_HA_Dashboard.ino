#include "Inkplate.h"
#include "Helpers.h"
#include "Button.h"
#include "SleepManager.h"
#include "MqttManager.h"
#include "WifiManager.h"

// base components
Inkplate display(INKPLATE_3BIT);
SleepManager sleeper(display);
MqttManager mqtt(display);
WifiManager wifi;

// ui components
void btn1aCallback(){ mqtt.turnOffAllLights(); }
void btn1bCallback(){ mqtt.turnOffXmasLights(); }
void btn2aCallback(){ mqtt.startClean(); }
void btn2bCallback(){ mqtt.stopClean(); }
void btn2cCallback(){ mqtt.gotoDock(); }

// 1st row
Button btn1a(display, "Lights Off", 100, 100, 200, 50, &btn1aCallback);
Button btn1b(display, "Xmas Toggle", 400, 100, 400, 75, &btn1bCallback);

// 2nd row
Button btn2a(display, "Start clean", 100, 300, 200, 40, &btn2aCallback);
Button btn2b(display, "Stop clean", 400, 300, 200, 60, &btn2bCallback);
Button btn2c(display, "Go to dock", 700, 300, 200, 80, &btn2cCallback);

Button* buttons[] = {&btn1a, &btn1b, &btn2a, &btn2b, &btn2c};

unsigned long uptime = 0;
int currentPage = 0;
bool requestUpdate = false;

const unsigned long CONTROL_TIMEOUT_MS = 60000;

// override
void setup() {
  Serial.begin(115200);
  Serial.println("START");
  display.begin();

  esp_bt_controller_disable();
//  adc_power_on();

// display.rtcSetTime(19, 21, 0);

  // Turn off frontlight
  display.frontlight(false);
  display.setFrontlight(0);

  sleeper.printWakeupReason();
  
  if (sleeper.isWakeupByButton()){

    if (!display.tsInit(true)) {
      Serial.println("ERROR: Touchscreen init failed!");
    }

    showControlScreen();

    xTaskCreatePinnedToCore(
                    touchLoop,   /* Task function. */
                    "Touch Loop",/* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    NULL,        /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  

    xTaskCreatePinnedToCore(
                      renderLoop,   /* Task function. */
                      "Render loop",/* name of task. */
                      10000,       /* Stack size of task */
                      NULL,        /* parameter of the task */
                      1,           /* priority of the task */
                      NULL,        /* Task handle to keep track of created task */
                      1);          /* pin task to core 1 */

    disableCore0WDT();
    disableCore1WDT();
  } else {
    showBaseScreen();
  }
}

// override
void loop() {
  // not using the task on which loop is runned
  // so it can be safely deleted
  vTaskDelete(NULL);  
}

void touchLoop(void* pvParameters ){
  Serial.println("Task touchLoop running on core " + String(xPortGetCoreID()));

  for(;;){
    if (display.tsAvailable()){
      uint8_t n;
      uint16_t x[2], y[2];
      n = display.tsGetData(x, y);
      if (n > 0) {
        uptime = millis();
        Serial.println("Touch: " + String(x[0]) + ", " + String(y[0]));
      }
      bool touched = n > 0;

      for (auto btn : buttons) {
        if (btn->loop(x[0], y[0], touched)) {
          requestUpdate = true;
          break;
        }
      }
    }
  }
}

void renderLoop(void* pvParameters ){
  Serial.println("Task renderLoop running on core " + String(xPortGetCoreID()));

  for(;;){
    if (requestUpdate) {
      requestUpdate = false;
      display.partialUpdate(false, true);
    }
    if (currentPage >= 1 && millis() - uptime >= CONTROL_TIMEOUT_MS) {
      showBaseScreen();
    }
  }
}

void showBaseScreen() {
  Serial.println("Showing base screen");
  currentPage = 0;
  wifi.fastConnect();
  mqtt.sendSystemInfo();
  display.selectDisplayMode(INKPLATE_3BIT);
  display.clearDisplay();
  Serial.println("Downloading image");
  if (!display.drawImage("http://192.168.50.201:5000/", display.PNG, 0, 0, 1, 0, 1)) {
    Serial.println("ERROR: Image drawing failed!");
  }
  Serial.println("Image downloaded, decoded and displayed.");
  display.display();
  sleeper.gotoSleep();
}

void showControlScreen() {
  Serial.println("Showing control screen");
  currentPage = 1;
  display.selectDisplayMode(INKPLATE_1BIT);
  for (auto btn: buttons) btn->draw();
  display.display(true);
  wifi.fastConnect();
  uptime = millis();
}
