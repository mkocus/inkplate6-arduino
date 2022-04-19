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
    const char* txt;
    Inkplate& display;
    CallbackType callback;
    int touchTime = 0;
    bool currentlyClicked = false;
    int instanceNr;
    int textSize = 4;

    bool touchInside(int touchX, int touchY);

    void drawInverted();
    void printText();

public:
    Button(Inkplate& disp, const char* txt, int x, int y, int width, int height, CallbackType callback) 
      : display(disp), txt(txt), x(x), y(y), w(width), h(height), callback(callback) { instanceNr = btnInstanceNr++; }

    void draw();

    /** @returns true if partial update is needed, false otherwise */
    bool loop(int touchX, int touchY, bool touched);
};

#endif
