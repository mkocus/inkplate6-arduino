#include "Button.h"
#include "Fonts/FreeSansBold12pt7b.h"

bool Button::touchInside(int touchX, int touchY) {
  auto x2 = x + w;
  auto y2 = y + h;
  return touchX >= x && touchX <= x2 && touchY >= y && touchY <= y2;
}

void Button::printText() {
  display.setTextSize(textSize);
  // display.setCursor(x + (w / 2) - (strlen(txt) * 3 * textSize),
  //                   y + (h / 2) - (4 * textSize));
  display.setCursor(x + (w / 2) - (strlen(txt) * 6),
                    y + (h / 2));

  display.print(txt);
}

void Button::draw() {
  display.fillRoundRect(x, y, w, h, roundRadius, WHITE);
  display.drawRoundRect(x, y, w, h, roundRadius, BLACK);
  display.setFont(&FreeSansBold12pt7b);
  display.setTextColor(BLACK);
  this->printText();
}

void Button::drawInverted() {
  display.fillRoundRect(x, y, w, h, roundRadius, BLACK);
  display.setTextColor(WHITE);
  this->printText();
}

bool Button::loop(int touchX, int touchY, bool touched) {
  auto now = millis();

  if (touched && touchInside(touchX, touchY)) {
      Serial.println("Touch in area btn[" + String(instanceNr) + "]");
      if (now - touchTime > 300) {
        Serial.println("Touch as click for btn[" + String(instanceNr) + "]");
        this->drawInverted();
        currentlyClicked = true;
        
        callback();
        touchTime = now;
        return true;
      }
      touchTime = now;
  }
  else if (currentlyClicked && now - touchTime > 300) {
    Serial.println("Touch timeout for btn[" + String(instanceNr) + "]");
    this->draw();
    currentlyClicked = false;
    return true;
  }
  return false;
}
