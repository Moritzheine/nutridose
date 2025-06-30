#include "menu_system.h"
#include "hardware/display.h"

// Static menu arrays
const String Menu::main_menu_[4] = {
    "Manual Dose",
    "Profiles",
    "Settings",
    "About"};

const String Menu::fertilizers_[4] = {
    "NPK Base",
    "Micro Nutrients",
    "pH Down",
    "Bloom Booster"};

uint8_t Menu::navigate(uint8_t current, uint8_t max_items, bool up)
{
    if (up)
    {
        return (current + 1) % max_items;
    }
    else
    {
        return (current == 0) ? max_items - 1 : current - 1;
    }
}

uint8_t Menu::adjustValue(uint8_t current, uint8_t min_val, uint8_t max_val, bool up)
{
    if (up)
    {
        return (current >= max_val) ? max_val : current + 1;
    }
    else
    {
        return (current <= min_val) ? min_val : current - 1;
    }
}

void Menu::showMainMenu(uint8_t selected)
{
    display.showMenu("NutriDose v1.0", main_menu_, 4, selected);
}

void Menu::showFertilizerSelect(uint8_t selected)
{
    display.showMenu("Select Fertilizer", fertilizers_, 4, selected);
}

void Menu::showAmountSet(uint8_t fertilizer, uint8_t amount)
{
    String label = fertilizers_[fertilizer];
    display.showValue(label, amount, "ml");
}

void Menu::showConfirm(uint8_t fertilizer, uint8_t amount)
{
    display.clear();

    // Title
    display.printCenter(5, "Confirm Dose");

    // Details
    display.printLine(2, "Fertilizer:");
    display.printLine(3, "  " + fertilizers_[fertilizer]);
    display.printLine(4, "Amount:");
    display.printLine(5, "  " + String(amount) + "ml");

    // Instructions
    display.printCenter(55, "Click=Start Hold=Back");

    display.show();
}

void Menu::showDosing(uint8_t fertilizer, uint8_t progress)
{
    String title = "Dosing " + fertilizers_[fertilizer];
    display.showProgress(title, progress);
}

void Menu::showComplete()
{
    display.clear();
    display.printCenter(20, "Dosing");
    display.printCenter(35, "Complete!");
    display.show();
}