#pragma once
#include <Arduino.h>
#include "config.h"

// Fertilizer structure for clean configuration
struct Fertilizer
{
    const char *name;       // Display name
    const char *short_name; // Short name for small displays
    uint8_t pin;            // GPIO pin for pump
    float default_dose_ml;  // Default dosing amount (0.1ml precision)
    bool is_nutrient;       // true=fertilizer, false=water/pH
    uint8_t pump_id;        // Pump index (0-4)
};

class FertilizerConfig
{
public:
    static const uint8_t COUNT = NUM_TOTAL_PUMPS;

    // Get fertilizer by index
    static const Fertilizer &get(uint8_t index);

    // Find fertilizer by pump_id
    static const Fertilizer *getByPumpId(uint8_t pump_id);

    // Get all fertilizers array
    static const Fertilizer *getAll();

    // Get only nutrients (no water)
    static uint8_t getNutrientCount();
    static const Fertilizer &getNutrient(uint8_t index);

private:
    static const Fertilizer fertilizers_[COUNT];
};

extern FertilizerConfig fertConfig;