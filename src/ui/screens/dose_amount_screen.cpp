#include "dose_amount_screen.h"
#include "ui/screen_manager.h"
#include "hardware/display.h"
#include "core/fertilizer_config.h"

void DoseAmountScreen::enter()
{
    enter_time_ = millis();
    refresh();
}

void DoseAmountScreen::refresh()
{
    const auto &fert = manager_->dose_ctx.nutrients_only ? fertConfig.getNutrient(manager_->dose_ctx.fertilizer_index) : fertConfig.get(manager_->dose_ctx.fertilizer_index);

    display.showValue(fert.name, manager_->dose_ctx.amount_ml, "ml");
}

void DoseAmountScreen::handleInput(InputEvent event)
{
    // EINFACHE NAVIGATION - EIN PATTERN FÜR ALLES
    if (event == InputEvent::ENCODER_UP)
    {
        manager_->dose_ctx.amount_ml = adjustValue(manager_->dose_ctx.amount_ml, 0.1f, 50.0f, 0.1f, true);
        refresh();
    }
    else if (event == InputEvent::ENCODER_DOWN)
    {
        manager_->dose_ctx.amount_ml = adjustValue(manager_->dose_ctx.amount_ml, 0.1f, 50.0f, 0.1f, false);
        refresh();
    }
    else if (event == InputEvent::BUTTON_CLICK)
    {
        manager_->switchTo(ScreenType::DOSE_CONFIRM);
    }
    else if (event == InputEvent::BUTTON_HOLD)
    {
        manager_->switchTo(ScreenType::DOSE_SELECT);
    }
}