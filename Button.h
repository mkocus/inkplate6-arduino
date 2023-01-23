#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "Inkplate.h"
#include "Fonts/FreeSansBold12pt7b.h"

typedef void (*CallbackType)();

static int btnInstanceNr = 0;

const int ICON_TEXT_PADDING = 15;
const int FONT_HEIGHT = 12;
const int ICON_SIZE = 48;

class Button {
private:
    int x;
    int y;
    int w;
    int h;
    const char* txt;
    Inkplate& display;
    CallbackType callback;
    int touchTime = 0;
    bool currentlyClicked = false;
    int instanceNr;
    int textSize = 1;
    int roundRadius = 15;

    bool touchInside(int touchX, int touchY);

    void drawInverted();
    void printText();

public:
    Button(Inkplate& disp, const char* txt, int x, int y, int width, int height, CallbackType callback) 
      : display(disp), txt(txt), x(x), y(y), h(height), callback(callback) { 
      instanceNr = btnInstanceNr++; 
      roundRadius = height/2;

      // calculate button length dynamically, based on font sizze
      int16_t xx, yy;
      uint16_t ww, hh;
      display.setTextSize(textSize);
      display.setFont(&FreeSansBold12pt7b);  
      display.getTextBounds(txt, x, y, &xx, &yy, &ww, &hh);
      w = height + ICON_TEXT_PADDING + ww + height/2;   // icon + icon/text padding + text + round radius
    }

    void draw();

    /** @returns true if partial update is needed, false otherwise */
    bool loop(int touchX, int touchY, bool touched);
};

#endif
