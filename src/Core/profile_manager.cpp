#include "profile_manager.h"
#include "storage/storage_manager.h"
#include "utils/logger.h"

ProfileManager profileManager;

void ProfileManager::begin()
{
    Logger::info("Profile Manager initialized");
}

bool ProfileManager::saveProfile(uint8_t index, const float ratios[4])
{
    if (index >= MAX_PROFILES)
        return false;

    storage.saveProfile(index, ratios);
    return true;
}

bool ProfileManager::deleteProfile(uint8_t index)
{
    if (index >= MAX_PROFILES)
        return false;

    storage.deleteProfile(index);
    return true;
}

Profile ProfileManager::getProfile(uint8_t index)
{
    Profile profile;

    if (index >= MAX_PROFILES)
    {
        // Return default
        profile.name = 'A';
        profile.active = false;
        for (uint8_t i = 0; i < 4; i++)
            profile.ratios[i] = 0;
        return profile;
    }

    profile.name = storage.getProfileName(index);
    profile.active = storage.isProfileActive(index);
    for (uint8_t i = 0; i < 4; i++)
    {
        profile.ratios[i] = storage.getProfileRatio(index, i);
    }

    return profile;
}

String ProfileManager::getRatioString(uint8_t index)
{
    return storage.getProfileRatioString(index);
}

bool ProfileManager::isProfileActive(uint8_t index)
{
    return storage.isProfileActive(index);
}

int8_t ProfileManager::getProfileIndex(char name)
{
    if (name < 'A' || name > 'D')
        return -1;
    return name - 'A';
}