#include "profile_detail_screen.h"
#include "ui/screen_manager.h"
#include "hardware/display.h"
#include "core/profile_manager.h"

extern ProfileManager profileManager;

// --- ProfileDetailScreen ---
void ProfileDetailScreen::enter()
{
    enter_time_ = millis();

    const Profile &profile = profileManager.getProfile(manager_->profile_ctx.selected_profile);
    String ratios = profileManager.getRatioString(manager_->profile_ctx.selected_profile);

    display.clear();

    // Title
    display.printCenter(5, "Profile " + String(profile.name));

    // Ratios with clear unit indication
    display.printCenter(20, ratios + " ml/L");
    display.printCenter(30, "(per Liter water)");

    display.show();
}

void ProfileDetailScreen::handleInput(InputEvent event)
{
    if (event == InputEvent::BUTTON_CLICK)
    {
        // Use profile - go to water amount selection
        manager_->profile_ctx.water_ml = 1000; // Default 1L
        manager_->switchTo(ScreenType::PROFILE_WATER);
    }
    else if (event == InputEvent::BUTTON_HOLD)
    {
        // Delete profile - go to confirmation
        manager_->switchTo(ScreenType::PROFILE_DELETE_CONFIRM);
    }
}

// --- ProfileDeleteConfirmScreen ---
void ProfileDeleteConfirmScreen::enter()
{
    enter_time_ = millis();

    const Profile &profile = profileManager.getProfile(manager_->profile_ctx.selected_profile);
    String detail = profileManager.getRatioString(manager_->profile_ctx.selected_profile);

    display.clear();
    display.printCenter(10, "Delete Profile " + String(profile.name) + "?");
    display.printCenter(25, detail + " ml/L");
    display.show();
}

void ProfileDeleteConfirmScreen::handleInput(InputEvent event)
{
    if (event == InputEvent::BUTTON_CLICK)
    {
        // Confirm deletion
        profileManager.deleteProfile(manager_->profile_ctx.selected_profile);
        manager_->switchTo(ScreenType::PROFILE_LIST);
    }
    else if (event == InputEvent::BUTTON_HOLD)
    {
        // Cancel - zurück zur Detail-Ansicht
        manager_->switchTo(ScreenType::PROFILE_DETAIL);
    }
}