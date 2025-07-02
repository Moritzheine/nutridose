#include "profile_dose_screen.h"
#include "ui/screen_manager.h"
#include "hardware/display.h"
#include "hardware/pump_driver.h"
#include "core/profile_manager.h"
#include "core/fertilizer_config.h"
#include "core/calibration.h"

extern PumpDriver pumpDriver;
extern ProfileManager profileManager;
extern Calibration calibration;

void ProfileDoseScreen::enter()
{
    enter_time_ = millis();
    manager_->profile_ctx.start_time = millis();
    startNextPump();
}

void ProfileDoseScreen::update()
{
    if (isPumpComplete())
    {
        // Stop current pump
        pumpDriver.stopAllPumps();

        // Move to next pump
        manager_->profile_ctx.current_pump++;

        // Check if all pumps done
        if (manager_->profile_ctx.current_pump >= 5)
        { // 4 fertilizers + water
            manager_->switchTo(ScreenType::COMPLETE);
            return;
        }

        // Start next pump
        startNextPump();
    }

    // Update display
    uint8_t progress = calculateOverallProgress();

    String title = "Profile " + String(char('A' + manager_->profile_ctx.selected_profile));
    display.showProgress(title, progress);
}

void ProfileDoseScreen::handleInput(InputEvent event)
{
    if (event == InputEvent::BUTTON_HOLD)
    {
        // Emergency stop
        pumpDriver.stopAllPumps();
        manager_->returnToMain();
    }
}

void ProfileDoseScreen::startNextPump()
{
    const Profile &profile = profileManager.getProfile(manager_->profile_ctx.selected_profile);

    // Skip pumps with 0 amount
    while (manager_->profile_ctx.current_pump < 5)
    {
        if (manager_->profile_ctx.current_pump < 4)
        {
            // Fertilizer pump
            current_amount_ = profile.ratios[manager_->profile_ctx.current_pump];
        }
        else
        {
            // Water pump
            current_amount_ = manager_->profile_ctx.water_ml;
        }

        if (current_amount_ > 0)
        {
            break;
        }

        manager_->profile_ctx.current_pump++;
    }

    // Check if we're done
    if (manager_->profile_ctx.current_pump >= 5)
    {
        manager_->switchTo(ScreenType::COMPLETE);
        return;
    }

    // Start the pump
    pump_start_time_ = millis();

    if (manager_->profile_ctx.current_pump < 4)
    {
        // Start fertilizer pump
        pumpDriver.startPump(manager_->profile_ctx.current_pump);
    }
    else
    {
        // Start water pump (pump 4)
        pumpDriver.startPump(4);
    }
}

bool ProfileDoseScreen::isPumpComplete()
{
    uint32_t elapsed = millis() - pump_start_time_;
    uint32_t target_time = calibration.calculatePumpTime(
        manager_->profile_ctx.current_pump < 4 ? manager_->profile_ctx.current_pump : 4,
        current_amount_);

    if (elapsed >= target_time)
    {
        // Update total elapsed time
        manager_->profile_ctx.elapsed_time_ms += target_time;
        return true;
    }

    return false;
}

uint8_t ProfileDoseScreen::calculateOverallProgress()
{
    uint32_t current_pump_elapsed = millis() - pump_start_time_;
    uint32_t total_elapsed = manager_->profile_ctx.elapsed_time_ms + current_pump_elapsed;

    if (total_elapsed >= manager_->profile_ctx.total_time_ms)
    {
        return 100;
    }

    return (total_elapsed * 100) / manager_->profile_ctx.total_time_ms;
}