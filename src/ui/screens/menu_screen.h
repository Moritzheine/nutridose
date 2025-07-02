#pragma once
#include "ui/base_screen.h"

class MenuScreen : public BaseScreen
{
    uint8_t menu_selection_ = 0; // Current menu selection
public:
    MenuScreen(ScreenManager *manager) : BaseScreen(manager) {}

    void enter() override;
    void update() override;
    void handleInput(InputEvent event) override;
};