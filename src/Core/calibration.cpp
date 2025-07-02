#include "calibration.h"
#include "storage/storage_manager.h"
#include "utils/logger.h"

Calibration calibration;

void Calibration::begin()
{
    Logger::info("Calibration system initialized");
}

void Calibration::saveCalibration(uint8_t pump_id, float factor)
{
    storage.saveCalibration(pump_id, factor);
}

float Calibration::getCalibrationFactor(uint8_t pump_id)
{
    return storage.getCalibrationFactor(pump_id);
}

uint32_t Calibration::calculatePumpTime(uint8_t pump_id, float target_ml)
{
    if (pump_id >= NUM_TOTAL_PUMPS || target_ml <= 0)
        return 0;

    float factor = getCalibrationFactor(pump_id);
    uint32_t base_time = (uint32_t)(target_ml / DEFAULT_ML_PER_SEC) * 1000;

    return (uint32_t)(base_time / factor);
}

void Calibration::resetToDefaults()
{
    for (uint8_t i = 0; i < NUM_TOTAL_PUMPS; i++)
    {
        saveCalibration(i, 1.0);
    }
    Logger::info("Calibration reset to defaults");
}