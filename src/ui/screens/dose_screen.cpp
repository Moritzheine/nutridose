#include "dose_screen.h"
#include "ui/screen_manager.h"
#include "hardware/display.h"
#include "hardware/pump_driver.h"
#include "core/fertilizer_config.h"

extern PumpDriver pumpDriver;

// --- DoseSelectScreen ---
void DoseSelectScreen::enter()
{
    enter_time_ = millis();
    refresh();
}

void DoseSelectScreen::refresh()
{

    uint8_t count = manager_->dose_ctx.nutrients_only ? fertConfig.getNutrientCount() : fertConfig.COUNT;
    String items[count];

    for (uint8_t i = 0; i < count; i++)
    {
        const auto &fert = manager_->dose_ctx.nutrients_only ? fertConfig.getNutrient(i) : fertConfig.get(i);
        items[i] = fert.name;
    }

    display.showMenu("Select Fertilizer", items, count, manager_->dose_ctx.fertilizer_index);
}

void DoseSelectScreen::handleInput(InputEvent event)
{
    uint8_t count = manager_->dose_ctx.nutrients_only ? fertConfig.getNutrientCount() : fertConfig.COUNT;

    if (event == InputEvent::ENCODER_UP)
    {
        manager_->dose_ctx.fertilizer_index = navigate(manager_->dose_ctx.fertilizer_index, count, true);
        refresh();
    }
    else if (event == InputEvent::ENCODER_DOWN)
    {
        manager_->dose_ctx.fertilizer_index = navigate(manager_->dose_ctx.fertilizer_index, count, false);
        refresh();
    }
    else if (event == InputEvent::BUTTON_CLICK)
    {
        const auto &fert = manager_->dose_ctx.nutrients_only ? fertConfig.getNutrient(manager_->dose_ctx.fertilizer_index) : fertConfig.get(manager_->dose_ctx.fertilizer_index);
        manager_->dose_ctx.amount_ml = fert.default_dose_ml;
        manager_->switchTo(ScreenType::DOSE_AMOUNT);
    }
    else if (event == InputEvent::BUTTON_HOLD)
    {
        manager_->switchTo(ScreenType::MENU);
    }
}

// --- DoseConfirmScreen ---
void DoseConfirmScreen::enter()
{
    enter_time_ = millis();
    const auto &fert = manager_->dose_ctx.nutrients_only ? fertConfig.getNutrient(manager_->dose_ctx.fertilizer_index) : fertConfig.get(manager_->dose_ctx.fertilizer_index);

    display.clear();
    display.printCenter(5, "Confirm Dose");
    display.printLine(2, "Fertilizer:");
    display.printLine(3, "  " + String(fert.name));
    display.printLine(4, "Amount:");

    // Format amount with 0.1ml precision
    String amountStr = (manager_->dose_ctx.amount_ml == (int)manager_->dose_ctx.amount_ml) ? String((int)manager_->dose_ctx.amount_ml) : String(manager_->dose_ctx.amount_ml, 1);
    display.printLine(5, "  " + amountStr + "ml");

    display.printCenter(55, "Click=Start Hold=Back");
    display.show();
}

void DoseConfirmScreen::update() {}

void DoseConfirmScreen::handleInput(InputEvent event)
{
    if (event == InputEvent::BUTTON_CLICK)
    {
        const auto &fert = manager_->dose_ctx.nutrients_only ? fertConfig.getNutrient(manager_->dose_ctx.fertilizer_index) : fertConfig.get(manager_->dose_ctx.fertilizer_index);

        manager_->dose_ctx.start_time = millis();
        pumpDriver.startPump(fert.pump_id);
        manager_->switchTo(ScreenType::PROGRESS);
    }
    else if (event == InputEvent::BUTTON_HOLD)
    {
        manager_->switchTo(ScreenType::DOSE_AMOUNT);
    }
}