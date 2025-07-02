#include "dose_amount_screen.h"
#include "ui/screen_manager.h"
#include "hardware/display.h"
#include "core/fertilizer_config.h"

void DoseAmountScreen::enter()
{
    enter_time_ = millis();
    nav_selection_ = true; // Default to value adjustment
    refresh();
}

void DoseAmountScreen::refresh()
{
    const auto &fert = manager_->dose_ctx.nutrients_only ? fertConfig.getNutrient(manager_->dose_ctx.fertilizer_index) : fertConfig.get(manager_->dose_ctx.fertilizer_index);

    display.clear();

    // Title
    display.printCenter(10, fert.name);

    // Value
    // display.setTextSize(2);
    String valueStr = (manager_->dose_ctx.amount_ml == (int)manager_->dose_ctx.amount_ml) ? String((int)manager_->dose_ctx.amount_ml) : String(manager_->dose_ctx.amount_ml, 1);
    display.printCenter(25, valueStr + " ml");
    // display.setTextSize(1);

    // Navigation
    display.showNavigation("< Back", "Next >", nav_selection_);

    display.show();
}

void DoseAmountScreen::handleInput(InputEvent event)
{
    if (event == InputEvent::ENCODER_UP)
    {
        if (nav_selection_)
        {
            // Adjust value
            manager_->dose_ctx.amount_ml = adjustValueFloat(manager_->dose_ctx.amount_ml, 0.1, 50.0, 0.1, true);
            refresh();
        }
        else
        {
            // Switch to value adjustment
            nav_selection_ = true;
            refresh();
        }
    }
    else if (event == InputEvent::ENCODER_DOWN)
    {
        if (nav_selection_)
        {
            // Adjust value
            manager_->dose_ctx.amount_ml = adjustValueFloat(manager_->dose_ctx.amount_ml, 0.1, 50.0, 0.1, false);
            refresh();
        }
        else
        {
            // Already on back button
            nav_selection_ = false;
            refresh();
        }
    }
    else if (event == InputEvent::BUTTON_CLICK)
    {
        if (nav_selection_)
        {
            // Next - go to confirm
            manager_->switchTo(ScreenType::DOSE_CONFIRM);
        }
        else
        {
            // Back
            manager_->switchTo(ScreenType::DOSE_SELECT);
        }
    }
    else if (event == InputEvent::BUTTON_HOLD)
    {
        manager_->switchTo(ScreenType::DOSE_SELECT);
    }
}