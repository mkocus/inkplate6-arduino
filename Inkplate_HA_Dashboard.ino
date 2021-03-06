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
void btn1Callback(){
  Serial.println("Button 1 clicked");
  mqtt.turnOffAllLights();
}

void btn2Callback(){
  Serial.println("Button 2 clicked");
  mqtt.startClean();
}

void btn3Callback(){
  Serial.println("Button 3 clicked");
  mqtt.stopClean();
}

void btn4Callback(){
  Serial.println("Button 4 clicked");
  mqtt.gotoDock();
}

// 1st row
Button btn1(display, "Lights Off", 100, 100, 200, 200, &btn1Callback);

// 2nd row
Button btn2(display, "Start clean", 100, 400, 200, 200, &btn2Callback);
Button btn3(display, "Stop clean", 400, 400, 200, 200, &btn3Callback);
Button btn4(display, "Go to dock", 700, 400, 200, 200, &btn4Callback);

unsigned long uptime = 0;
int currentPage = 0;
bool requestUpdate = false;

// override
void setup() {
  Serial.begin(115200);
  Serial.println("START");
  display.begin();

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
      if (btn1.loop(x[0], y[0], touched) || btn2.loop(x[0], y[0], touched) || btn3.loop(x[0], y[0], touched) || btn4.loop(x[0], y[0], touched)) {
        requestUpdate = true;
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
    if (currentPage >= 1 && millis() - uptime >= 20000) {
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
  if (!display.drawImage("http://192.168.50.201:5000/", display.PNG, 0, 0, 1, 0)) {
    Serial.println("ERROR: Image drawing failed!");
  }
  display.display(true);
  sleeper.gotoSleep();
}

void showControlScreen() {
  Serial.println("Showing control screen");
  currentPage = 1;
  display.selectDisplayMode(INKPLATE_1BIT);
  btn1.draw();
  btn2.draw();
  btn3.draw();
  btn4.draw();
  display.display(true);
  wifi.fastConnect();
  uptime = millis();
}
