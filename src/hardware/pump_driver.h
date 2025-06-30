#pragma once
#include <Arduino.h>
#include "config.h"

class PumpDriver
{
public:
    // Public access to pump configuration
    static const uint8_t PUMP_PINS[5];
    static const char *PUMP_NAMES[5];

    void begin();
    bool startPump(uint8_t pump_id);
    bool stopPump(uint8_t pump_id);
    void stopAllPumps();
    bool isPumpRunning(uint8_t pump_id);
    void update(); // Safety check
    void printStatus();

private:
    bool pump_states_[NUM_TOTAL_PUMPS] = {false};
    uint32_t pump_start_times_[NUM_TOTAL_PUMPS] = {0};
};