#include "ui_controller.h"
#include "menu_system.h"
#include "hardware/pump_driver.h"
#include "core/calibration.h"
#include "utils/logger.h"

UIController uiController;

void UIController::begin()
{
    display.begin();
    input.begin();

    current_state_ = UIState::MAIN_SCREEN;
    updateActivity();

    Logger::info("UI Controller initialized");
}

void UIController::update()
{
    input.update();

    // Timeout check - return to main screen after inactivity
    if (current_state_ != UIState::MAIN_SCREEN &&
        current_state_ != UIState::DOSING &&
        (millis() - last_activity_) > MENU_TIMEOUT_MS)
    {
        resetToMain();
        return;
    }

    // Handle current state
    switch (current_state_)
    {
    case UIState::MAIN_SCREEN:
        handleMainScreen();
        break;
    case UIState::MAIN_MENU:
        handleMainMenu();
        break;
    case UIState::FERTILIZER_SELECT:
        handleFertilizerSelect();
        break;
    case UIState::AMOUNT_SET:
        handleAmountSet();
        break;
    case UIState::CONFIRM_DOSE:
        handleConfirmDose();
        break;
    case UIState::DOSING:
        handleDosing();
        break;
    case UIState::COMPLETE:
        handleComplete();
        break;
    case UIState::CALIBRATION_PUMP_SELECT:
        handleCalibrationPumpSelect();
        break;
    case UIState::CALIBRATION_RUNNING:
        handleCalibrationRunning();
        break;
    case UIState::CALIBRATION_INPUT:
        handleCalibrationInput();
        break;
    case UIState::CALIBRATION_COMPLETE:
        handleCalibrationComplete();
        break;
    }
}

void UIController::handleMainScreen()
{
    static uint32_t last_update = 0;

    // Update display every 2 seconds
    if (millis() - last_update > 2000)
    {
        display.clear();

        // Title
        display.printCenter(5, "NutriDose v1.0");
        display.printCenter(15, "Ready");

        // Status info
        display.printLine(4, "5 Pumps Online");
        display.printLine(5, "Click for Menu");

        // Bottom instruction
        display.printCenter(55, "Turn=Test Click=Menu");

        display.show();
        last_update = millis();
    }

    InputEvent event = input.getEvent();
    if (event == InputEvent::BUTTON_CLICK)
    {
        current_state_ = UIState::MAIN_MENU;
        selected_item_ = 0;
        updateActivity();
    }
    else if (event == InputEvent::ENCODER_UP || event == InputEvent::ENCODER_DOWN)
    {
        // Quick pump test from main screen
        static uint8_t test_pump = 0;
        if (event == InputEvent::ENCODER_UP)
        {
            test_pump = (test_pump + 1) % NUM_TOTAL_PUMPS;
        }
        else
        {
            test_pump = (test_pump == 0) ? NUM_TOTAL_PUMPS - 1 : test_pump - 1;
        }

        display.clear();
        display.printCenter(20, "Test Pump " + String(test_pump));
        display.printCenter(35, "Quick Pulse");
        display.show();

        // Quick 500ms pulse for testing
        extern PumpDriver pumpDriver;
        pumpDriver.startPump(test_pump);
        delay(500);
        pumpDriver.stopPump(test_pump);

        updateActivity();
    }
}

void UIController::handleMainMenu()
{
    static bool menu_displayed = false;

    if (!menu_displayed)
    {
        Menu::showMainMenu(selected_item_);
        menu_displayed = true;
    }

    InputEvent event = input.getEvent();
    if (event == InputEvent::ENCODER_UP)
    {
        selected_item_ = Menu::navigate(selected_item_, 4, true);
        Menu::showMainMenu(selected_item_);
        updateActivity();
    }
    else if (event == InputEvent::ENCODER_DOWN)
    {
        selected_item_ = Menu::navigate(selected_item_, 4, false);
        Menu::showMainMenu(selected_item_);
        updateActivity();
    }
    else if (event == InputEvent::BUTTON_CLICK)
    {
        if (selected_item_ == 0)
        { // Manual Dose
            current_state_ = UIState::FERTILIZER_SELECT;
            selected_fertilizer_ = 0;
        }
        else if (selected_item_ == 1)
        { // Profiles (not implemented)
            resetToMain();
        }
        else if (selected_item_ == 2)
        { // Calibration
            current_state_ = UIState::CALIBRATION_PUMP_SELECT;
            selected_fertilizer_ = 0;
        }
        else if (selected_item_ == 3)
        { // Settings
            resetToMain();
        }
        else
        { // About
            resetToMain();
        }
        menu_displayed = false;
        updateActivity();
    }
    else if (event == InputEvent::BUTTON_HOLD)
    {
        resetToMain();
        menu_displayed = false;
    }
}

void UIController::handleFertilizerSelect()
{
    static bool menu_displayed = false;

    if (!menu_displayed)
    {
        Menu::showFertilizerSelect(selected_fertilizer_);
        menu_displayed = true;
    }

    InputEvent event = input.getEvent();
    if (event == InputEvent::ENCODER_UP)
    {
        selected_fertilizer_ = Menu::navigate(selected_fertilizer_, 4, true);
        Menu::showFertilizerSelect(selected_fertilizer_);
        updateActivity();
    }
    else if (event == InputEvent::ENCODER_DOWN)
    {
        selected_fertilizer_ = Menu::navigate(selected_fertilizer_, 4, false);
        Menu::showFertilizerSelect(selected_fertilizer_);
        updateActivity();
    }
    else if (event == InputEvent::BUTTON_CLICK)
    {
        current_state_ = UIState::AMOUNT_SET;
        dose_amount_ = 10; // Default 10ml
        menu_displayed = false;
        updateActivity();
    }
    else if (event == InputEvent::BUTTON_HOLD)
    {
        current_state_ = UIState::MAIN_MENU;
        menu_displayed = false;
        updateActivity();
    }
}

void UIController::handleAmountSet()
{
    static bool screen_displayed = false;

    if (!screen_displayed)
    {
        Menu::showAmountSet(selected_fertilizer_, dose_amount_);
        screen_displayed = true;
    }

    InputEvent event = input.getEvent();
    if (event == InputEvent::ENCODER_UP)
    {
        dose_amount_ = Menu::adjustValue(dose_amount_, 1, 50, true);
        Menu::showAmountSet(selected_fertilizer_, dose_amount_);
        updateActivity();
    }
    else if (event == InputEvent::ENCODER_DOWN)
    {
        dose_amount_ = Menu::adjustValue(dose_amount_, 1, 50, false);
        Menu::showAmountSet(selected_fertilizer_, dose_amount_);
        updateActivity();
    }
    else if (event == InputEvent::BUTTON_CLICK)
    {
        current_state_ = UIState::CONFIRM_DOSE;
        screen_displayed = false;
        updateActivity();
    }
    else if (event == InputEvent::BUTTON_HOLD)
    {
        current_state_ = UIState::FERTILIZER_SELECT;
        screen_displayed = false;
        updateActivity();
    }
}

void UIController::handleConfirmDose()
{
    static bool screen_displayed = false;

    if (!screen_displayed)
    {
        Menu::showConfirm(selected_fertilizer_, dose_amount_);
        screen_displayed = true;
    }

    InputEvent event = input.getEvent();
    if (event == InputEvent::BUTTON_CLICK)
    {
        current_state_ = UIState::DOSING;
        dose_start_time_ = millis();
        screen_displayed = false;

        // Start actual dosing
        extern PumpDriver pumpDriver;
        pumpDriver.startPump(selected_fertilizer_);

        updateActivity();
    }
    else if (event == InputEvent::BUTTON_HOLD)
    {
        current_state_ = UIState::AMOUNT_SET;
        screen_displayed = false;
        updateActivity();
    }
}

void UIController::handleDosing()
{
    uint8_t progress = calculateDoseProgress();
    Menu::showDosing(selected_fertilizer_, progress);

    // Check if dosing complete
    if (progress >= 100)
    {
        extern PumpDriver pumpDriver;
        pumpDriver.stopPump(selected_fertilizer_);

        current_state_ = UIState::COMPLETE;
        updateActivity();
    }

    // Emergency stop
    InputEvent event = input.getEvent();
    if (event == InputEvent::BUTTON_HOLD)
    {
        extern PumpDriver pumpDriver;
        pumpDriver.stopAllPumps();
        resetToMain();
    }
}

void UIController::handleComplete()
{
    static uint32_t complete_start = 0;

    if (complete_start == 0)
    {
        Menu::showComplete();
        complete_start = millis();
    }

    // Auto return to main after 3 seconds
    if (millis() - complete_start > 3000)
    {
        complete_start = 0;
        resetToMain();
    }

    // Or manual return
    InputEvent event = input.getEvent();
    if (event == InputEvent::BUTTON_CLICK)
    {
        complete_start = 0;
        resetToMain();
    }
}

void UIController::resetToMain()
{
    current_state_ = UIState::MAIN_SCREEN;
    updateActivity();
}

void UIController::updateActivity()
{
    last_activity_ = millis();
}

uint8_t UIController::calculateDoseProgress()
{
    uint32_t elapsed = millis() - dose_start_time_;
    uint32_t target_time = calibration.calculatePumpTime(selected_fertilizer_, dose_amount_);

    if (elapsed >= target_time)
        return 100;
    return (elapsed * 100) / target_time;
}

void UIController::handleCalibrationPumpSelect()
{
    static bool menu_displayed = false;

    if (!menu_displayed)
    {
        display.clear();
        display.printCenter(5, "Calibration");
        display.printCenter(15, "Select Pump");

        // Show selected pump
        display.printLine(4, "Pump:");
        display.printLine(5, "  " + String(selected_fertilizer_) + " - " + PumpDriver::PUMP_NAMES[selected_fertilizer_]);

        display.printCenter(55, "Click=Start Hold=Back");
        display.show();
        menu_displayed = true;
    }

    InputEvent event = input.getEvent();
    if (event == InputEvent::ENCODER_UP)
    {
        selected_fertilizer_ = (selected_fertilizer_ + 1) % NUM_TOTAL_PUMPS;
        menu_displayed = false;
        updateActivity();
    }
    else if (event == InputEvent::ENCODER_DOWN)
    {
        selected_fertilizer_ = (selected_fertilizer_ == 0) ? NUM_TOTAL_PUMPS - 1 : selected_fertilizer_ - 1;
        menu_displayed = false;
        updateActivity();
    }
    else if (event == InputEvent::BUTTON_CLICK)
    {
        current_state_ = UIState::CALIBRATION_RUNNING;
        dose_start_time_ = millis();

        // Start 50ml calibration dose
        extern PumpDriver pumpDriver;
        pumpDriver.startPump(selected_fertilizer_);

        menu_displayed = false;
        updateActivity();
    }
    else if (event == InputEvent::BUTTON_HOLD)
    {
        current_state_ = UIState::MAIN_MENU;
        menu_displayed = false;
        updateActivity();
    }
}

void UIController::handleCalibrationRunning()
{
    uint32_t elapsed = millis() - dose_start_time_;
    uint32_t target_time = 50000; // 50 seconds for 50ml at 1ml/s
    uint8_t progress = (elapsed * 100) / target_time;

    if (progress > 100)
        progress = 100;

    display.clear();
    display.printCenter(5, "Calibrating");
    display.printCenter(15, "Pumping 50ml...");

    // Progress bar
    uint8_t barWidth = 100;
    uint8_t barX = 14;
    uint8_t barY = 30;
    display.printCenter(25, String(progress) + "%");

    display.show();

    // Check if complete
    if (progress >= 100)
    {
        extern PumpDriver pumpDriver;
        pumpDriver.stopPump(selected_fertilizer_);

        current_state_ = UIState::CALIBRATION_INPUT;
        actual_volume_ = 50; // Default assumption
        updateActivity();
    }

    // Emergency stop
    InputEvent event = input.getEvent();
    if (event == InputEvent::BUTTON_HOLD)
    {
        extern PumpDriver pumpDriver;
        pumpDriver.stopAllPumps();
        resetToMain();
    }
}

void UIController::handleCalibrationInput()
{
    static bool screen_displayed = false;

    if (!screen_displayed)
    {
        display.clear();
        display.printCenter(5, "Calibration");
        display.printCenter(15, "Actual Volume?");

        // Show current input
        display.printCenter(35, String(actual_volume_) + "ml");

        display.printCenter(55, "Click=Save Hold=Cancel");
        display.show();
        screen_displayed = true;
    }

    InputEvent event = input.getEvent();
    if (event == InputEvent::ENCODER_UP)
    {
        if (actual_volume_ < 99)
            actual_volume_++;
        screen_displayed = false;
        updateActivity();
    }
    else if (event == InputEvent::ENCODER_DOWN)
    {
        if (actual_volume_ > 1)
            actual_volume_--;
        screen_displayed = false;
        updateActivity();
    }
    else if (event == InputEvent::BUTTON_CLICK)
    {
        // Calculate and save calibration factor
        float factor = (float)actual_volume_ / 50.0;
        calibration.saveCalibration(selected_fertilizer_, factor);

        current_state_ = UIState::CALIBRATION_COMPLETE;
        screen_displayed = false;
        updateActivity();
    }
    else if (event == InputEvent::BUTTON_HOLD)
    {
        resetToMain();
        screen_displayed = false;
    }
}

void UIController::handleCalibrationComplete()
{
    static uint32_t complete_start = 0;

    if (complete_start == 0)
    {
        display.clear();
        display.printCenter(15, "Calibration");
        display.printCenter(25, "Complete!");

        float factor = calibration.getCalibrationFactor(selected_fertilizer_);
        display.printCenter(40, "Factor: " + String(factor, 2));

        display.show();
        complete_start = millis();
    }

    // Auto return after 3 seconds
    if (millis() - complete_start > 3000)
    {
        complete_start = 0;
        resetToMain();
    }

    // Manual return
    InputEvent event = input.getEvent();
    if (event != InputEvent::NONE)
    {
        complete_start = 0;
        resetToMain();
    }
}