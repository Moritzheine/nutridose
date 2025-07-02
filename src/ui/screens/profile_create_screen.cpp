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

void ProfileCreateScreen::update() {}

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

        // Show value with 0.1ml precision
        float value = manager_->profile_ctx.new_ratios[manager_->profile_ctx.create_step];
        String valueStr = (value == (int)value) ? String((int)value) : String(value, 1);
        display.printCenter(35, valueStr + " ml");

        // Navigation buttons
        String leftText = (manager_->profile_ctx.create_step > 0) ? "< Back" : "Cancel";
        display.showNavigation(leftText, "Next >", manager_->profile_ctx.nav_selection);
    }
    else
    {
        // Summary before save
        display.printLine(2, "Summary:");
        String ratio_str = "";
        for (uint8_t i = 0; i < 4; i++)
        {
            if (i > 0)
                ratio_str += ":";
            float val = manager_->profile_ctx.new_ratios[i];
            ratio_str += (val == (int)val) ? String((int)val) : String(val, 1);
        }
        display.printCenter(30, ratio_str + " ml");

        // Navigation for save
        display.showNavigation("< Back", "Save", manager_->profile_ctx.nav_selection);
    }

    display.show();
}

void ProfileCreateScreen::handleInput(InputEvent event)
{
    if (manager_->profile_ctx.create_step < 4)
    {
        // Adjusting fertilizer values
        if (event == InputEvent::ENCODER_UP)
        {
            if (manager_->profile_ctx.nav_selection)
            {
                // Adjusting value
                float &value = manager_->profile_ctx.new_ratios[manager_->profile_ctx.create_step];
                value += 0.1;
                if (value > 50.0)
                    value = 50.0;
                refreshDisplay();
            }
            else
            {
                // Switch navigation
                manager_->profile_ctx.nav_selection = true;
                refreshDisplay();
            }
        }
        else if (event == InputEvent::ENCODER_DOWN)
        {
            if (manager_->profile_ctx.nav_selection)
            {
                // Adjusting value
                float &value = manager_->profile_ctx.new_ratios[manager_->profile_ctx.create_step];
                value -= 0.1;
                if (value < 0)
                    value = 0;
                refreshDisplay();
            }
            else
            {
                // Switch navigation
                manager_->profile_ctx.nav_selection = false;
                refreshDisplay();
            }
        }
        else if (event == InputEvent::BUTTON_CLICK)
        {
            if (manager_->profile_ctx.nav_selection)
            {
                // Next fertilizer
                manager_->profile_ctx.create_step++;
                refreshDisplay();
            }
            else
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
        else if (event == InputEvent::BUTTON_HOLD)
        {
            // Quick exit
            manager_->switchTo(ScreenType::PROFILE_LIST);
        }
    }
    else
    {
        // Summary screen - only navigation
        if (event == InputEvent::ENCODER_UP || event == InputEvent::ENCODER_DOWN)
        {
            manager_->profile_ctx.nav_selection = !manager_->profile_ctx.nav_selection;
            refreshDisplay();
        }
        else if (event == InputEvent::BUTTON_CLICK)
        {
            if (manager_->profile_ctx.nav_selection)
            {
                // Save profile
                saveProfile();
                manager_->switchTo(ScreenType::PROFILE_LIST);
            }
            else
            {
                // Back to last fertilizer
                manager_->profile_ctx.create_step = 3;
                refreshDisplay();
            }
        }
        else if (event == InputEvent::BUTTON_HOLD)
        {
            manager_->switchTo(ScreenType::PROFILE_LIST);
        }
    }
}

void ProfileCreateScreen::saveProfile()
{
    profileManager.saveProfile(
        manager_->profile_ctx.selected_profile,
        manager_->profile_ctx.new_ratios);
}