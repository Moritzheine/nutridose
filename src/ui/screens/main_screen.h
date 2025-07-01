#pragma once
#include "ui/base_screen.h"

class MainScreen : public BaseScreen
{
public:
    MainScreen(ScreenManager *manager) : BaseScreen(manager) {}

    void enter() override;
    void update() override;
    void handleInput(InputEvent event) override;

private:
    uint32_t last_update_ = 0;
    uint8_t test_pump_ = 0;
};