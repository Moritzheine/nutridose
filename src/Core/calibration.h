#pragma once
#include <Arduino.h>
#include "config.h"

class Calibration
{
public:
    void begin();

    // Base calibration (for large volumes)
    void saveCalibration(uint8_t pump_id, float factor);
    float getCalibrationFactor(uint8_t pump_id);

    // NEW: Small volume offset calibration
    void saveOffsetCalibration(uint8_t pump_id, uint16_t offset_ms);
    uint16_t getOffsetCalibration(uint8_t pump_id);

    // Smart pump time calculation with offset compensation
    uint32_t calculatePumpTime(uint8_t pump_id, float target_ml);

    void resetToDefaults();

private:
    static const uint32_t DEFAULT_ML_PER_SEC = 3;        // 3ml per second base
    static constexpr float SMALL_VOLUME_THRESHOLD = 5.0; // Below this: use offset compensation
};

extern Calibration calibration;