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

// NEW: Small volume offset calibration
class CalibrationOffsetScreen : public BaseScreen
{
public:
    CalibrationOffsetScreen(ScreenManager *manager) : BaseScreen(manager) {}

    void enter() override;
    void update() override;
    void handleInput(InputEvent event) override;

private:
    uint32_t repetition_start_time_ = 0;
    bool is_pumping_ = false;
    uint32_t next_auto_start_ = 0;
    
    void refreshDisplay();
    void startNextRepetition();
};