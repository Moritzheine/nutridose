#include <Arduino.h>
#include "config.h"
#include "hardware/pump_driver.h"
#include "core/calibration.h"
#include "core/profile_manager.h"
#include "ui/screen_manager.h"
#include "utils/logger.h"

PumpDriver pumpDriver;

void setup()
{
  Logger::begin();
  delay(1000); // Allow time for serial monitor to connect
  Logger::println("\n🌿 NutriDose Calibrated v" NUTRIDOSE_VERSION);

  // Initialize hardware & calibration
  pumpDriver.begin();
  calibration.begin();
  profileManager.begin();

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