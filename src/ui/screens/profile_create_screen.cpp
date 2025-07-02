#include "profile_create_screen.h"
#include "ui/screen_manager.h"
#include "hardware/display.h"
#include "core/profile_manager.h"
#include "core/fertilizer_config.h"

extern ProfileManager profileManager;

void ProfileCreateScreen::enter()
{
    enter_time_ = millis();

    // Reset creation state
    manager_->profile_ctx.create_step = 0;
    manager_->profile_ctx.nav_selection = true; // Default to Next

    // Set default values
    manager_->profile_ctx.new_ratios[0] = 10.0; // NPK
    manager_->profile_ctx.new_ratios[1] = 5.0;  // Micro
    manager_->profile_ctx.new_ratios[2] = 2.0;  // pH
    manager_->profile_ctx.new_ratios[3] = 8.0;  // Bloom

    refreshDisplay();
}

void ProfileCreateScreen::refreshDisplay()
{
    display.clear();

    // Title
    char profile_name = 'A' + manager_->profile_ctx.selected_profile;
    display.printCenter(5, "Create Profile " + String(profile_name));

    if (manager_->profile_ctx.create_step < 4)
    {
        // Show current fertilizer
        const auto &fert = fertConfig.get(manager_->profile_ctx.create_step);
        display.printCenter(20, fert.name);

        // Show value with ml/L indication
        float value = manager_->profile_ctx.new_ratios[manager_->profile_ctx.create_step];
        String valueStr = (value == (int)value) ? String((int)value) : String(value, 1);
        display.printCenter(35, valueStr + " ml/L");
    }
    else
    {
        // Summary before save
        display.printLine(2, "Summary (ml/L):");
        String ratio_str = "";
        for (uint8_t i = 0; i < 4; i++)
        {
            if (i > 0)
                ratio_str += ":";
            float val = manager_->profile_ctx.new_ratios[i];
            ratio_str += (val == (int)val) ? String((int)val) : String(val, 1);
        }
        display.printCenter(30, ratio_str);
    }

    display.show();
}

void ProfileCreateScreen::handleInput(InputEvent event)
{
    if (manager_->profile_ctx.create_step < 4)
    {
        // EINFACHE WERTE-EINGABE
        if (event == InputEvent::ENCODER_UP)
        {
            float &value = manager_->profile_ctx.new_ratios[manager_->profile_ctx.create_step];
            value += 0.1;
            if (value > 50.0)
                value = 50.0;
            refreshDisplay();
        }
        else if (event == InputEvent::ENCODER_DOWN)
        {
            float &value = manager_->profile_ctx.new_ratios[manager_->profile_ctx.create_step];
            value -= 0.1;
            if (value < 0)
                value = 0;
            refreshDisplay();
        }
        else if (event == InputEvent::BUTTON_CLICK)
        {
            // Next fertilizer
            manager_->profile_ctx.create_step++;
            refreshDisplay();
        }
        else if (event == InputEvent::BUTTON_HOLD)
        {
            // Back or Cancel
            if (manager_->profile_ctx.create_step > 0)
            {
                manager_->profile_ctx.create_step--;
                refreshDisplay();
            }
            else
            {
                manager_->switchTo(ScreenType::PROFILE_LIST);
            }
        }
    }
    else
    {
        // Summary screen
        if (event == InputEvent::BUTTON_CLICK)
        {
            // Save profile
            saveProfile();
            manager_->switchTo(ScreenType::PROFILE_LIST);
        }
        else if (event == InputEvent::BUTTON_HOLD)
        {
            // Back to last fertilizer
            manager_->profile_ctx.create_step = 3;
            refreshDisplay();
        }
    }
}

void ProfileCreateScreen::saveProfile()
{
    profileManager.saveProfile(
        manager_->profile_ctx.selected_profile,
        manager_->profile_ctx.new_ratios);
}