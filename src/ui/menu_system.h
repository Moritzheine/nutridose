#pragma once
#include <Arduino.h>
#include "config.h"

// Simple menu navigation
class Menu
{
public:
    static uint8_t navigate(uint8_t current, uint8_t max_items, bool up);
    static uint8_t adjustValue(uint8_t current, uint8_t min_val, uint8_t max_val, bool up);

    // Menu screens
    static void showMainMenu(uint8_t selected);
    static void showFertilizerSelect(uint8_t selected);
    static void showAmountSet(uint8_t fertilizer, uint8_t amount);
    static void showConfirm(uint8_t fertilizer, uint8_t amount);
    static void showDosing(uint8_t fertilizer, uint8_t progress);
    static void showComplete();

private:
    static const String main_menu_[5]; // Extended to 5 items
    static const String fertilizers_[4];
};