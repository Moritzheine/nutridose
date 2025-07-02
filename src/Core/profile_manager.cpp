#include "profile_manager.h"
#include "utils/logger.h"
#include <LittleFS.h>

ProfileManager profileManager;

void ProfileManager::begin()
{
    initDefaults();
    loadFromStorage();
    Logger::info("Profile Manager initialized");
}

void ProfileManager::initDefaults()
{
    for (uint8_t i = 0; i < MAX_PROFILES; i++)
    {
        profiles_[i].name = 'A' + i;
        profiles_[i].active = false;
        for (uint8_t j = 0; j < 4; j++)
        {
            profiles_[i].ratios[j] = 0;
        }
    }
}

bool ProfileManager::saveProfile(uint8_t index, const float ratios[4])
{
    if (index >= MAX_PROFILES)
        return false;

    profiles_[index].active = true;
    for (uint8_t i = 0; i < 4; i++)
    {
        profiles_[index].ratios[i] = ratios[i];
    }

    saveToStorage();
    Logger::info("Profile " + String(profiles_[index].name) + " saved");
    return true;
}

bool ProfileManager::deleteProfile(uint8_t index)
{
    if (index >= MAX_PROFILES)
        return false;

    profiles_[index].active = false;
    for (uint8_t i = 0; i < 4; i++)
    {
        profiles_[index].ratios[i] = 0;
    }

    saveToStorage();
    Logger::info("Profile " + String(profiles_[index].name) + " deleted");
    return true;
}

const Profile &ProfileManager::getProfile(uint8_t index) const
{
    if (index >= MAX_PROFILES)
        index = 0;
    return profiles_[index];
}

String ProfileManager::getRatioString(uint8_t index) const
{
    if (index >= MAX_PROFILES || !profiles_[index].active)
    {
        return "Empty";
    }

    const Profile &p = profiles_[index];
    String result = "";

    for (uint8_t i = 0; i < 4; i++)
    {
        if (i > 0)
            result += ":";

        // Format with 1 decimal place, remove trailing zero
        if (p.ratios[i] == (int)p.ratios[i])
        {
            result += String((int)p.ratios[i]);
        }
        else
        {
            result += String(p.ratios[i], 1);
        }
    }

    return result;
}

bool ProfileManager::isProfileActive(uint8_t index) const
{
    if (index >= MAX_PROFILES)
        return false;
    return profiles_[index].active;
}

int8_t ProfileManager::getProfileIndex(char name) const
{
    if (name < 'A' || name > 'D')
        return -1;
    return name - 'A';
}

void ProfileManager::loadFromStorage()
{
    File file = LittleFS.open("/profiles.dat", "r");
    if (!file)
    {
        Logger::info("No profile file, using defaults");
        return;
    }

    // Skip header
    file.seek(4);

    for (uint8_t i = 0; i < MAX_PROFILES; i++)
    {
        if (file.available() >= 17)
        { // 1 byte active + 4*4 bytes float
            file.read((uint8_t *)&profiles_[i].active, 1);
            file.read((uint8_t *)&profiles_[i].ratios, 16); // 4 floats
        }
    }

    file.close();
    Logger::info("Profiles loaded from storage");
}

void ProfileManager::saveToStorage()
{
    File file = LittleFS.open("/profiles.dat", "w");
    if (!file)
    {
        Logger::error("Failed to save profiles");
        return;
    }

    // Write header
    const char header[4] = {'P', 'R', 'F', '2'}; // Version 2 for float
    file.write((uint8_t *)header, 4);

    // Write profiles
    for (uint8_t i = 0; i < MAX_PROFILES; i++)
    {
        file.write((uint8_t *)&profiles_[i].active, 1);
        file.write((uint8_t *)&profiles_[i].ratios, 16); // 4 floats
    }

    file.close();
}