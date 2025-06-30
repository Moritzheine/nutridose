#include "calibration.h"
#include "utils/logger.h"
#include <LittleFS.h>

Calibration calibration;

void Calibration::begin()
{
    if (!LittleFS.begin())
    {
        Logger::error("LittleFS mount failed");
        return;
    }

    loadFromStorage();
    Logger::info("Calibration system initialized");
}

void Calibration::saveCalibration(uint8_t pump_id, float factor)
{
    if (pump_id >= NUM_TOTAL_PUMPS)
        return;

    pump_factors_[pump_id] = factor;
    saveToStorage();

    Logger::info("Pump " + String(pump_id) + " calibrated: " + String(factor, 3));
}

float Calibration::getCalibrationFactor(uint8_t pump_id)
{
    if (pump_id >= NUM_TOTAL_PUMPS)
        return 1.0;
    return pump_factors_[pump_id];
}

uint32_t Calibration::calculatePumpTime(uint8_t pump_id, uint8_t target_ml)
{
    if (pump_id >= NUM_TOTAL_PUMPS)
        return 0;

    float factor = pump_factors_[pump_id];
    uint32_t base_time = target_ml * DEFAULT_ML_PER_SEC;

    return (uint32_t)(base_time / factor);
}

void Calibration::resetToDefaults()
{
    for (uint8_t i = 0; i < NUM_TOTAL_PUMPS; i++)
    {
        pump_factors_[i] = 1.0;
    }
    saveToStorage();
    Logger::info("Calibration reset to defaults");
}

void Calibration::loadFromStorage()
{
    File file = LittleFS.open("/cal.dat", "r");
    if (!file)
    {
        Logger::info("No calibration file, using defaults");
        return;
    }

    for (uint8_t i = 0; i < NUM_TOTAL_PUMPS; i++)
    {
        if (file.available() >= sizeof(float))
        {
            file.read((uint8_t *)&pump_factors_[i], sizeof(float));
        }
    }

    file.close();
    Logger::info("Calibration loaded from storage");
}

void Calibration::saveToStorage()
{
    File file = LittleFS.open("/cal.dat", "w");
    if (!file)
    {
        Logger::error("Failed to save calibration");
        return;
    }

    for (uint8_t i = 0; i < NUM_TOTAL_PUMPS; i++)
    {
        file.write((uint8_t *)&pump_factors_[i], sizeof(float));
    }

    file.close();
}