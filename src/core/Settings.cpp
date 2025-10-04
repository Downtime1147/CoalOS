#include "core/Settings.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

Settings Settings::GetDefaults() {
    Settings defaults;
    
    // Display defaults
    defaults.textColor = glm::vec3(0.0f, 1.0f, 0.0f);  // Green
    defaults.typewriterSpeed = 50.0f;
    
    // CRT defaults (subtle settings)
    defaults.crtEnabled = true;
    defaults.scanlineIntensity = 0.03f;
    defaults.curvature = 0.05f;
    defaults.vignetteStrength = 0.15f;
    defaults.chromaticAberration = 0.3f;
    defaults.glowIntensity = 0.1f;
    defaults.noiseAmount = 0.02f;
    
    return defaults;
}

bool Settings::SaveToFile(const std::string& filename) {
    try {
        json settingsJson;
        
        // Save display settings
        settingsJson["textColor"] = {textColor.r, textColor.g, textColor.b};
        settingsJson["typewriterSpeed"] = typewriterSpeed;
        
        // Save CRT settings
        settingsJson["crtEnabled"] = crtEnabled;
        settingsJson["scanlineIntensity"] = scanlineIntensity;
        settingsJson["curvature"] = curvature;
        settingsJson["vignetteStrength"] = vignetteStrength;
        settingsJson["chromaticAberration"] = chromaticAberration;
        settingsJson["glowIntensity"] = glowIntensity;
        settingsJson["noiseAmount"] = noiseAmount;
        
        // Create saves directory if needed
        std::system("mkdir -p saves");
        
        // Write to file
        std::ofstream file("saves/" + filename);
        if (!file.is_open()) {
            std::cerr << "Failed to save settings" << std::endl;
            return false;
        }
        
        file << std::setw(4) << settingsJson << std::endl;
        file.close();
        
        std::cout << "Settings saved to saves/" << filename << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error saving settings: " << e.what() << std::endl;
        return false;
    }
}

bool Settings::LoadFromFile(const std::string& filename) {
    try {
        std::ifstream file("saves/" + filename);
        if (!file.is_open()) {
            std::cerr << "Settings file not found, using defaults" << std::endl;
            return false;
        }
        
        json settingsJson;
        file >> settingsJson;
        file.close();
        
        // Load display settings
        if (settingsJson.contains("textColor") && settingsJson["textColor"].is_array()) {
            auto colorArray = settingsJson["textColor"];
            textColor = glm::vec3(colorArray[0], colorArray[1], colorArray[2]);
        }
        if (settingsJson.contains("typewriterSpeed")) {
            typewriterSpeed = settingsJson["typewriterSpeed"];
        }
        
        // Load CRT settings
        if (settingsJson.contains("crtEnabled")) {
            crtEnabled = settingsJson["crtEnabled"];
        }
        if (settingsJson.contains("scanlineIntensity")) {
            scanlineIntensity = settingsJson["scanlineIntensity"];
        }
        if (settingsJson.contains("curvature")) {
            curvature = settingsJson["curvature"];
        }
        if (settingsJson.contains("vignetteStrength")) {
            vignetteStrength = settingsJson["vignetteStrength"];
        }
        if (settingsJson.contains("chromaticAberration")) {
            chromaticAberration = settingsJson["chromaticAberration"];
        }
        if (settingsJson.contains("glowIntensity")) {
            glowIntensity = settingsJson["glowIntensity"];
        }
        if (settingsJson.contains("noiseAmount")) {
            noiseAmount = settingsJson["noiseAmount"];
        }
        
        std::cout << "Settings loaded from saves/" << filename << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error loading settings: " << e.what() << std::endl;
        return false;
    }
}

bool Settings::SettingsExist(const std::string& filename) {
    std::ifstream file("saves/" + filename);
    return file.good();
}