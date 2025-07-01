#pragma once
#include "ui/base_screen.h"

// Progress/Dosing screen
class ProgressScreen : public BaseScreen
{
public:
    ProgressScreen(ScreenManager *manager) : BaseScreen(manager) {}

    void enter() override;
    void update() override;
    void handleInput(InputEvent event) override;

private:
    uint8_t calculateProgress();
};

// Complete screen
class CompleteScreen : public BaseScreen
{
public:
    CompleteScreen(ScreenManager *manager) : BaseScreen(manager) {}

    void enter() override;
    void update() override;
    void handleInput(InputEvent event) override;
};