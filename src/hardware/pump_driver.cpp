#include "pump_driver.h"
#include "utils/logger.h"

// Static member definitions
const uint8_t PumpDriver::PUMP_PINS[5] = {
    PUMP_1_NPK_BASE_PIN,
    PUMP_2_MICRO_PIN,
    PUMP_3_PH_DOWN_PIN,
    PUMP_4_BLOOM_PIN,
    PUMP_5_WATER_PIN};

const char *PumpDriver::PUMP_NAMES[5] = {
    "NPK Base",
    "Micro",
    "pH Down",
    "Bloom",
    "Water"};

void PumpDriver::begin()
{
    Logger::info("Initializing pumps...");

    for (uint8_t i = 0; i < NUM_TOTAL_PUMPS; i++)
    {
        pinMode(PUMP_PINS[i], OUTPUT);
        digitalWrite(PUMP_PINS[i], LOW);
        pump_states_[i] = false;
        pump_start_times_[i] = 0;
    }

    Logger::info("Pumps initialized");
}

bool PumpDriver::startPump(uint8_t pump_id)
{
    if (pump_id >= NUM_TOTAL_PUMPS)
        return false;

    digitalWrite(PUMP_PINS[pump_id], HIGH);
    pump_states_[pump_id] = true;
    pump_start_times_[pump_id] = millis();

    Logger::info("Started pump " + String(pump_id) + " (" + PUMP_NAMES[pump_id] + ")");
    return true;
}

bool PumpDriver::stopPump(uint8_t pump_id)
{
    if (pump_id >= NUM_TOTAL_PUMPS)
        return false;

    digitalWrite(PUMP_PINS[pump_id], LOW);
    pump_states_[pump_id] = false;
    pump_start_times_[pump_id] = 0;

    Logger::info("Stopped pump " + String(pump_id) + " (" + PUMP_NAMES[pump_id] + ")");
    return true;
}

void PumpDriver::stopAllPumps()
{
    Logger::info("Stopping all pumps");
    for (uint8_t i = 0; i < NUM_TOTAL_PUMPS; i++)
    {
        stopPump(i);
    }
}

bool PumpDriver::isPumpRunning(uint8_t pump_id)
{
    if (pump_id >= NUM_TOTAL_PUMPS)
        return false;
    return pump_states_[pump_id];
}

void PumpDriver::update()
{
    // Safety timeout check
    uint32_t now = millis();
    for (uint8_t i = 0; i < NUM_TOTAL_PUMPS; i++)
    {
        if (pump_states_[i])
        {
            if (now - pump_start_times_[i] > MAX_PUMP_RUN_TIME_MS)
            {
                Logger::error("Pump " + String(i) + " timeout!");
                stopPump(i);
            }
        }
    }
}

void PumpDriver::printStatus()
{
    Logger::println("\nPump Status:");
    for (uint8_t i = 0; i < NUM_TOTAL_PUMPS; i++)
    {
        String status = String(i) + " " + PUMP_NAMES[i] + ": " +
                        (pump_states_[i] ? "ON" : "OFF");
        Logger::println(status);
    }
}