#include "Button.h"
#include "Bitmaps/lightbulb_b.h"
#include "Bitmaps/lightbulb_w.h"

bool Button::touchInside(int touchX, int touchY) {
  auto x2 = x + w;
  auto y2 = y + h;
  return touchX >= x && touchX <= x2 && touchY >= y && touchY <= y2;
}

void Button::printText() {
  display.setTextSize(textSize);
  display.setCursor(x + h + ICON_TEXT_PADDING,
                    y + (h / 2 + FONT_HEIGHT/2));
  display.print(txt);
}

void Button::draw() {
  display.fillRoundRect(x, y, w, h, roundRadius, WHITE);    // inner
  display.drawRoundRect(x, y, w, h, roundRadius, BLACK);    // outer line
  display.fillRoundRect(x, y, h, h, roundRadius, BLACK);    // icon bg
  display.drawBitmap(x + (h/2 - ICON_SIZE/2), 
                     y + (h/2 - ICON_SIZE/2), 
                     lightbulb_w, 
                     ICON_SIZE, 
                     ICON_SIZE, 
                     WHITE);   // icon
  display.setFont(&FreeSansBold12pt7b);  
  display.setTextColor(BLACK);
  printText();   // text
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
      if (!currentlyClicked) {
        Serial.println("Touch as click for btn[" + String(instanceNr) + "]");
        this->drawInverted();
        currentlyClicked = true;
        
        callback();
        touchTime = now;
        return true;
      }
      touchTime = now;
  }
  else if (currentlyClicked && now - touchTime > 600) {
    Serial.println("Touch timeout for btn[" + String(instanceNr) + "]");
    this->draw();
    currentlyClicked = false;
    return true;
  }
  return false;
}
