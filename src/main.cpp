#include <Arduino.h>
#include "config.h"
#include "hardware/pump_driver.h"
#include "utils/logger.h"

PumpDriver pumpDriver;
void processCommand(String cmd);

void setup() {
    Logger::begin();
    Logger::println("\n🌿 NutriDose Pump Test v" NUTRIDOSE_VERSION);
    
    pumpDriver.begin();
    
    Logger::println("Commands: p0-p4 on/off, status, stop, help");
    Logger::print("> ");
}

void loop() {
    pumpDriver.update();
    
    if (Serial.available()) {
        String cmd = Serial.readString();
        cmd.trim();
        processCommand(cmd);
        Logger::print("> ");
    }
    
    delay(10);
}

void processCommand(String cmd) {
    cmd.toLowerCase();
    
    if (cmd == "help") {
        Logger::println("Commands:");
        Logger::println("  p<0-4> on   - Start pump");
        Logger::println("  p<0-4> off  - Stop pump");
        Logger::println("  status      - Show status");
        Logger::println("  stop        - Stop all");
    }
    else if (cmd == "status") {
        pumpDriver.printStatus();
    }
    else if (cmd == "stop") {
        pumpDriver.stopAllPumps();
        Logger::println("All pumps stopped");
    }
    else if (cmd.startsWith("p") && cmd.length() >= 4) {
        int pump = cmd.charAt(1) - '0';
        String action = cmd.substring(3);
        
        if (pump >= 0 && pump < NUM_TOTAL_PUMPS) {
            if (action == "on") {
                pumpDriver.startPump(pump);
            } else if (action == "off") {
                pumpDriver.stopPump(pump);
            }
        }
    }
    else {
        Logger::println("Unknown command. Type 'help'");
    }
}