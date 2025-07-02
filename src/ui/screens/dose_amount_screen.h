#pragma once
#include "ui/base_screen.h"

class DoseAmountScreen : public BaseScreen
{
public:
    DoseAmountScreen(ScreenManager *manager) : BaseScreen(manager) {}

    void enter() override;
    void handleInput(InputEvent event) override;
    void refresh() override;

private:
    bool nav_selection_ = true; // true = adjust value, false = back button
};