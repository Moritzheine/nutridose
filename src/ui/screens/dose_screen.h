#pragma once
#include "ui/base_screen.h"

// Fertilizer selection screen
class DoseSelectScreen : public BaseScreen
{
public:
    DoseSelectScreen(ScreenManager *manager) : BaseScreen(manager) {}

    void enter() override;
    void update() override;
    void handleInput(InputEvent event) override;
};

// Amount setting screen
class DoseAmountScreen : public BaseScreen
{
public:
    DoseAmountScreen(ScreenManager *manager) : BaseScreen(manager) {}

    void enter() override;
    void update() override;
    void handleInput(InputEvent event) override;
};

// Confirmation screen
class DoseConfirmScreen : public BaseScreen
{
public:
    DoseConfirmScreen(ScreenManager *manager) : BaseScreen(manager) {}

    void enter() override;
    void update() override;
    void handleInput(InputEvent event) override;
};