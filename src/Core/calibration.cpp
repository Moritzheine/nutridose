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

// NEW: Small volume offset calibration
void Calibration::saveOffsetCalibration(uint8_t pump_id, uint16_t offset_ms)
{
    storage.saveOffsetCalibration(pump_id, offset_ms);
}

uint16_t Calibration::getOffsetCalibration(uint8_t pump_id)
{
    return storage.getOffsetCalibration(pump_id);
}

uint32_t Calibration::calculatePumpTime(uint8_t pump_id, float target_ml)
{
    if (pump_id >= NUM_TOTAL_PUMPS || target_ml <= 0)
        return 0;

    // Get calibration data
    float factor = getCalibrationFactor(pump_id);
    uint16_t offset_ms = getOffsetCalibration(pump_id);

    // Calculate base time
    uint32_t base_time = (uint32_t)(target_ml * 1000 / DEFAULT_ML_PER_SEC);
    uint32_t calibrated_time = (uint32_t)(base_time / factor);

    // Apply offset compensation for small volumes
    if (target_ml < SMALL_VOLUME_THRESHOLD && offset_ms > 0)
    {
        calibrated_time += offset_ms;
        Logger::info("Small volume: " + String(target_ml) + "ml + " + String(offset_ms) + "ms offset");
    }

    return calibrated_time;
}

void Calibration::resetToDefaults()
{
    for (uint8_t i = 0; i < NUM_TOTAL_PUMPS; i++)
    {
        saveCalibration(i, 1.0);
        saveOffsetCalibration(i, 0);
    }
    Logger::info("Calibration reset to defaults");
}