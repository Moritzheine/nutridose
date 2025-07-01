#pragma once
#include "ui/base_screen.h"

class MenuScreen : public BaseScreen
{
public:
    MenuScreen(ScreenManager *manager) : BaseScreen(manager) {}

    void enter() override;
    void update() override;
    void handleInput(InputEvent event) override;
};