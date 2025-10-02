#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>
#include <map>

enum class GameMode {
    BOOTING,
    MAIN_TERMINAL,
    TOOL_FTPEA,
    TOOL_SSCRACK,
    TOOL_METALIB,
    REMOTE_CONNECTION
};

class GameState {
public:
    GameState();
    ~GameState();

    GameMode GetCurrentMode() const { return m_CurrentMode; }
    void SetMode(GameMode mode) { m_CurrentMode = mode; }

    // Network device data
    struct NetworkDevice {
        std::string ip;
        std::string essid;
        std::string password;
        std::string os;
    };

    void AddDevice(const std::string& ip, const NetworkDevice& device);
    NetworkDevice* GetDevice(const std::string& ip);
    bool DeviceExists(const std::string& ip) const;

    // Connection state
    void SetConnectedDevice(const std::string& ip) { m_ConnectedDevice = ip; }
    std::string GetConnectedDevice() const { return m_ConnectedDevice; }
    bool IsConnected() const { return !m_ConnectedDevice.empty(); }

private:
    GameMode m_CurrentMode;
    std::map<std::string, NetworkDevice> m_NetworkDevices;
    std::string m_ConnectedDevice;

    void InitializeNetworkDevices();
};

#endif // GAMESTATE_H
