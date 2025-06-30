#pragma once

#define NUTRIDOSE_VERSION "1.0.0"
#define NUM_TOTAL_PUMPS 5
#define MAX_PUMP_RUN_TIME_MS 30000

// ESP32-C3 Pin Definitions - Updated for actual hardware
// Input Pins (Rotary Encoder)
#define ENCODER_CLK_PIN 0 // CLK
#define ENCODER_DT_PIN 1  // DT
#define ENCODER_SW_PIN 2  // SW (Button)

// Display Pins (I2C OLED)
#define DISPLAY_SDA_PIN 3 // SDA
#define DISPLAY_SCL_PIN 4 // SCL

// Pump Control Pins (moved to avoid conflicts)
#define PUMP_1_NPK_BASE_PIN 5 // GPIO5  - NPK Base fertilizer
#define PUMP_2_MICRO_PIN 6    // GPIO6  - Micro nutrients
#define PUMP_3_PH_DOWN_PIN 7  // GPIO7  - pH Down
#define PUMP_4_BLOOM_PIN 8    // GPIO8  - Bloom booster
#define PUMP_5_WATER_PIN 9    // GPIO9  - Water/Flush

// UI Configuration
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define MENU_TIMEOUT_MS 30000

// External declarations (defined in main.cpp)
extern const uint8_t PUMP_PINS[5];
extern const char *PUMP_NAMES[5];