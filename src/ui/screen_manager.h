#pragma once
#include "base_screen.h"
#include "hardware/display.h"
#include "hardware/input.h"
#include <memory>

struct DoseContext
{
    uint8_t fertilizer_index = 0;
    float amount_ml = 10.0; // 0.1ml precision
    uint32_t start_time = 0;
    bool nutrients_only = true;
};

struct CalibrationContext
{
    uint8_t pump_index = 0;
    uint8_t actual_volume = 50;
    uint32_t start_time = 0;

    // NEW: For offset calibration
    uint8_t offset_repetitions = 10;
    uint8_t current_repetition = 0;
    float offset_target_ml = 1.0;     // Changed from 2.0 to 1.0ml
    float offset_actual_total = 60.0; // Expected total: 50ml base + 10x1ml = 60ml

    // NEW: Feedback for user
    uint16_t old_offset_ms = 0;
    int16_t correction_ms = 0;
    uint16_t new_offset_ms = 0;
};

struct ProfileContext
{
    uint8_t selected_profile = 0; // 0-3 for A-D
    uint16_t water_ml = 1000;     // Water amount (100ml steps)
    uint8_t current_pump = 0;     // For sequential dosing
    uint32_t start_time = 0;
    uint16_t total_time_ms = 0;   // Total time for all pumps
    uint16_t elapsed_time_ms = 0; // Elapsed time across all pumps

    // For profile creation
    uint8_t create_step = 0;                     // 0-3 for each fertilizer
    float new_ratios[4] = {10.0, 5.0, 2.0, 8.0}; // Default values
    bool nav_selection = true;                   // true = right button, false = left
};

class ScreenManager
{
public:
    void begin();
    void update();
    void switchTo(ScreenType type);
    void returnToMain();

    // Shared data between screens
    DoseContext dose_ctx;
    CalibrationContext cal_ctx;
    ProfileContext profile_ctx;
    uint8_t menu_selection = 0;

private:
    BaseScreen *current_screen_ = nullptr;
    BaseScreen *screens_[21]; // Increased for offset calibration screen
    uint32_t last_activity_ = 0;

    void updateActivity();
    bool checkTimeout();
};

extern ScreenManager screenManager;