#include "main_screen.h"
#include "ui/screen_manager.h"
#include "hardware/display.h"
#include "hardware/pump_driver.h"
#include "core/fertilizer_config.h"

extern PumpDriver pumpDriver;

void MainScreen::enter()
{
    enter_time_ = millis();
    last_update_ = 0;
}

void MainScreen::update()
{
    if (millis() - last_update_ > 2000)
    {
        display.clear();

        display.printCenter(5, "NutriDose v1.0");
        display.printCenter(15, "Ready");

        display.printLine(4, "5 Pumps Online");
        display.printLine(5, "Click for Menu");

        display.printCenter(55, "Turn=Test Click=Menu");

        display.show();
        last_update_ = millis();
    }
}

void MainScreen::handleInput(InputEvent event)
{
    if (event == InputEvent::BUTTON_CLICK)
    {
        manager_->menu_selection = 0;
        manager_->switchTo(ScreenType::MENU);
    }
    else if (event == InputEvent::ENCODER_UP || event == InputEvent::ENCODER_DOWN)
    {
        // Quick pump test
        if (event == InputEvent::ENCODER_UP)
        {
            test_pump_ = (test_pump_ + 1) % NUM_TOTAL_PUMPS;
        }
        else
        {
            test_pump_ = (test_pump_ == 0) ? NUM_TOTAL_PUMPS - 1 : test_pump_ - 1;
        }

        const auto &fert = fertConfig.get(test_pump_);
        display.clear();
        display.printCenter(20, "Test Pump " + String(test_pump_));
        display.printCenter(30, fert.name);
        display.printCenter(40, "Quick Pulse");
        display.show();

        pumpDriver.startPump(test_pump_);
        delay(500);
        pumpDriver.stopPump(test_pump_);
    }
}