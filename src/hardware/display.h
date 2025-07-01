#pragma once
#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include "config.h"

class Display
{
public:
    Display();
    void begin();
    void clear();
    void show();

    // Text functions
    void printLine(uint8_t line, const String &text);
    void printCenter(uint8_t y, const String &text);

    // UI elements
    void showMenu(const String &title, const String items[], uint8_t count, uint8_t selected);
    void showValue(const String &label, uint8_t value, const String &unit);
    void showProgress(const String &title, uint8_t percent);

    // Direct drawing access (for custom screens)
    Adafruit_SSD1306 *getOled() { return &oled_; }

private:
    Adafruit_SSD1306 oled_;
};

// Global display object
extern Display display;