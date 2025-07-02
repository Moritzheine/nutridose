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

    // VEREINFACHTE UI - NUR BESTEHENDE + EINE NEUE FUNKTION
    void showMenu(const String &title, const String items[], uint8_t count, uint8_t selected);
    void showValue(const String &label, float value, const String &unit);
    void showProgress(const String &title, uint8_t percent);
    void showProfileList(const String profiles[], uint8_t selected);

    // EINE NEUE: Konsistente Footer-Anweisungen
    void showFooter(const String &instruction);

    // Direct drawing access
    Adafruit_SSD1306 *getOled() { return &oled_; }

private:
    Adafruit_SSD1306 oled_;

    // Consistent UI elements
    void drawNavFooter(const String &instruction);
    void drawDualNavButtons(const String &left, const String &right, bool right_selected);
    void drawProgressBar(uint8_t y, uint8_t percent);
};

extern Display display;