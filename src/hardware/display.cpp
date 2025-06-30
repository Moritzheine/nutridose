#include "display.h"
#include <Wire.h>

// Global display instance
Display display;

Display::Display() : oled_(DISPLAY_WIDTH, DISPLAY_HEIGHT, &Wire, -1)
{
}

void Display::begin()
{
    Wire.begin(DISPLAY_SDA_PIN, DISPLAY_SCL_PIN);

    if (!oled_.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println("Display init failed!");
        return;
    }

    oled_.clearDisplay();
    oled_.setTextSize(1);
    oled_.setTextColor(SSD1306_WHITE);
    oled_.display();
}

void Display::clear()
{
    oled_.clearDisplay();
}

void Display::show()
{
    oled_.display();
}

void Display::printLine(uint8_t line, const String &text)
{
    oled_.setCursor(0, line * 8);
    oled_.print(text);
}

void Display::printCenter(uint8_t y, const String &text)
{
    int16_t x1, y1;
    uint16_t w, h;
    oled_.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    uint8_t x = (DISPLAY_WIDTH - w) / 2;
    oled_.setCursor(x, y);
    oled_.print(text);
}

void Display::showMenu(const String &title, const String items[], uint8_t count, uint8_t selected)
{
    clear();

    // Title
    oled_.setTextSize(1);
    printCenter(0, title);

    // Separator line
    oled_.drawLine(0, 12, DISPLAY_WIDTH - 1, 12, SSD1306_WHITE);

    // Menu items
    for (uint8_t i = 0; i < count; i++)
    {
        uint8_t y = 20 + (i * 10);

        if (i == selected)
        {
            // Highlight selected item
            oled_.fillRect(0, y - 1, DISPLAY_WIDTH, 9, SSD1306_WHITE);
            oled_.setTextColor(SSD1306_BLACK);
            oled_.setCursor(2, y);
            oled_.print("> " + items[i]);
            oled_.setTextColor(SSD1306_WHITE);
        }
        else
        {
            oled_.setCursor(4, y);
            oled_.print(items[i]);
        }
    }

    show();
}

void Display::showValue(const String &label, uint8_t value, const String &unit)
{
    clear();

    // Label
    oled_.setTextSize(1);
    printCenter(10, label);

    // Large value
    oled_.setTextSize(2);
    String valueStr = String(value) + unit;
    printCenter(30, valueStr);

    // Instructions
    oled_.setTextSize(1);
    printCenter(55, "Turn=Adjust Click=OK");

    show();
}

void Display::showProgress(const String &title, uint8_t percent)
{
    clear();

    // Title
    oled_.setTextSize(1);
    printCenter(10, title);

    // Progress bar
    uint8_t barWidth = 100;
    uint8_t barHeight = 8;
    uint8_t barX = (DISPLAY_WIDTH - barWidth) / 2;
    uint8_t barY = 30;

    // Progress bar outline
    oled_.drawRect(barX, barY, barWidth, barHeight, SSD1306_WHITE);

    // Progress fill
    uint8_t fillWidth = (barWidth - 2) * percent / 100;
    if (fillWidth > 0)
    {
        oled_.fillRect(barX + 1, barY + 1, fillWidth, barHeight - 2, SSD1306_WHITE);
    }

    // Percentage text
    String percentStr = String(percent) + "%";
    printCenter(45, percentStr);

    show();
}