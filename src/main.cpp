#include <Arduino.h>
#include "config.h"
#include "hardware/pump_driver.h"
#include "core/calibration.h"
#include "ui/ui_controller.h"
#include "utils/logger.h"

PumpDriver pumpDriver;

void setup()
{
  Logger::begin();
  Logger::println("\n🌿 NutriDose Calibrated v" NUTRIDOSE_VERSION);

  // Initialize hardware & calibration
  pumpDriver.begin();
  calibration.begin();

  // Initialize UI system
  uiController.begin();

  Logger::println("System ready - UI & Calibration active");
}

void loop()
{
  // Update pump safety checks
  pumpDriver.update();

  // Update UI and handle input
  uiController.update();

  // Small delay for stability (non-blocking)
  delay(10);
}