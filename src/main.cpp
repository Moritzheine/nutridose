#include <Arduino.h>
#include "config.h"
#include "hardware/pump_driver.h"
#include "core/calibration.h"
#include "ui/screen_manager.h"
#include "utils/logger.h"

PumpDriver pumpDriver;

void setup()
{
  Logger::begin();
  Logger::println("\n🌿 NutriDose Calibrated v" NUTRIDOSE_VERSION);

  // Initialize hardware & calibration
  pumpDriver.begin();
  calibration.begin();

  // Initialize Screen Manager
  screenManager.begin();

  Logger::println("System ready - Modular UI active");
}

void loop()
{
  // Update pump safety checks
  pumpDriver.update();

  // Update Screen Manager
  screenManager.update();

  // Small delay for stability
  delay(10);
}