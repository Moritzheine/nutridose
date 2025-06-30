#include "input.h"

// Global input instance
Input input;

// Static instance pointer for callbacks
Input *Input::instance_ = nullptr;

void Input::begin()
{
    instance_ = this;

    // Initialize encoder
    encoder_ = new RotaryEncoder(ENCODER_CLK_PIN, ENCODER_DT_PIN, RotaryEncoder::LatchMode::TWO03);
    last_encoder_pos_ = encoder_->getPosition();

    // Initialize button
    button_ = new OneButton(ENCODER_SW_PIN, true);
    button_->attachClick(onButtonClick);
    button_->attachLongPressStart(onButtonHold);

    last_event_ = InputEvent::NONE;

    Serial.println("Input initialized");
}

void Input::update()
{
    // Update encoder
    encoder_->tick();

    // Update button
    button_->tick();

    // Check encoder rotation
    long newPos = encoder_->getPosition();
    if (newPos != last_encoder_pos_)
    {
        if (newPos < last_encoder_pos_)
        {
            last_event_ = InputEvent::ENCODER_UP;
        }
        else
        {
            last_event_ = InputEvent::ENCODER_DOWN;
        }
        last_encoder_pos_ = newPos;
    }
}

InputEvent Input::getEvent()
{
    InputEvent event = last_event_;
    last_event_ = InputEvent::NONE;
    return event;
}

// Static callback functions
void Input::onButtonClick()
{
    if (instance_)
    {
        instance_->last_event_ = InputEvent::BUTTON_CLICK;
    }
}

void Input::onButtonHold()
{
    if (instance_)
    {
        instance_->last_event_ = InputEvent::BUTTON_HOLD;
    }
}