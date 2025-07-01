#include "pump_driver.h"
#include "core/fertilizer_config.h"
#include "utils/logger.h"

void PumpDriver::begin()
{
    Logger::info("Initializing pumps...");

    for (uint8_t i = 0; i < NUM_TOTAL_PUMPS; i++)
    {
        const auto &fert = fertConfig.get(i);
        pinMode(fert.pin, OUTPUT);
        digitalWrite(fert.pin, LOW);
        pump_states_[i] = false;
        pump_start_times_[i] = 0;
    }

    Logger::info("Pumps initialized");
}

bool PumpDriver::startPump(uint8_t pump_id)
{
    if (pump_id >= NUM_TOTAL_PUMPS)
        return false;

    const auto &fert = fertConfig.get(pump_id);
    digitalWrite(fert.pin, HIGH);
    pump_states_[pump_id] = true;
    pump_start_times_[pump_id] = millis();

    Logger::info("Started pump " + String(pump_id) + " (" + fert.name + ") on pin " + String(fert.pin));
    return true;
}

bool PumpDriver::stopPump(uint8_t pump_id)
{
    if (pump_id >= NUM_TOTAL_PUMPS)
        return false;

    const auto &fert = fertConfig.get(pump_id);
    digitalWrite(fert.pin, LOW);
    pump_states_[pump_id] = false;
    pump_start_times_[pump_id] = 0;

    Logger::info("Stopped pump " + String(pump_id) + " (" + fert.name + ")");
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
        const auto &fert = fertConfig.get(i);
        String status = String(i) + " " + fert.name + ": " +
                        (pump_states_[i] ? "ON" : "OFF");
        Logger::println(status);
    }
}