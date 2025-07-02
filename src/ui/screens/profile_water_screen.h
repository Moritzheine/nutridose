#pragma once
#include "ui/base_screen.h"

class ProfileWaterScreen : public BaseScreen
{
public:
    ProfileWaterScreen(ScreenManager *manager) : BaseScreen(manager) {}

    void enter() override;
    void update() override;
    void handleInput(InputEvent event) override;
};