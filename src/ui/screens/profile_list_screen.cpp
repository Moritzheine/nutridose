#include "profile_list_screen.h"
#include "ui/screen_manager.h"
#include "hardware/display.h"
#include "core/profile_manager.h"

extern ProfileManager profileManager;

void ProfileListScreen::enter()
{
    enter_time_ = millis();
    refreshDisplay();
}

void ProfileListScreen::refreshDisplay()
{
    String profiles[4];
    for (uint8_t i = 0; i < 4; i++)
    {
        profiles[i] = profileManager.getRatioString(i);
    }
    display.showProfileList(profiles, manager_->profile_ctx.selected_profile);
}

void ProfileListScreen::handleInput(InputEvent event)
{
    // EINFACHE MENU-NAVIGATION
    if (event == InputEvent::ENCODER_UP)
    {
        manager_->profile_ctx.selected_profile = navigate(manager_->profile_ctx.selected_profile, 4, true);
        refreshDisplay();
    }
    else if (event == InputEvent::ENCODER_DOWN)
    {
        manager_->profile_ctx.selected_profile = navigate(manager_->profile_ctx.selected_profile, 4, false);
        refreshDisplay();
    }
    else if (event == InputEvent::BUTTON_CLICK)
    {
        if (profileManager.isProfileActive(manager_->profile_ctx.selected_profile))
        {
            manager_->switchTo(ScreenType::PROFILE_DETAIL);
        }
        else
        {
            manager_->switchTo(ScreenType::PROFILE_CREATE);
        }
    }
    else if (event == InputEvent::BUTTON_HOLD)
    {
        // Hold = zurück zum Menü (konsistent)
        manager_->switchTo(ScreenType::MENU);
    }
}