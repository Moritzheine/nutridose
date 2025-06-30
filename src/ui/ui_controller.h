#pragma once
#include <Arduino.h>
#include "hardware/input.h"
#include "hardware/display.h"

enum class UIState
{
    MAIN_SCREEN,
    MAIN_MENU,
    FERTILIZER_SELECT,
    AMOUNT_SET,
    CONFIRM_DOSE,
    DOSING,
    COMPLETE,
    CALIBRATION_PUMP_SELECT,
    CALIBRATION_RUNNING,
    CALIBRATION_INPUT,
    CALIBRATION_COMPLETE
};

class UIController
{
public:
    void begin();
    void update();

private:
    UIState current_state_ = UIState::MAIN_SCREEN;
    uint8_t selected_item_ = 0;
    uint8_t selected_fertilizer_ = 0;
    uint8_t dose_amount_ = 10;
    uint8_t actual_volume_ = 50; // Calibration input
    uint32_t last_activity_ = 0;
    uint32_t dose_start_time_ = 0;

    // State handlers
    void handleMainScreen();
    void handleMainMenu();
    void handleFertilizerSelect();
    void handleAmountSet();
    void handleConfirmDose();
    void handleDosing();
    void handleComplete();
    void handleCalibrationPumpSelect();
    void handleCalibrationRunning();
    void handleCalibrationInput();
    void handleCalibrationComplete();

    // Utilities
    void resetToMain();
    void updateActivity();
    uint8_t calculateDoseProgress();
};

extern UIController uiController;