#pragma once
#include "ui/base_screen.h"

class ProfileListScreen : public BaseScreen
{
public:
    ProfileListScreen(ScreenManager *manager) : BaseScreen(manager) {}

    void enter() override;
    void update() override;
    void handleInput(InputEvent event) override;

private:
    void refreshDisplay();
};