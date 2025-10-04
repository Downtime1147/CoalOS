#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <glm/glm.hpp>

struct Settings {
    // Display settings
    glm::vec3 textColor;
    float typewriterSpeed;
    
    // CRT settings
    bool crtEnabled;
    float scanlineIntensity;
    float curvature;
    float vignetteStrength;
    float chromaticAberration;
    float glowIntensity;
    float noiseAmount;
    
    // Default values
    static Settings GetDefaults();
    
    // Save/Load
    bool SaveToFile(const std::string& filename = "settings.json");
    bool LoadFromFile(const std::string& filename = "settings.json");
    static bool SettingsExist(const std::string& filename = "settings.json");
};

#endif // SETTINGS_H
