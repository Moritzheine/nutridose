#pragma once
#include <Arduino.h>
#include <RotaryEncoder.h>
#include <OneButton.h>
#include "config.h"

enum class InputEvent
{
    NONE,
    ENCODER_UP,
    ENCODER_DOWN,
    BUTTON_CLICK,
    BUTTON_HOLD
};

class Input
{
public:
    void begin();
    void update();
    InputEvent getEvent();

private:
    RotaryEncoder *encoder_;
    OneButton *button_;
    InputEvent last_event_;
    long last_encoder_pos_;

    static void onButtonClick();
    static void onButtonHold();
    static Input *instance_;
};

// Global input object
extern Input input;