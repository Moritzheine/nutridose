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

    display.clear();

    // Title
    display.printCenter(5, "Profile " + String(profile.name));

    // Ratios
    String ratios = profileManager.getRatioString(manager_->profile_ctx.selected_profile);
    display.printCenter(25, ratios + " ml");

    // Navigation
    display.showNavigation("Back", "Next >", nav_selection_);

    display.show();
}

void ProfileDetailScreen::update()
{
    // Static display
}

void ProfileDetailScreen::handleInput(InputEvent event)
{
    if (event == InputEvent::ENCODER_UP || event == InputEvent::ENCODER_DOWN)
    {
        nav_selection_ = !nav_selection_;
        enter(); // Refresh display
    }
    else if (event == InputEvent::BUTTON_CLICK)
    {
        if (nav_selection_)
        {
            // Next - go to water amount
            manager_->profile_ctx.water_ml = 100; // Default
            manager_->switchTo(ScreenType::PROFILE_WATER);
        }
        else
        {
            // Back
            manager_->switchTo(ScreenType::PROFILE_LIST);
        }
    }
    else if (event == InputEvent::BUTTON_HOLD)
    {
        manager_->switchTo(ScreenType::PROFILE_LIST);
    }
}

// --- ProfileDeleteConfirmScreen ---
void ProfileDeleteConfirmScreen::enter()
{
    enter_time_ = millis();

    const Profile &profile = profileManager.getProfile(manager_->profile_ctx.selected_profile);

    display.clear();

    display.printCenter(10, "Delete Profile " + String(profile.name) + "?");
    display.printCenter(25, profileManager.getRatioString(manager_->profile_ctx.selected_profile));

    display.printCenter(45, "Click=Delete");
    display.printCenter(55, "Hold=Cancel");

    display.show();
}

void ProfileDeleteConfirmScreen::update() {}

void ProfileDeleteConfirmScreen::handleInput(InputEvent event)
{
    if (event == InputEvent::BUTTON_CLICK)
    {
        // Delete profile
        profileManager.deleteProfile(manager_->profile_ctx.selected_profile);
        manager_->switchTo(ScreenType::PROFILE_LIST);
    }
    else if (event == InputEvent::BUTTON_HOLD)
    {
        // Cancel
        manager_->switchTo(ScreenType::PROFILE_LIST);
    }
}