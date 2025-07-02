#pragma once
#include "ui/base_screen.h"

class ProfileSummaryScreen : public BaseScreen
{
public:
    ProfileSummaryScreen(ScreenManager *manager) : BaseScreen(manager) {}

    void enter() override;
    void update() override;
    void handleInput(InputEvent event) override;

private:
    void calculateTotalTime();
};