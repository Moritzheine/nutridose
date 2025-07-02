#pragma once
#include <Arduino.h>
#include "config.h"

struct Profile
{
    char name;       // 'A'-'D'
    float ratios[4]; // NPK, Micro, pH, Bloom (in ml/L)
    bool active;     // Slot used?

    float getTotalMl() const
    {
        return ratios[0] + ratios[1] + ratios[2] + ratios[3];
    }
};

class ProfileManager
{
public:
    static const uint8_t MAX_PROFILES = 4;

    void begin();

    // Profile operations - delegated to storage
    bool saveProfile(uint8_t index, const float ratios[4]);
    bool deleteProfile(uint8_t index);
    Profile getProfile(uint8_t index);

    // Convenience methods
    String getRatioString(uint8_t index);
    bool isProfileActive(uint8_t index);
    int8_t getProfileIndex(char name);
};

extern ProfileManager profileManager;