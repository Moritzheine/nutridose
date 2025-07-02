#include <Arduino.h>
#include "config.h"
#include "hardware/pump_driver.h"
#include "storage/storage_manager.h"
#include "core/calibration.h"
#include "core/profile_manager.h"
#include "ui/screen_manager.h"
#include "utils/logger.h"

PumpDriver pumpDriver;

void setup()
{
  Logger::begin();
  delay(1000);
  Logger::println("\n🌿 NutriDose Unified v" NUTRIDOSE_VERSION);

  // Initialize central storage first
  storage.begin();

  // Initialize hardware & systems
  pumpDriver.begin();
  calibration.begin();
  profileManager.begin();
  screenManager.begin();

  Logger::println("System ready - Unified storage active");
}

void loop()
{
  pumpDriver.update();
  screenManager.update();
  delay(10);
}