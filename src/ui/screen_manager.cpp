#include "screen_manager.h"
#include "screens/main_screen.h"
#include "screens/menu_screen.h"
#include "screens/dose_screen.h"
#include "screens/calibration_screen.h"
#include "screens/progress_screen.h"
#include "config.h"
#include "utils/logger.h"

ScreenManager screenManager;

void ScreenManager::begin()
{
    display.begin();
    input.begin();

    // Initialize all screens
    screens_[(int)ScreenType::MAIN] = new MainScreen(this);
    screens_[(int)ScreenType::MENU] = new MenuScreen(this);
    screens_[(int)ScreenType::DOSE_SELECT] = new DoseSelectScreen(this);
    screens_[(int)ScreenType::DOSE_AMOUNT] = new DoseAmountScreen(this);
    screens_[(int)ScreenType::DOSE_CONFIRM] = new DoseConfirmScreen(this);
    screens_[(int)ScreenType::PROGRESS] = new ProgressScreen(this);
    screens_[(int)ScreenType::COMPLETE] = new CompleteScreen(this);
    screens_[(int)ScreenType::CALIBRATION_SELECT] = new CalibrationSelectScreen(this);
    screens_[(int)ScreenType::CALIBRATION_RUN] = new CalibrationRunScreen(this);
    screens_[(int)ScreenType::CALIBRATION_INPUT] = new CalibrationInputScreen(this);

    switchTo(ScreenType::MAIN);
    Logger::info("Screen Manager initialized");
}

void ScreenManager::update()
{
    input.update();

    if (checkTimeout())
    {
        returnToMain();
        return;
    }

    InputEvent event = input.getEvent();
    if (event != InputEvent::NONE)
    {
        updateActivity();
    }

    if (current_screen_)
    {
        current_screen_->update();
        if (event != InputEvent::NONE)
        {
            current_screen_->handleInput(event);
        }
    }
}

void ScreenManager::switchTo(ScreenType type)
{
    if (current_screen_)
    {
        current_screen_->exit();
    }

    current_screen_ = screens_[(int)type];
    if (current_screen_)
    {
        current_screen_->enter();
        updateActivity();
    }
}

void ScreenManager::returnToMain()
{
    switchTo(ScreenType::MAIN);
}

void ScreenManager::updateActivity()
{
    last_activity_ = millis();
}

bool ScreenManager::checkTimeout()
{
    if (current_screen_ == screens_[(int)ScreenType::MAIN] ||
        current_screen_ == screens_[(int)ScreenType::PROGRESS])
    {
        return false;
    }

    return (millis() - last_activity_) > MENU_TIMEOUT_MS;
}