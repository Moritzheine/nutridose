#pragma once
#include "ui/base_screen.h"

class ProfileCreateScreen : public BaseScreen
{
public:
    ProfileCreateScreen(ScreenManager *manager) : BaseScreen(manager) {}

    void enter() override;
    void update() override;
    void handleInput(InputEvent event) override;

private:
    void refreshDisplay();
    void saveProfile();
};