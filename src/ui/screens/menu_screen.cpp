#include "menu_screen.h"
#include "ui/screen_manager.h"
#include "hardware/display.h"

static const String MAIN_MENU_ITEMS[5] = {
    "Manual Dose",
    "Profiles",
    "Calibration",
    "Settings",
    "About"};

void MenuScreen::enter()
{
    enter_time_ = millis();
    display.showMenu("NutriDose v1.0", MAIN_MENU_ITEMS, 5, manager_->menu_selection);
}

void MenuScreen::update()
{
    // Menu is static, no update needed
}

void MenuScreen::handleInput(InputEvent event)
{
    if (event == InputEvent::ENCODER_UP)
    {
        manager_->menu_selection = navigate(manager_->menu_selection, 5, true);
        display.showMenu("NutriDose v1.0", MAIN_MENU_ITEMS, 5, manager_->menu_selection);
    }
    else if (event == InputEvent::ENCODER_DOWN)
    {
        manager_->menu_selection = navigate(manager_->menu_selection, 5, false);
        display.showMenu("NutriDose v1.0", MAIN_MENU_ITEMS, 5, manager_->menu_selection);
    }
    else if (event == InputEvent::BUTTON_CLICK)
    {
        switch (manager_->menu_selection)
        {
        case 0: // Manual Dose
            manager_->dose_ctx.fertilizer_index = 0;
            manager_->dose_ctx.nutrients_only = true;
            manager_->switchTo(ScreenType::DOSE_SELECT);
            break;
        case 1: // Profiles
            manager_->returnToMain();
            break;
        case 2: // Calibration
            manager_->cal_ctx.pump_index = 0;
            manager_->switchTo(ScreenType::CALIBRATION_SELECT);
            break;
        case 3: // Settings
        case 4: // About
            manager_->returnToMain();
            break;
        }
    }
    else if (event == InputEvent::BUTTON_HOLD)
    {
        manager_->returnToMain();
    }
}