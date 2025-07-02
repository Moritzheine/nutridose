#pragma once
#include <Arduino.h>
#include "config.h"

class Calibration
{
public:
    void begin();
    void saveCalibration(uint8_t pump_id, float factor);
    float getCalibrationFactor(uint8_t pump_id);
    uint32_t calculatePumpTime(uint8_t pump_id, float target_ml);
    void resetToDefaults();

private:
    static const uint32_t DEFAULT_ML_PER_SEC = 3; // 3ml per second base
};

extern Calibration calibration;