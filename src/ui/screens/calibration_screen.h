#pragma once
#include "ui/base_screen.h"

// Pump selection for calibration
class CalibrationSelectScreen : public BaseScreen
{
public:
    CalibrationSelectScreen(ScreenManager *manager) : BaseScreen(manager) {}

    void enter() override;
    void update() override;
    void handleInput(InputEvent event) override;
};

// Calibration running
class CalibrationRunScreen : public BaseScreen
{
public:
    CalibrationRunScreen(ScreenManager *manager) : BaseScreen(manager) {}

    void enter() override;
    void update() override;
    void handleInput(InputEvent event) override;
};

// Actual volume input
class CalibrationInputScreen : public BaseScreen
{
public:
    CalibrationInputScreen(ScreenManager *manager) : BaseScreen(manager) {}

    void enter() override;
    void update() override;
    void handleInput(InputEvent event) override;
};