#pragma once
#include <Arduino.h>
#include "config.h"

class PumpDriver
{
    const uint8_t PUMP_PINS[5] = {
        PUMP_1_NPK_BASE_PIN,
        PUMP_2_MICRO_PIN,
        PUMP_3_PH_DOWN_PIN,
        PUMP_4_BLOOM_PIN,
        PUMP_5_WATER_PIN};

    const char *PUMP_NAMES[5] = {
        "NPK Base",
        "Micro",
        "pH Down",
        "Bloom",
        "Water"};

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