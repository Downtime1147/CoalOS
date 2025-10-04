# CoalOS Save System Documentation

## Overview

The save system automatically generates random network data on first run and persists game state between sessions.

## Features

- **Auto-save on exit**: Game state saved when closing the application
- **Manual save**: Use `save` command at any time
- **Random network generation**: 20 unique networks created on first run
- **Persistent inventory**: File system saved and restored
- **JSON format**: Human-readable save files

## File Structure

Save files are stored in `saves/save.json`:

```json
{
  "timestamp": 1234567890,
  "version": "1.0",
  "inventory": [
    "rockyou.pwd",
    "captured_handshake.cap"
  ],
  "devices": [
    {
      "ip": "192.168.1.45",
      "essid": "NETGEAR_5742",
      "password": "Kx9@mPz4!wR",
      "os": "Windows 10"
    }
  ]
}
```

## Random Network Generation

Networks are generated with realistic-looking data:

### IP Addresses
- 3-5 segments (e.g., `192.168.1.45`, `10.0.45.123.67`)
- Each segment: 1-255
- Intentionally unrealistic for game immersion

### ESSID (Network Names)
Patterns include:
- `PREFIX_NUMBER` (e.g., `NETGEAR_5742`)
- `PREFIX-SUFFIX` (e.g., `LINKSYS-5G`)
- `PREFIX+NUMBER+SUFFIX` (e.g., `ASUS9942_ULTRA`)

Prefixes: NETGEAR, LINKSYS, TP-LINK, ASUS, DLINK, HOME, OFFICE, etc.
Suffixes: 5G, 2.4G, FAST, PRO, PLUS, MAX, ULTRA, SECURE, etc.

### Passwords
- 8-20 characters
- Mix of uppercase, lowercase, digits, and special characters
- Always contains at least one of each type
- Example: `Kx9@mPz4!wR`

### Operating Systems
Randomly selected from:
- Windows 7, 10, 11, XP
- macOS Monterey, Ventura, Catalina
- Ubuntu 20.04, 22.04
- Debian 11
- CentOS 7
- Red Hat Enterprise Linux 8
- FreeBSD 13
- Android 12
- iOS 16

## How It Works

### First Run
1. Engine checks for `saves/save.json`
2. If not found, generates 20 random networks
3. Saves initial state to `saves/save.json`
4. Player starts with `rockyou.pwd` in inventory

### Subsequent Runs
1. Engine loads `saves/save.json`
2. Restores inventory (all files player collected)
3. Restores all network devices with their credentials
4. Player continues where they left off

### Manual Save
```bash
save    # Saves current state immediately
```

### Auto-save
- Automatically saves when you close the game
- Happens in `Engine::Shutdown()`

## Implementation Details

### Files Added

**Headers:**
- `include/systems/SaveManager.h`

**Source:**
- `src/systems/SaveManager.cpp`

**Modified:**
- `include/core/GameState.h` - Added `GetAllDevices()` and `LoadDevices()`
- `src/core/GameState.cpp` - Implemented device management
- `include/core/Engine.h` - Added SaveManager member
- `src/core/Engine.cpp` - Integrated save/load logic
- `include/systems/CommandParser.h` - Added save command
- `src/systems/CommandParser.cpp` - Implemented save command

### Code Examples

#### Manually Trigger Save
```cpp
// In any command or system
m_Engine->SaveGameData();
```

#### Generate New Networks
```cpp
// Generate 50 networks instead of 20
auto devices = SaveManager::GenerateRandomNetworks(50);
m_GameState->LoadDevices(devices);
```

#### Check if Save Exists
```cpp
SaveManager saveManager;
if (saveManager.SaveExists("save.json")) {
    // Load existing save
} else {
    // Generate new data
}
```

#### Custom Save Files
```cpp
// Save to different file
m_SaveManager->SaveGame("backup.json", inventory, devices);

// Load from different file
m_SaveManager->LoadGame("backup.json", inventory, devices);
```

## Accessing Network Data

### In Commands
```cpp
void CommandParser::CmdIwlist(const std::vector<std::string>& args) {
    // Get all devices through GameState
    auto allDevices = m_GameState->GetAllDevices();
    
    m_Terminal->AddLine("");
    m_Terminal->AddLine("Scanning for networks...");
    m_Terminal->AddLine("");
    
    int count = 1;
    for (const auto& device : allDevices) {
        m_Terminal->AddLine("Cell " + std::to_string(count++));
        m_Terminal->AddLine("  IP: " + device.ip);
        m_Terminal->AddLine("  ESSID: \"" + device.essid + "\"");
        m_Terminal->AddLine("  OS: " + device.os);
        m_Terminal->AddLine("");
    }
}
```

### Get Specific Device
```cpp
auto* device = m_GameState->GetDevice("192.168.1.45");
if (device) {
    std::cout << "Password: " << device->password << std::endl;
}
```

## Debugging Save System

### View Save File
```bash
cat saves/save.json
```

### Delete Save (Fresh Start)
```bash
rm saves/save.json
```

### Console Output
The system prints helpful messages:
```
Loading existing save...
Loaded 5 files and 20 network devices
Game saved successfully to saves/save.json
```

### Troubleshooting

**Save file not created:**
- Check if `saves/` directory exists (created automatically)
- Verify write permissions
- Look for error messages in console

**Data not persisting:**
- Ensure `SaveGameData()` is called on shutdown
- Check that JSON library is properly linked
- Verify save file is being written (check timestamp)

**Can't load save:**
- Check JSON format is valid
- Ensure version compatibility
- Look for parsing errors in console

## Future Enhancements

Ideas for expanding the save system:

1. **Multiple save slots**: `save.json`, `save2.json`, etc.
2. **Named saves**: Let players name their saves
3. **Auto-save intervals**: Save every 5 minutes
4. **Cloud sync**: Optional online backup
5. **Compression**: Compress large save files
6. **Encryption**: Encrypt save data (for immersion)
7. **Statistics tracking**: Time played, networks hacked, etc.
8. **Achievements**: Track player accomplishments
9. **Settings persistence**: Save CRT preferences, colors, etc.
10. **Backup system**: Keep last N save files

## Save File Location

- **Linux/Mac**: `./saves/save.json` (relative to executable)
- **Windows**: `.\saves\save.json` (relative to executable)
- **Development**: In your build directory

To change location, modify `m_SaveDirectory` in `SaveManager.h`:
```cpp
SaveManager::SaveManager() : m_SaveDirectory("saves/") {
    // Could use platform-specific locations:
    // Linux: ~/.local/share/coalos/
    // Windows: %APPDATA%/CoalOS/
    // Mac: ~/Library/Application Support/CoalOS/
}
```

## Important Notes

- Save file is human-readable JSON (easy to edit/debug)
- Each network is unique per save
- Deleting save.json creates fresh random networks
- The `save` command provides feedback via typewriter effect
- Auto-save prevents data loss on crashes (if properly closed)

## Testing the System

1. **First run**: Start game, check 20 networks generated
2. **Add files**: Use commands to add files to inventory
3. **Manual save**: Type `save` command
4. **Restart**: Close and reopen game
5. **Verify**: Check inventory and networks persist
6. **Delete save**: Remove `saves/save.json`
7. **Fresh start**: Verify new random networks generated