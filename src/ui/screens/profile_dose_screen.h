#pragma once
#include "ui/base_screen.h"

class ProfileDoseScreen : public BaseScreen
{
public:
    ProfileDoseScreen(ScreenManager *manager) : BaseScreen(manager) {}

    void enter() override;
    void update() override;
    void handleInput(InputEvent event) override;

private:
    uint32_t pump_start_time_ = 0;
    float current_amount_ = 0;

    void startNextPump();
    bool isPumpComplete();
    uint8_t calculateOverallProgress();
};