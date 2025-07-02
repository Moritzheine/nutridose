#include "calibration_screen.h"
#include "ui/screen_manager.h"
#include "hardware/display.h"
#include "hardware/pump_driver.h"
#include "core/fertilizer_config.h"
#include "core/calibration.h"

extern PumpDriver pumpDriver;
extern Calibration calibration;

// --- CalibrationSelectScreen ---
void CalibrationSelectScreen::enter()
{
    enter_time_ = millis();

    display.clear();
    display.printCenter(5, "Calibration");
    display.printCenter(15, "Select Pump");

    const auto &fert = fertConfig.get(manager_->cal_ctx.pump_index);
    display.printLine(4, "Pump:");
    display.printLine(5, "  " + String(manager_->cal_ctx.pump_index) + " - " + fert.name);

    display.printCenter(55, "Click=Start Hold=Back");
    display.show();
}

void CalibrationSelectScreen::update() {}

void CalibrationSelectScreen::handleInput(InputEvent event)
{
    if (event == InputEvent::ENCODER_UP)
    {
        manager_->cal_ctx.pump_index = (manager_->cal_ctx.pump_index + 1) % fertConfig.COUNT;
        enter(); // Refresh display
    }
    else if (event == InputEvent::ENCODER_DOWN)
    {
        manager_->cal_ctx.pump_index = (manager_->cal_ctx.pump_index == 0) ? fertConfig.COUNT - 1 : manager_->cal_ctx.pump_index - 1;
        enter(); // Refresh display
    }
    else if (event == InputEvent::BUTTON_CLICK)
    {
        manager_->cal_ctx.start_time = millis();
        manager_->switchTo(ScreenType::CALIBRATION_RUN);
    }
    else if (event == InputEvent::BUTTON_HOLD)
    {
        manager_->switchTo(ScreenType::MENU);
    }
}

// --- CalibrationRunScreen ---
void CalibrationRunScreen::enter()
{
    enter_time_ = millis();
    pumpDriver.startPump(manager_->cal_ctx.pump_index);
}

void CalibrationRunScreen::update()
{
    uint32_t elapsed = millis() - enter_time_;

    uint32_t target_time = calibration.calculatePumpTime(manager_->cal_ctx.pump_index, 50); // 50ml target
    uint8_t progress = (elapsed * 100) / target_time;

    if (progress > 100)
        progress = 100;

    display.clear();
    display.printCenter(5, "Calibrating");
    display.printCenter(15, "Pumping 50ml...");
    display.printCenter(25, String(progress) + "%");
    display.show();

    if (progress >= 100)
    {
        pumpDriver.stopPump(manager_->cal_ctx.pump_index);
        manager_->cal_ctx.actual_volume = 50;
        manager_->switchTo(ScreenType::CALIBRATION_INPUT);
    }
}

void CalibrationRunScreen::handleInput(InputEvent event)
{
    if (event == InputEvent::BUTTON_HOLD)
    {
        pumpDriver.stopAllPumps();
        manager_->returnToMain();
    }
}

// --- CalibrationInputScreen ---
void CalibrationInputScreen::enter()
{
    enter_time_ = millis();

    display.clear();
    display.printCenter(5, "Base Calibration");
    display.printCenter(15, "Actual Volume?");
    display.printCenter(35, String(manager_->cal_ctx.actual_volume) + "ml");
    display.printCenter(55, "Click=Save Hold=Cancel");
    display.show();
}

void CalibrationInputScreen::update() {}

void CalibrationInputScreen::handleInput(InputEvent event)
{
    if (event == InputEvent::ENCODER_UP)
    {
        if (manager_->cal_ctx.actual_volume < 99)
            manager_->cal_ctx.actual_volume++;
        enter(); // Refresh display
    }
    else if (event == InputEvent::ENCODER_DOWN)
    {
        if (manager_->cal_ctx.actual_volume > 1)
            manager_->cal_ctx.actual_volume--;
        enter(); // Refresh display
    }
    else if (event == InputEvent::BUTTON_CLICK)
    {
        // Save base calibration
        float previous_factor = calibration.getCalibrationFactor(manager_->cal_ctx.pump_index);
        float factor = (float)manager_->cal_ctx.actual_volume / 50.0;
        calibration.saveCalibration(manager_->cal_ctx.pump_index, factor * previous_factor);

        // Offer offset calibration for small volumes
        manager_->cal_ctx.current_repetition = 0;
        manager_->switchTo(ScreenType::CALIBRATION_OFFSET);
    }
    else if (event == InputEvent::BUTTON_HOLD)
    {
        manager_->returnToMain();
    }
}

// --- NEW: CalibrationOffsetScreen ---
void CalibrationOffsetScreen::enter()
{
    enter_time_ = millis();
    is_pumping_ = false;

    // Set auto-start timer for first dose
    if (manager_->cal_ctx.current_repetition == 0)
    {
        next_auto_start_ = millis() + 2000; // Start in 2 seconds
    }
    else
    {
        next_auto_start_ = 0; // No auto-start if resuming
    }

    refreshDisplay();
}

void CalibrationOffsetScreen::update()
{
    // Update countdown display
    if (manager_->cal_ctx.current_repetition == 0 && next_auto_start_ > 0 && !is_pumping_)
    {
        static uint32_t last_countdown_update = 0;
        if (millis() - last_countdown_update > 200) // Update countdown every 200ms
        {
            refreshDisplay();
            last_countdown_update = millis();
        }
    }

    // Auto-start first repetition after 2 seconds
    if (manager_->cal_ctx.current_repetition == 0 && !is_pumping_ && next_auto_start_ > 0 && millis() >= next_auto_start_)
    {
        startNextRepetition();
        next_auto_start_ = 0; // Disable auto-start
        return;
    }

    // Auto-start next repetition after 500ms pause
    if (!is_pumping_ && next_auto_start_ > 0 && millis() >= next_auto_start_ && manager_->cal_ctx.current_repetition < manager_->cal_ctx.offset_repetitions)
    {
        startNextRepetition();
        next_auto_start_ = 0;
        return;
    }

    if (is_pumping_)
    {
        uint32_t elapsed = millis() - repetition_start_time_;
        uint32_t target_time = calibration.calculatePumpTime(manager_->cal_ctx.pump_index, manager_->cal_ctx.offset_target_ml);

        if (elapsed >= target_time)
        {
            // Stop pump and advance
            pumpDriver.stopPump(manager_->cal_ctx.pump_index);
            is_pumping_ = false;
            manager_->cal_ctx.current_repetition++;

            if (manager_->cal_ctx.current_repetition >= manager_->cal_ctx.offset_repetitions)
            {
                // All done - show total volume input
                refreshDisplay();
            }
            else
            {
                // Schedule next repetition after 500ms
                next_auto_start_ = millis() + 500;
                refreshDisplay(); // Show "Next dose..." screen
            }
        }
        else
        {
            // Show progress
            uint8_t progress = (elapsed * 100) / target_time;
            display.clear();
            display.printCenter(5, "Adding 10x 2ml");
            display.printCenter(15, "Dose " + String(manager_->cal_ctx.current_repetition + 1) + "/10");
            display.printCenter(30, String(progress) + "%");
            display.printCenter(45, "Pumping...");
            display.show();
        }
    }
}

void CalibrationOffsetScreen::handleInput(InputEvent event)
{
    if (manager_->cal_ctx.current_repetition == 0 && next_auto_start_ > 0)
    {
        // Before auto-start - allow skip or immediate start
        if (event == InputEvent::BUTTON_CLICK)
        {
            // Start immediately
            startNextRepetition();
            next_auto_start_ = 0;
        }
        else if (event == InputEvent::BUTTON_HOLD)
        {
            // Skip offset calibration
            next_auto_start_ = 0;
            manager_->switchTo(ScreenType::COMPLETE);
        }
    }
    else if (manager_->cal_ctx.current_repetition >= manager_->cal_ctx.offset_repetitions)
    {
        // Total volume input
        if (event == InputEvent::ENCODER_UP)
        {
            manager_->cal_ctx.offset_actual_total += 0.5; // 0.5ml steps for easier reading
            if (manager_->cal_ctx.offset_actual_total > 100.0)
                manager_->cal_ctx.offset_actual_total = 100.0;
            refreshDisplay();
        }
        else if (event == InputEvent::ENCODER_DOWN)
        {
            manager_->cal_ctx.offset_actual_total -= 0.5;
            if (manager_->cal_ctx.offset_actual_total < 50.0) // Can't be less than base calibration
                manager_->cal_ctx.offset_actual_total = 50.0;
            refreshDisplay();
        }
        else if (event == InputEvent::BUTTON_CLICK)
        {
            // Calculate CUMULATIVE offset correction
            float expected_total = 50.0 + (manager_->cal_ctx.offset_target_ml * manager_->cal_ctx.offset_repetitions); // 70ml expected
            float actual_total = manager_->cal_ctx.offset_actual_total;

            // The additional volume difference (should be ~20ml, actual - 50ml base)
            float actual_additional = actual_total - 50.0;
            float expected_additional = manager_->cal_ctx.offset_target_ml * manager_->cal_ctx.offset_repetitions; // 20ml

            // Difference per small dose
            float difference_per_dose = (expected_additional - actual_additional) / manager_->cal_ctx.offset_repetitions;

            // Convert to time correction (333ms per ml at 3ml/s)
            int16_t correction_ms = (int16_t)(difference_per_dose * 333);

            // Get current offset and ADD correction to it
            uint16_t current_offset = calibration.getOffsetCalibration(manager_->cal_ctx.pump_index);
            int32_t new_offset = (int32_t)current_offset + correction_ms;

            // Clamp to reasonable range
            if (new_offset > 2000)
                new_offset = 2000;
            if (new_offset < 0)
                new_offset = 0;

            // Store for feedback display
            manager_->cal_ctx.old_offset_ms = current_offset;
            manager_->cal_ctx.correction_ms = correction_ms;
            manager_->cal_ctx.new_offset_ms = (uint16_t)new_offset;

            calibration.saveOffsetCalibration(manager_->cal_ctx.pump_index, (uint16_t)new_offset);
            manager_->switchTo(ScreenType::COMPLETE);
        }
        else if (event == InputEvent::BUTTON_HOLD)
        {
            // Skip offset calibration
            manager_->switchTo(ScreenType::COMPLETE);
        }
    }
    else if (event == InputEvent::BUTTON_HOLD)
    {
        // Emergency stop during pumping sequence
        if (is_pumping_)
        {
            pumpDriver.stopAllPumps();
        }
        next_auto_start_ = 0; // Cancel any pending auto-start
        manager_->returnToMain();
    }
}

void CalibrationOffsetScreen::refreshDisplay()
{
    display.clear();

    if (manager_->cal_ctx.current_repetition == 0 && next_auto_start_ > 0)
    {
        // Countdown screen
        uint32_t remaining = (next_auto_start_ > millis()) ? (next_auto_start_ - millis()) / 1000 + 1 : 0;
        display.printCenter(2, "Offset Calibration");
        display.printCenter(15, "Adding 10x 2ml");
        display.printCenter(25, "automatically...");
        if (remaining > 0)
        {
            display.printCenter(40, "Start in " + String(remaining) + "s");
        }
        else
        {
            display.printCenter(40, "Starting...");
        }
        display.printCenter(55, "Click=Now Hold=Skip");
    }
    else if (manager_->cal_ctx.current_repetition >= manager_->cal_ctx.offset_repetitions)
    {
        // Total volume input
        display.printCenter(2, "Total Volume?");
        display.printCenter(12, "Expected: ~70ml");
        display.printLine(3, "Actual:");
        display.printCenter(30, String(manager_->cal_ctx.offset_actual_total, 1) + "ml");
        display.printCenter(50, "Click=Save Hold=Skip");
    }
    else if (manager_->cal_ctx.current_repetition > 0)
    {
        // Waiting for next dose
        display.printCenter(10, "Next dose...");
        display.printCenter(25, String(manager_->cal_ctx.current_repetition + 1) + " of 10");
        display.printCenter(40, "2ml target");
    }

    display.show();
}

void CalibrationOffsetScreen::startNextRepetition()
{
    repetition_start_time_ = millis();
    is_pumping_ = true;
    pumpDriver.startPump(manager_->cal_ctx.pump_index);
}