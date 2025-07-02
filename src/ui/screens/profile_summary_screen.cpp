#include "profile_summary_screen.h"
#include "ui/screen_manager.h"
#include "hardware/display.h"
#include "core/profile_manager.h"
#include "core/calibration.h"

extern ProfileManager profileManager;
extern Calibration calibration;

void ProfileSummaryScreen::enter()
{
    enter_time_ = millis();
    calculateTotalTime();

    const Profile &profile = profileManager.getProfile(manager_->profile_ctx.selected_profile);

    display.clear();

    // Title
    display.printCenter(2, "Ready to Dose");

    // Profile info
    display.printLine(2, "Profile " + String(profile.name) + ":");
    display.printLine(3, " " + profileManager.getRatioString(manager_->profile_ctx.selected_profile));

    // Water info
    display.printLine(4, "Water: " + String(manager_->profile_ctx.water_ml) + "ml");

    // Total volume
    float total_ml = profile.getTotalMl() + manager_->profile_ctx.water_ml;
    display.printLine(5, "Total: " + String(total_ml, 1) + "ml");

    // Estimated time
    uint16_t total_sec = manager_->profile_ctx.total_time_ms / 1000;
    display.printLine(6, "Time: ~" + String(total_sec) + "s");

    display.printCenter(55, "Click=Start Hold=Back");

    display.show();
}

void ProfileSummaryScreen::update() {}

void ProfileSummaryScreen::handleInput(InputEvent event)
{
    if (event == InputEvent::BUTTON_CLICK)
    {
        // Start multi-pump sequence
        manager_->profile_ctx.current_pump = 0;
        manager_->profile_ctx.elapsed_time_ms = 0;
        manager_->switchTo(ScreenType::PROFILE_DOSE);
    }
    else if (event == InputEvent::BUTTON_HOLD)
    {
        manager_->switchTo(ScreenType::PROFILE_WATER);
    }
}

void ProfileSummaryScreen::calculateTotalTime()
{
    const Profile &profile = profileManager.getProfile(manager_->profile_ctx.selected_profile);
    uint32_t total_time = 0;

    // Calculate time for each fertilizer
    for (uint8_t i = 0; i < 4; i++)
    {
        if (profile.ratios[i] > 0)
        {
            total_time += calibration.calculatePumpTime(i, profile.ratios[i]);
        }
    }

    // Add water time
    total_time += calibration.calculatePumpTime(4, manager_->profile_ctx.water_ml);

    manager_->profile_ctx.total_time_ms = total_time;
}