#include "progress_screen.h"
#include "ui/screen_manager.h"
#include "hardware/display.h"
#include "hardware/pump_driver.h"
#include "core/fertilizer_config.h"
#include "core/calibration.h"

extern PumpDriver pumpDriver;
extern Calibration calibration;

// --- ProgressScreen ---
void ProgressScreen::enter()
{
    enter_time_ = millis();
}

void ProgressScreen::update()
{
    uint8_t progress = calculateProgress();

    const auto &fert = manager_->dose_ctx.nutrients_only ? fertConfig.getNutrient(manager_->dose_ctx.fertilizer_index) : fertConfig.get(manager_->dose_ctx.fertilizer_index);

    String title = "Dosing " + String(fert.short_name);
    display.showProgress(title, progress);

    if (progress >= 100)
    {
        pumpDriver.stopPump(fert.pump_id);
        manager_->switchTo(ScreenType::COMPLETE);
    }
}

void ProgressScreen::handleInput(InputEvent event)
{
    if (event == InputEvent::BUTTON_HOLD)
    {
        pumpDriver.stopAllPumps();
        manager_->returnToMain();
    }
}

uint8_t ProgressScreen::calculateProgress()
{
    const auto &fert = manager_->dose_ctx.nutrients_only ? fertConfig.getNutrient(manager_->dose_ctx.fertilizer_index) : fertConfig.get(manager_->dose_ctx.fertilizer_index);

    uint32_t elapsed = millis() - manager_->dose_ctx.start_time;
    uint32_t target_time = calibration.calculatePumpTime(fert.pump_id, manager_->dose_ctx.amount_ml);

    if (elapsed >= target_time)
        return 100;
    return (elapsed * 100) / target_time;
}

// --- CompleteScreen ---
void CompleteScreen::enter()
{
    enter_time_ = millis();

    display.clear();
    display.printCenter(20, "Dosing");
    display.printCenter(35, "Complete!");
    display.show();
}

void CompleteScreen::update()
{
    // Auto return after 3 seconds
    if (millis() - enter_time_ > 3000)
    {
        manager_->returnToMain();
    }
}

void CompleteScreen::handleInput(InputEvent event)
{
    if (event == InputEvent::BUTTON_CLICK)
    {
        manager_->returnToMain();
    }
}