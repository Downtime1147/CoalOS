#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <string>
#include <vector>
#include "core/GameState.h"

class FileSystem;

class SaveManager {
public:
    SaveManager();
    ~SaveManager();

    // Check if save file exists
    bool SaveExists(const std::string& filename = "save.json");
    
    // Save current game state
    bool SaveGame(const std::string& filename, 
                  const std::vector<std::string>& inventory,
                  const std::vector<GameState::NetworkDevice>& devices);
    
    // Load game state
    bool LoadGame(const std::string& filename,
                  std::vector<std::string>& outInventory,
                  std::vector<GameState::NetworkDevice>& outDevices);
    
    // Generate random network data
    static std::vector<GameState::NetworkDevice> GenerateRandomNetworks(int count = 20);

private:
    std::string m_SaveDirectory;
    
    // Helper functions for random generation
    static std::string GenerateRandomIP();
    static std::string GenerateRandomESSID();
    static std::string GenerateRandomPassword();
    static std::string GenerateRandomOS();
    static std::string GenerateRandomMAC();
};

#endif // SAVEMANAGER_H