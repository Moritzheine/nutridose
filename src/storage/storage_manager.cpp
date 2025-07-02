#include "storage_manager.h"
#include "utils/logger.h"
#include <LittleFS.h>

StorageManager storage;

void StorageManager::begin()
{
    if (!LittleFS.begin(false))
    {
        Logger::error("LittleFS mount failed, formatting");
        if (!LittleFS.begin(true))
        {
            Logger::error("LittleFS format failed");
            return;
        }
    }

    initDefaults();
    loadFromFlash();
    Logger::info("Storage Manager initialized");
}

void StorageManager::initDefaults()
{
    // Profile defaults
    for (uint8_t i = 0; i < 4; i++)
    {
        data_.profiles[i].name = 'A' + i;
        data_.profiles[i].active = false;
        for (uint8_t j = 0; j < 4; j++)
        {
            data_.profiles[i].ratios[j] = 0;
        }
    }
}

void StorageManager::loadFromFlash()
{
    File file = LittleFS.open("/appdata.dat", "r");
    if (!file)
    {
        Logger::info("No storage file, using defaults");
        return;
    }

    // Check version
    uint16_t version;
    if (file.read((uint8_t *)&version, sizeof(version)) != sizeof(version) || version != data_.version)
    {
        file.close();
        Logger::info("Storage version mismatch, using defaults");
        return;
    }

    // Read complete data structure
    if (file.read((uint8_t *)&data_, sizeof(data_)) == sizeof(data_))
    {
        Logger::info("Storage data loaded");
    }

    file.close();
}

void StorageManager::saveToFlash()
{
    File file = LittleFS.open("/appdata.dat", "w");
    if (!file)
    {
        Logger::error("Failed to save storage");
        return;
    }

    file.write((uint8_t *)&data_, sizeof(data_));
    file.close();
}

// CALIBRATION METHODS
void StorageManager::saveCalibration(uint8_t pump_id, float factor)
{
    if (pump_id >= NUM_TOTAL_PUMPS)
        return;

    data_.pump_factors[pump_id] = factor;
    saveToFlash();
    Logger::info("Pump " + String(pump_id) + " calibrated: " + String(factor, 3));
}

float StorageManager::getCalibrationFactor(uint8_t pump_id)
{
    if (pump_id >= NUM_TOTAL_PUMPS)
        return 1.0;
    return data_.pump_factors[pump_id];
}

// PROFILE METHODS
void StorageManager::saveProfile(uint8_t index, const float ratios[4])
{
    if (index >= 4)
        return;

    data_.profiles[index].active = true;
    for (uint8_t i = 0; i < 4; i++)
    {
        data_.profiles[index].ratios[i] = ratios[i];
    }

    saveToFlash();
    Logger::info("Profile " + String(data_.profiles[index].name) + " saved");
}

void StorageManager::deleteProfile(uint8_t index)
{
    if (index >= 4)
        return;

    data_.profiles[index].active = false;
    for (uint8_t i = 0; i < 4; i++)
    {
        data_.profiles[index].ratios[i] = 0;
    }

    saveToFlash();
    Logger::info("Profile " + String(data_.profiles[index].name) + " deleted");
}

bool StorageManager::isProfileActive(uint8_t index)
{
    if (index >= 4)
        return false;
    return data_.profiles[index].active;
}

float StorageManager::getProfileRatio(uint8_t profile_idx, uint8_t ratio_idx)
{
    if (profile_idx >= 4 || ratio_idx >= 4)
        return 0;
    return data_.profiles[profile_idx].ratios[ratio_idx];
}

String StorageManager::getProfileRatioString(uint8_t index)
{
    if (index >= 4 || !data_.profiles[index].active)
    {
        return "Empty";
    }

    String result = "";
    for (uint8_t i = 0; i < 4; i++)
    {
        if (i > 0)
            result += ":";
        float val = data_.profiles[index].ratios[i];
        if (val == (int)val)
        {
            result += String((int)val);
        }
        else
        {
            result += String(val, 1);
        }
    }
    return result;
}

char StorageManager::getProfileName(uint8_t index)
{
    if (index >= 4)
        return 'A';
    return data_.profiles[index].name;
}

// DOSE METHODS
void StorageManager::saveLastDose(uint8_t pump_id, float amount)
{
    if (pump_id >= NUM_TOTAL_PUMPS)
        return;

    data_.last_doses[pump_id] = amount;
    saveToFlash();
}

float StorageManager::getLastDose(uint8_t pump_id)
{
    if (pump_id >= NUM_TOTAL_PUMPS)
        return 10.0;
    return data_.last_doses[pump_id];
}