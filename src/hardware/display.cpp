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

void Display::showValue(const String &label, float value, const String &unit)
{
    clear();

    // Label
    oled_.setTextSize(1);
    printCenter(10, label);

    // Large value
    oled_.setTextSize(2);
    int decimals = 1;

    String valueStr = String(value, decimals) + unit;
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

void Display::showProfileList(const String profiles[], uint8_t selected)
{
    clear();

    // Title
    oled_.setTextSize(1);
    printCenter(0, "Profiles");

    // Separator line
    oled_.drawLine(0, 10, DISPLAY_WIDTH - 1, 10, SSD1306_WHITE);

    // Profile items with divider lines
    for (uint8_t i = 0; i < 4; i++)
    {
        uint8_t y = 16 + (i * 12);

        // Highlight selected row
        if (i == selected)
        {
            oled_.fillRect(0, y - 2, DISPLAY_WIDTH, 12, SSD1306_WHITE);
            oled_.setTextColor(SSD1306_BLACK);
        }

        // Profile letter
        oled_.setCursor(4, y);
        oled_.print(char('A' + i));

        // Vertical divider
        if (i == selected)
        {
            oled_.drawLine(20, y - 2, 20, y + 9, SSD1306_BLACK);
        }
        else
        {
            oled_.drawLine(20, y - 2, 20, y + 9, SSD1306_WHITE);
        }

        // Profile ratios
        oled_.setCursor(26, y);
        oled_.print(profiles[i]);

        // Reset color
        if (i == selected)
        {
            oled_.setTextColor(SSD1306_WHITE);
        }

        // Horizontal divider (except for last item)
        if (i < 3)
        {
            oled_.drawLine(0, y + 10, DISPLAY_WIDTH - 1, y + 10, SSD1306_WHITE);
        }
    }

    show();
}

void Display::showNavigation(const String &left, const String &right, bool rightSelected)
{
    // Draw navigation bar at bottom
    uint8_t navY = 48;
    uint8_t boxHeight = 14;
    uint8_t boxWidth = 50;
    uint8_t spacing = 10;

    // Left button
    uint8_t leftX = spacing;
    if (!rightSelected)
    {
        oled_.fillRect(leftX - 2, navY - 2, boxWidth + 4, boxHeight + 4, SSD1306_WHITE);
        oled_.setTextColor(SSD1306_BLACK);
    }
    else
    {
        oled_.drawRect(leftX - 2, navY - 2, boxWidth + 4, boxHeight + 4, SSD1306_WHITE);
    }

    int16_t x1, y1;
    uint16_t w, h;
    oled_.getTextBounds(left, 0, 0, &x1, &y1, &w, &h);
    oled_.setCursor(leftX + (boxWidth - w) / 2, navY + 3);
    oled_.print(left);

    // Right button
    uint8_t rightX = DISPLAY_WIDTH - boxWidth - spacing;
    if (!rightSelected)
    {
        oled_.setTextColor(SSD1306_WHITE);
    }
    else
    {
        oled_.fillRect(rightX - 2, navY - 2, boxWidth + 4, boxHeight + 4, SSD1306_WHITE);
        oled_.setTextColor(SSD1306_BLACK);
    }

    oled_.drawRect(rightX - 2, navY - 2, boxWidth + 4, boxHeight + 4, SSD1306_WHITE);
    oled_.getTextBounds(right, 0, 0, &x1, &y1, &w, &h);
    oled_.setCursor(rightX + (boxWidth - w) / 2, navY + 3);
    oled_.print(right);

    oled_.setTextColor(SSD1306_WHITE);
}