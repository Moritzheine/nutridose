#pragma once
#include <Arduino.h>
#include "config.h"

// ZENTRALE STORAGE-STRUKTUR
struct AppData
{
    // Calibration data
    float pump_factors[NUM_TOTAL_PUMPS] = {1.0, 1.0, 1.0, 1.0, 1.0};

    // NEW: Small volume offset calibration (in milliseconds)
    uint16_t pump_offset_ms[NUM_TOTAL_PUMPS] = {650, 650, 650, 650, 650};

    // Profile data
    struct ProfileData
    {
        char name;
        float ratios[4];
        bool active;
    } profiles[4];

    // Last doses for each pump
    float last_doses[NUM_TOTAL_PUMPS] = {10.0, 5.0, 2.0, 8.0, 1000.0};

    // Data version for migration
    uint16_t version = 2; // Increased for offset calibration
};

class StorageManager
{
public:
    void begin();

    // CALIBRATION
    void saveCalibration(uint8_t pump_id, float factor);
    float getCalibrationFactor(uint8_t pump_id);

    // NEW: Small volume offset calibration
    void saveOffsetCalibration(uint8_t pump_id, uint16_t offset_ms);
    uint16_t getOffsetCalibration(uint8_t pump_id);

    // PROFILES
    void saveProfile(uint8_t index, const float ratios[4]);
    void deleteProfile(uint8_t index);
    bool isProfileActive(uint8_t index);
    float getProfileRatio(uint8_t profile_idx, uint8_t ratio_idx);
    String getProfileRatioString(uint8_t index);
    char getProfileName(uint8_t index);

    // LAST DOSES
    void saveLastDose(uint8_t pump_id, float amount);
    float getLastDose(uint8_t pump_id);

private:
    AppData data_;

    void loadFromFlash();
    void saveToFlash();
    void initDefaults();
};

extern StorageManager storage;