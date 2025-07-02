#include "profile_water_screen.h"
#include "ui/screen_manager.h"
#include "hardware/display.h"

void ProfileWaterScreen::enter()
{
    enter_time_ = millis();
    display.showValue("Water Amount", manager_->profile_ctx.water_ml, "ml");
}

void ProfileWaterScreen::update() {}

void ProfileWaterScreen::handleInput(InputEvent event)
{
    if (event == InputEvent::ENCODER_UP)
    {
        manager_->profile_ctx.water_ml += 100;
        if (manager_->profile_ctx.water_ml > 5000)
        {
            manager_->profile_ctx.water_ml = 5000;
        }
        display.showValue("Water Amount", manager_->profile_ctx.water_ml, "ml");
    }
    else if (event == InputEvent::ENCODER_DOWN)
    {
        if (manager_->profile_ctx.water_ml >= 100)
        {
            manager_->profile_ctx.water_ml -= 100;
        }
        else
        {
            manager_->profile_ctx.water_ml = 0;
        }
        display.showValue("Water Amount", manager_->profile_ctx.water_ml, "ml");
    }
    else if (event == InputEvent::BUTTON_CLICK)
    {
        manager_->switchTo(ScreenType::PROFILE_SUMMARY);
    }
    else if (event == InputEvent::BUTTON_HOLD)
    {
        manager_->switchTo(ScreenType::PROFILE_DETAIL);
    }
}