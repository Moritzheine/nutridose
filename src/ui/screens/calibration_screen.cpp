#include "calibration_screen.h"
#include "ui/screen_manager.h"
#include "hardware/display.h"
#include "hardware/pump_driver.h"
#include "core/fertilizer_config.h"
#include "core/calibration.h"

extern PumpDriver pumpDriver;
extern Calibration calibration;

// --- CalibrationSelectScreen ---
void CalibrationSelectScreen::enter()
{
    enter_time_ = millis();

    display.clear();
    display.printCenter(5, "Calibration");
    display.printCenter(15, "Select Pump");

    const auto &fert = fertConfig.get(manager_->cal_ctx.pump_index);
    display.printLine(4, "Pump:");
    display.printLine(5, "  " + String(manager_->cal_ctx.pump_index) + " - " + fert.name);

    display.printCenter(55, "Click=Start Hold=Back");
    display.show();
}

void CalibrationSelectScreen::update() {}

void CalibrationSelectScreen::handleInput(InputEvent event)
{
    if (event == InputEvent::ENCODER_UP)
    {
        manager_->cal_ctx.pump_index = (manager_->cal_ctx.pump_index + 1) % fertConfig.COUNT;
        enter(); // Refresh display
    }
    else if (event == InputEvent::ENCODER_DOWN)
    {
        manager_->cal_ctx.pump_index = (manager_->cal_ctx.pump_index == 0) ? fertConfig.COUNT - 1 : manager_->cal_ctx.pump_index - 1;
        enter(); // Refresh display
    }
    else if (event == InputEvent::BUTTON_CLICK)
    {
        manager_->cal_ctx.start_time = millis();
        manager_->switchTo(ScreenType::CALIBRATION_RUN);
    }
    else if (event == InputEvent::BUTTON_HOLD)
    {
        manager_->switchTo(ScreenType::MENU);
    }
}

// --- CalibrationRunScreen ---
void CalibrationRunScreen::enter()
{
    enter_time_ = millis();
    pumpDriver.startPump(manager_->cal_ctx.pump_index);
}

void CalibrationRunScreen::update()
{
    uint32_t elapsed = millis() - enter_time_;

    uint32_t target_time = calibration.calculatePumpTime(manager_->cal_ctx.pump_index, 50); // 50ml target
    uint8_t progress = (elapsed * 100) / target_time;

    if (progress > 100)
        progress = 100;

    display.clear();
    display.printCenter(5, "Calibrating");
    display.printCenter(15, "Pumping 50ml...");
    display.printCenter(25, String(progress) + "%");
    display.show();

    if (progress >= 100)
    {
        pumpDriver.stopPump(manager_->cal_ctx.pump_index);
        manager_->cal_ctx.actual_volume = 50;
        manager_->switchTo(ScreenType::CALIBRATION_INPUT);
    }
}

void CalibrationRunScreen::handleInput(InputEvent event)
{
    if (event == InputEvent::BUTTON_HOLD)
    {
        pumpDriver.stopAllPumps();
        manager_->returnToMain();
    }
}

// --- CalibrationInputScreen ---
void CalibrationInputScreen::enter()
{
    enter_time_ = millis();

    display.clear();
    display.printCenter(5, "Calibration");
    display.printCenter(15, "Actual Volume?");
    display.printCenter(35, String(manager_->cal_ctx.actual_volume) + "ml");
    display.printCenter(55, "Click=Save Hold=Cancel");
    display.show();
}

void CalibrationInputScreen::update() {}

void CalibrationInputScreen::handleInput(InputEvent event)
{
    if (event == InputEvent::ENCODER_UP)
    {
        if (manager_->cal_ctx.actual_volume < 99)
            manager_->cal_ctx.actual_volume++;
        enter(); // Refresh display
    }
    else if (event == InputEvent::ENCODER_DOWN)
    {
        if (manager_->cal_ctx.actual_volume > 1)
            manager_->cal_ctx.actual_volume--;
        enter(); // Refresh display
    }
    else if (event == InputEvent::BUTTON_CLICK)
    {
        // get previuous precision factor
        float previous_factor = calibration.getCalibrationFactor(manager_->cal_ctx.pump_index);

        float factor = (float)manager_->cal_ctx.actual_volume / 50.0;
        calibration.saveCalibration(manager_->cal_ctx.pump_index, factor * previous_factor);
        manager_->switchTo(ScreenType::COMPLETE);
    }
    else if (event == InputEvent::BUTTON_HOLD)
    {
        manager_->returnToMain();
    }
}