#include <Arduino.h>
#include "config.h"
#include "hardware/pump_driver.h"
#include "ui/ui_controller.h"
#include "utils/logger.h"

PumpDriver pumpDriver;

void setup()
{
  Logger::begin();
  Logger::println("\n🌿 NutriDose UI Test v" NUTRIDOSE_VERSION);

  // Initialize hardware
  pumpDriver.begin();

  // Initialize UI system
  uiController.begin();

  Logger::println("System ready - UI active");
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