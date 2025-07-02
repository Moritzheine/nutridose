#pragma once
#include <Arduino.h>
#include "hardware/input.h"

enum class ScreenType
{
    MAIN,
    MENU,
    DOSE_SELECT,
    DOSE_AMOUNT,
    DOSE_CONFIRM,
    PROGRESS,
    COMPLETE,
    CALIBRATION_SELECT,
    CALIBRATION_RUN,
    CALIBRATION_INPUT,
    // Profile screens
    PROFILE_LIST,
    PROFILE_DETAIL,
    PROFILE_WATER,
    PROFILE_SUMMARY,
    PROFILE_DOSE,
    PROFILE_DELETE_CONFIRM,
    PROFILE_CREATE
};

class ScreenManager; // Forward declaration

class BaseScreen
{
public:
    BaseScreen(ScreenManager *manager) : manager_(manager) {}
    virtual ~BaseScreen() = default;

    virtual void enter() = 0;
    virtual void update() {};  // Called every loop iteration
    virtual void refresh() {}; // Optional, called when screen needs to be redrawn
    virtual void handleInput(InputEvent event) = 0;
    virtual void exit() {}

    // Navigation helpers
    static uint8_t navigate(uint8_t current, uint8_t max_items, bool up)
    {
        if (up)
        {
            return (current + 1) % max_items;
        }
        else
        {
            return (current == 0) ? max_items - 1 : current - 1;
        }
    }

    static uint8_t adjustValue(uint8_t current, uint8_t min_val, uint8_t max_val, bool up)
    {
        if (up)
        {
            return (current >= max_val) ? max_val : current + 1;
        }
        else
        {
            return (current <= min_val) ? min_val : current - 1;
        }
    }

    static float adjustValueFloat(float current, float min_val, float max_val, float step, bool up)
    {
        if (up)
        {
            current += step;
            return (current > max_val) ? max_val : current;
        }
        else
        {
            current -= step;
            return (current < min_val) ? min_val : current;
        }
    }

protected:
    ScreenManager *manager_;
    uint32_t enter_time_ = 0;
};