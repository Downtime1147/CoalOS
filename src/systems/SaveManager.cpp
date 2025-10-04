#include "systems/SaveManager.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <iomanip>
#include <ctime>

using json = nlohmann::json;

SaveManager::SaveManager() : m_SaveDirectory("saves/") {
}

SaveManager::~SaveManager() {
}

bool SaveManager::SaveExists(const std::string& filename) {
    std::ifstream file(m_SaveDirectory + filename);
    return file.good();
}

bool SaveManager::SaveGame(const std::string& filename, 
                           const std::vector<std::string>& inventory,
                           const std::vector<GameState::NetworkDevice>& devices) {
    try {
        json saveData;
        
        // Save timestamp
        std::time_t now = std::time(nullptr);
        saveData["timestamp"] = now;
        saveData["version"] = "1.0";
        
        // Save inventory
        saveData["inventory"] = inventory;
        
        // Save network devices
        json devicesJson = json::array();
        for (const auto& device : devices) {
            json deviceJson;
            deviceJson["ip"] = device.ip;
            deviceJson["essid"] = device.essid;
            deviceJson["password"] = device.password;
            deviceJson["os"] = device.os;
            devicesJson.push_back(deviceJson);
        }
        saveData["devices"] = devicesJson;
        
        // Create save directory if it doesn't exist
        std::system(("mkdir -p " + m_SaveDirectory).c_str());
        
        // Write to file
        std::ofstream file(m_SaveDirectory + filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open save file for writing: " << filename << std::endl;
            return false;
        }
        
        file << std::setw(4) << saveData << std::endl;
        file.close();
        
        std::cout << "Game saved successfully to " << m_SaveDirectory + filename << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error saving game: " << e.what() << std::endl;
        return false;
    }
}

bool SaveManager::LoadGame(const std::string& filename,
                           std::vector<std::string>& outInventory,
                           std::vector<GameState::NetworkDevice>& outDevices) {
    try {
        std::ifstream file(m_SaveDirectory + filename);
        if (!file.is_open()) {
            std::cerr << "Save file not found: " << filename << std::endl;
            return false;
        }
        
        json saveData;
        file >> saveData;
        file.close();
        
        // Load inventory
        if (saveData.contains("inventory")) {
            outInventory = saveData["inventory"].get<std::vector<std::string>>();
        }
        
        // Load devices
        if (saveData.contains("devices")) {
            outDevices.clear();
            for (const auto& deviceJson : saveData["devices"]) {
                GameState::NetworkDevice device;
                device.ip = deviceJson["ip"].get<std::string>();
                device.essid = deviceJson["essid"].get<std::string>();
                device.password = deviceJson["password"].get<std::string>();
                device.os = deviceJson["os"].get<std::string>();
                outDevices.push_back(device);
            }
        }
        
        std::cout << "Game loaded successfully from " << m_SaveDirectory + filename << std::endl;
        std::cout << "Loaded " << outInventory.size() << " files and " << outDevices.size() << " network devices" << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error loading game: " << e.what() << std::endl;
        return false;
    }
}

std::vector<GameState::NetworkDevice> SaveManager::GenerateRandomNetworks(int count) {
    std::vector<GameState::NetworkDevice> devices;
    devices.reserve(count);
    
    for (int i = 0; i < count; ++i) {
        GameState::NetworkDevice device;
        device.ip = GenerateRandomIP();
        device.essid = GenerateRandomESSID();
        device.password = GenerateRandomPassword();
        device.os = GenerateRandomOS();
        devices.push_back(device);
    }
    
    std::cout << "Generated " << count << " random network devices" << std::endl;
    return devices;
}

std::string SaveManager::GenerateRandomIP() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> octet(1, 255);
    std::uniform_int_distribution<> segments(3, 5); // 3-5 segments for variety
    
    std::ostringstream ip;
    int numSegments = segments(gen);
    
    for (int i = 0; i < numSegments; ++i) {
        if (i > 0) ip << ".";
        ip << octet(gen);
    }
    
    return ip.str();
}

std::string SaveManager::GenerateRandomESSID() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    
    // Variety of ESSID patterns
    static const std::vector<std::string> prefixes = {
        "NETGEAR", "LINKSYS", "TP-LINK", "ASUS", "DLINK",
        "HOME", "OFFICE", "WIFI", "NET", "ROUTER",
        "CORP", "GUEST", "SECURE", "PUBLIC", "PRIVATE"
    };
    
    static const std::vector<std::string> suffixes = {
        "5G", "2.4G", "FAST", "PRO", "PLUS",
        "MAX", "ULTRA", "SECURE", "GUEST", "ADMIN"
    };
    
    std::uniform_int_distribution<> prefixDist(0, prefixes.size() - 1);
    std::uniform_int_distribution<> suffixDist(0, suffixes.size() - 1);
    std::uniform_int_distribution<> numDist(0, 9999);
    std::uniform_int_distribution<> typeDist(0, 3);
    
    std::ostringstream essid;
    
    switch (typeDist(gen)) {
        case 0: // Prefix + Number
            essid << prefixes[prefixDist(gen)] << "_" << numDist(gen);
            break;
        case 1: // Prefix + Suffix
            essid << prefixes[prefixDist(gen)] << "-" << suffixes[suffixDist(gen)];
            break;
        case 2: // Prefix + Number + Suffix
            essid << prefixes[prefixDist(gen)] << numDist(gen) << "_" << suffixes[suffixDist(gen)];
            break;
        case 3: // Just Prefix + Short Number
            essid << prefixes[prefixDist(gen)] << (numDist(gen) % 100);
            break;
    }
    
    return essid.str();
}

std::string SaveManager::GenerateRandomPassword() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    
    static const std::string alphaLower = "abcdefghijklmnopqrstuvwxyz";
    static const std::string alphaUpper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static const std::string digits = "0123456789";
    static const std::string special = "!@#$%^&*()_+-=[]{}|;:,.<>?";
    
    std::uniform_int_distribution<> lengthDist(8, 20);
    std::uniform_int_distribution<> typeDist(0, 3);
    
    int length = lengthDist(gen);
    std::ostringstream password;
    
    // Ensure at least one of each type
    password << alphaUpper[gen() % alphaUpper.length()];
    password << alphaLower[gen() % alphaLower.length()];
    password << digits[gen() % digits.length()];
    password << special[gen() % special.length()];
    
    // Fill the rest randomly
    std::string allChars = alphaLower + alphaUpper + digits + special;
    std::uniform_int_distribution<> charDist(0, allChars.length() - 1);
    
    for (int i = 4; i < length; ++i) {
        password << allChars[charDist(gen)];
    }
    
    return password.str();
}

std::string SaveManager::GenerateRandomOS() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    
    static const std::vector<std::string> operatingSystems = {
        "Windows 7",
        "Windows 10",
        "Windows 11",
        "Windows XP",
        "macOS Monterey",
        "macOS Ventura",
        "macOS Catalina",
        "Ubuntu 20.04",
        "Ubuntu 22.04",
        "Debian 11",
        "CentOS 7",
        "Red Hat Enterprise Linux 8",
        "FreeBSD 13",
        "Android 12",
        "iOS 16"
    };
    
    std::uniform_int_distribution<> dist(0, operatingSystems.size() - 1);
    return operatingSystems[dist(gen)];
}

std::string SaveManager::GenerateRandomMAC() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> hexDist(0, 15);
    
    std::ostringstream mac;
    mac << std::hex << std::setfill('0');
    
    for (int i = 0; i < 6; ++i) {
        if (i > 0) mac << ":";
        mac << std::setw(2) << hexDist(gen) << std::setw(2) << hexDist(gen);
    }
    
    return mac.str();
}
