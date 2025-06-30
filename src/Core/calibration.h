#pragma once
#include <Arduino.h>
#include "config.h"

class Calibration
{
public:
    void begin();
    void saveCalibration(uint8_t pump_id, float factor);
    float getCalibrationFactor(uint8_t pump_id);
    uint32_t calculatePumpTime(uint8_t pump_id, uint8_t target_ml);
    void resetToDefaults();

private:
    float pump_factors_[NUM_TOTAL_PUMPS] = {1.0, 1.0, 1.0, 1.0, 1.0};
    static const uint32_t DEFAULT_ML_PER_SEC = 1000; // 1ml per second base

    void loadFromStorage();
    void saveToStorage();
};

extern Calibration calibration;