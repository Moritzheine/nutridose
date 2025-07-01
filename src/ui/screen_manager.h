#pragma once
#include "base_screen.h"
#include "hardware/display.h"
#include "hardware/input.h"
#include <memory>

struct DoseContext
{
    uint8_t fertilizer_index = 0;
    uint8_t amount_ml = 10;
    uint32_t start_time = 0;
    bool nutrients_only = true;
};

struct CalibrationContext
{
    uint8_t pump_index = 0;
    uint8_t actual_volume = 50;
    uint32_t start_time = 0;
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
    uint8_t menu_selection = 0;

private:
    BaseScreen *current_screen_ = nullptr;
    BaseScreen *screens_[10];
    uint32_t last_activity_ = 0;

    void updateActivity();
    bool checkTimeout();
};

extern ScreenManager screenManager;