#pragma once
#include "ui/base_screen.h"

class ProfileDetailScreen : public BaseScreen
{
public:
    ProfileDetailScreen(ScreenManager *manager) : BaseScreen(manager) {}

    void enter() override;
    void update() override;
    void handleInput(InputEvent event) override;

private:
    bool nav_selection_ = true; // true = right (Next), false = left (Back)
};

class ProfileDeleteConfirmScreen : public BaseScreen
{
public:
    ProfileDeleteConfirmScreen(ScreenManager *manager) : BaseScreen(manager) {}

    void enter() override;
    void update() override;
    void handleInput(InputEvent event) override;
};