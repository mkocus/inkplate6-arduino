#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "Inkplate.h"

typedef void (*CallbackType)();

static int btnInstanceNr = 0;

class Button {
private:
    int x;
    int y;
    int w;
    int h;
    Inkplate& display;
    CallbackType callback;
    int touchTime = 0;
    bool currentlyClicked = false;
    int instanceNr;

    bool touchInside(int touchX, int touchY) {
      auto x2 = x + w;
      auto y2 = y + h;
      return touchX >= x && touchX <= x2 && touchY >= y && touchY <= y2;
    }

public:
    Button(Inkplate& disp, int x, int y, int width, int height, CallbackType callback) 
      : display(disp), x(x), y(y), w(width), h(height), callback(callback) { instanceNr = btnInstanceNr++; }

    void draw() {
        display.drawRect(x, y, w, h, BLACK);
    }

    /** @returns true if partial update is needed, false otherwise */
    bool loop(int touchX, int touchY, bool touched) {
        auto now = millis();

        if (touched && touchInside(touchX, touchY)) {
            Serial.println("Touch in area btn[" + String(instanceNr) + "]");
            if (now - touchTime > 500) {
              Serial.println("Touch as click for btn[" + String(instanceNr) + "]");
              display.fillRect(x, y, w, h, BLACK);
              currentlyClicked = true;
              
              callback();
              touchTime = now;
              return true;
            }
            touchTime = now;
        }
        else if (currentlyClicked && now - touchTime > 500) {
          Serial.println("Touch timeout for btn[" + String(instanceNr) + "]");
          display.fillRect(x, y, w, h, WHITE);
          display.drawRect(x, y, w, h, BLACK);
          currentlyClicked = false;
          return true;
        }
        return false;
    }

};

#endif
