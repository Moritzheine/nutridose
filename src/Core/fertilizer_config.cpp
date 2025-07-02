#include "fertilizer_config.h"

// Global instance
FertilizerConfig fertConfig;

// Central fertilizer configuration
const Fertilizer FertilizerConfig::fertilizers_[COUNT] = {
    // name,           short,    pin,                    default_ml, is_nutrient, pump_id
    {"NPK Base", "NPK", PUMP_1_NPK_BASE_PIN, 1.0, true, 0},
    {"Micro Nutrients", "Micro", PUMP_2_MICRO_PIN, 1.0, true, 1},
    {"pH Down", "pH-", PUMP_3_PH_DOWN_PIN, 1.0, true, 2},
    {"Bloom Booster", "Bloom", PUMP_4_BLOOM_PIN, 1.0, true, 3},
    {"Water", "H2O", PUMP_5_WATER_PIN, 1000.0, false, 4}};

const Fertilizer &FertilizerConfig::get(uint8_t index)
{
    if (index >= COUNT)
        index = 0;
    return fertilizers_[index];
}

const Fertilizer *FertilizerConfig::getByPumpId(uint8_t pump_id)
{
    for (uint8_t i = 0; i < COUNT; i++)
    {
        if (fertilizers_[i].pump_id == pump_id)
        {
            return &fertilizers_[i];
        }
    }
    return nullptr;
}

const Fertilizer *FertilizerConfig::getAll()
{
    return fertilizers_;
}

uint8_t FertilizerConfig::getNutrientCount()
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < COUNT; i++)
    {
        if (fertilizers_[i].is_nutrient)
            count++;
    }
    return count;
}

const Fertilizer &FertilizerConfig::getNutrient(uint8_t index)
{
    uint8_t nutrient_idx = 0;
    for (uint8_t i = 0; i < COUNT; i++)
    {
        if (fertilizers_[i].is_nutrient)
        {
            if (nutrient_idx == index)
            {
                return fertilizers_[i];
            }
            nutrient_idx++;
        }
    }
    // Fallback to first
    return fertilizers_[0];
}