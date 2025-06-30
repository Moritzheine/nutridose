#pragma once
#include <Arduino.h>

class Logger
{
public:
    static void begin()
    {
        Serial.begin(115200);
        while (!Serial)
            delay(10);
    }

    static void print(const String &msg) { Serial.print(msg); }
    static void println(const String &msg) { Serial.println(msg); }

    static void info(const String &msg)
    {
        Serial.println("[INFO] " + msg);
    }

    static void error(const String &msg)
    {
        Serial.println("[ERROR] " + msg);
    }
};