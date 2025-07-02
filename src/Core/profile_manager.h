#pragma once
#include <Arduino.h>
#include "config.h"

struct Profile
{
    char name;       // 'A'-'D'
    float ratios[4]; // NPK, Micro, pH, Bloom (in ml) - 0.1ml precision
    bool active;     // Slot used?

    // Helper to get total ml
    float getTotalMl() const
    {
        return ratios[0] + ratios[1] + ratios[2] + ratios[3];
    }
    String getName() const
    {
        return String(name);
    }
    String stringify() const
    {
        String result = getName() + ": ";
        for (uint8_t i = 0; i < 4; i++)
        {
            if (i > 0)
                result += ":";
            result += String(ratios[i], 1); // 1 decimal place
        }
        return result;
    }
};

class ProfileManager
{
public:
    static const uint8_t MAX_PROFILES = 4;

    void begin();

    // Profile operations
    bool saveProfile(uint8_t index, const float ratios[4]);
    bool deleteProfile(uint8_t index);
    const Profile &getProfile(uint8_t index) const;

    // Get formatted ratio string (e.g. "10:5:2:8")
    String getRatioString(uint8_t index) const;

    // Check if profile slot is used
    bool isProfileActive(uint8_t index) const;

    // Get profile by name ('A'-'D')
    int8_t getProfileIndex(char name) const;

private:
    Profile profiles_[MAX_PROFILES];

    void loadFromStorage();
    void saveToStorage();
    void initDefaults();
};

extern ProfileManager profileManager;