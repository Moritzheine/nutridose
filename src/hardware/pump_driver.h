#pragma once
#include <Arduino.h>
#include "config.h"

class PumpDriver
{
public:
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