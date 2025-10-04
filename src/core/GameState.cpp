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

std::vector<GameState::NetworkDevice> GameState::GetAllDevices() const {
    std::vector<NetworkDevice> devices;
    for (const auto& pair : m_NetworkDevices) {
        devices.push_back(pair.second);
    }
    return devices;
}

void GameState::LoadDevices(const std::vector<NetworkDevice>& devices) {
    m_NetworkDevices.clear();
    for (const auto& device : devices) {
        m_NetworkDevices[device.ip] = device;
    }
}

void GameState::InitializeNetworkDevices() {
    // This will be called if no save exists
    // You can add some default devices here or leave empty
    // The SaveManager will generate random ones
}
