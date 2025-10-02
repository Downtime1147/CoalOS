#include "core/GameState.h"

GameState::GameState() : m_CurrentMode(GameMode::BOOTING) {
    InitializeNetworkDevices();
}

GameState::~GameState() {
}

void GameState::AddDevice(const std::string& ip, const NetworkDevice& device) {
    m_NetworkDevices[ip] = device;
}

GameState::NetworkDevice* GameState::GetDevice(const std::string& ip) {
    auto it = m_NetworkDevices.find(ip);
    if (it != m_NetworkDevices.end()) {
        return &it->second;
    }
    return nullptr;
}

bool GameState::DeviceExists(const std::string& ip) const {
    return m_NetworkDevices.find(ip) != m_NetworkDevices.end();
}

void GameState::InitializeNetworkDevices() {
    // Initialize all devices from original Python version
    AddDevice("735.92.44.64.846", {"735.92.44.64.846", "Zx@P!o33Q7", "mnQwertrewQnm123", "Windows 7"});
    AddDevice("653.67.49.847.46", {"653.67.49.847.46", "Hal's_Comp", "MmkwgUltbara98", "Macintosh"});
    AddDevice("273.84.845.53.32", {"273.84.845.53.32", "NVB_271324", "ddid.No8817994", "Windows 10"});
    AddDevice("177.93.84.162.14", {"177.93.84.162.14", "VB0SE_CORP", "Discrom77681E4QR!", "Windows 10"});
    AddDevice("31.638.48.35.744", {"31.638.48.35.744", "DESKNo.747", "tunnelTrain2pltfrm6?!", "Windows 7"});
    AddDevice("98.712.345.63.28", {"98.712.345.63.28", "RF8_11G4M3", "AllTheGames911711", "Macintosh"});
    AddDevice("987.612.34.57.08", {"987.612.34.57.08", "PLA7746_CL", "AviationIsTheKey7273", "Windows 10"});
    AddDevice("45.64.565.434.56", {"45.64.565.434.56", "AAF7_B1GU4", "AllDinosAreInteresting5515", "Windows 7"});
}
