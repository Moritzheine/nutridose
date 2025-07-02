#include "menu_screen.h"
#include "ui/screen_manager.h"
#include "hardware/display.h"

static const int MENU_ITEM_COUNT = 3;
static const String MAIN_MENU_ITEMS[MENU_ITEM_COUNT] = {
    "Manual Dose",
    "Profiles",
    "Calibration",
};

void MenuScreen::enter()
{
    enter_time_ = millis();
    display.showMenu("NutriDose v1.0", MAIN_MENU_ITEMS, MENU_ITEM_COUNT, menu_selection_);
}

void MenuScreen::update()
{
    // Menu is static, no update needed
}

void MenuScreen::handleInput(InputEvent event)
{
    if (event == InputEvent::ENCODER_UP)
    {
        menu_selection_ = navigate(menu_selection_, MENU_ITEM_COUNT, true);
        display.showMenu("NutriDose v1.0", MAIN_MENU_ITEMS, MENU_ITEM_COUNT, menu_selection_);
    }
    else if (event == InputEvent::ENCODER_DOWN)
    {
        menu_selection_ = navigate(menu_selection_, MENU_ITEM_COUNT, false);
        display.showMenu("NutriDose v1.0", MAIN_MENU_ITEMS, MENU_ITEM_COUNT, menu_selection_);
    }
    else if (event == InputEvent::BUTTON_CLICK)
    {
        switch (menu_selection_)
        {
        case 0: // Manual Dose
            // manager_->dose_ctx.fertilizer_index = 0;
            // manager_->dose_ctx.nutrients_only = true;
            manager_->switchTo(ScreenType::DOSE_SELECT);
            break;
        case 1: // Profiles
            manager_->profile_ctx.selected_profile = 0;
            manager_->switchTo(ScreenType::PROFILE_LIST);
            break;
        case 2: // Calibration
            manager_->cal_ctx.pump_index = 0;
            manager_->switchTo(ScreenType::CALIBRATION_SELECT);
            break;
        }
    }
    else if (event == InputEvent::BUTTON_HOLD)
    {
        manager_->returnToMain();
    }
}