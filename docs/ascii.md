# ASCII Art Utility - Usage Guide

## File Structure

Create your ASCII art text files in the assets folder:

```
assets/
├── fonts/
│   └── monospace.ttf
└── art/
    ├── boot.txt
    ├── ftpea.txt
    ├── sscrack.txt
    ├── metalib.txt
    └── nmap.txt
```

## Example ASCII Art Files

### assets/art/boot.txt
```
  _____ ___    _    _       ___  ____  
 / ____/ _ \  / \  | |     / _ \/ ___| 
| |   | | | |/ _ \ | |    | | | \___ \ 
| |___| |_| / ___ \| |___ | |_| |___) |
 \____|\___/_/   \_\_____| \___/|____/ 
                                        
        Coal OS Boot Loader v1.0
```

### assets/art/ftpea.txt
```
 _____ _____ ____            
|  ___|_   _|  _ \ ___  __ _ 
| |_    | | | |_) / _ \/ _` |
|  _|   | | |  __/  __/ (_| |
|_|     |_| |_|   \___|\__,_|
                              
  FTP Exploitation Assistant
```

### assets/art/sscrack.txt
```
 ____ ____                     _    
/ ___/ ___|  ___ _ __ __ _  __| | __
\___ \___ \ / __| '__/ _` |/ _` |/ /
 ___) |__) | (__| | | (_| | (_| |   
|____/____/ \___|_|  \__,_|\__,_|\_\
                                     
   SSH Port Cracking Tool v2.1.3
```

## Usage in Code

### Method 1: Load and Display to Terminal (Simple)

```cpp
// In your tool initialization or command handler
AsciiArt art;
art.LoadFromFile("assets/art/ftpea.txt");
art.DisplayToTerminal(m_Terminal);
```

### Method 2: Pre-load and Cache (Efficient)

```cpp
// In Engine::Initialize() - load all art once
m_AsciiArt = std::make_unique<AsciiArt>();
m_AsciiArt->LoadAndCache("boot", "assets/art/boot.txt");
m_AsciiArt->LoadAndCache("ftpea", "assets/art/ftpea.txt");
m_AsciiArt->LoadAndCache("sscrack", "assets/art/sscrack.txt");
m_AsciiArt->LoadAndCache("metalib", "assets/art/metalib.txt");
m_AsciiArt->LoadAndCache("nmap", "assets/art/nmap.txt");

// Later, display from cache
m_AsciiArt->DisplayCachedToTerminal("ftpea", m_Terminal);
```

### Method 3: Direct Rendering (Advanced)

```cpp
// Render ASCII art at specific coordinates without terminal
m_AsciiArt->RenderCachedDirect("boot", m_TextRenderer.get(), 100.0f, 500.0f, 1.0f);
```

## Integration Example: FTPea Tool

```cpp
void CommandParser::CmdFtpea(const std::vector<std::string>& args) {
    m_Terminal->AddLine("");
    
    // Display FTPea banner
    m_AsciiArt->DisplayCachedToTerminal("ftpea", m_Terminal);
    
    m_Terminal->AddLine("");
    m_Terminal->AddLine("Type 'help' for commands, 'exit' to quit");
    m_Terminal->AddLine("");
    
    // Change to FTPea input mode
    m_Terminal->SetPrompt("root ~ FTpea:~$ ");
}
```

## Creating ASCII Art

### Online Generators
- **Text to ASCII**: https://patorjk.com/software/taag/
- **ASCII Art Generator**: https://www.ascii-art-generator.org/
- **Image to ASCII**: https://www.asciiart.eu/image-to-ascii

### Recommended Fonts for Text-to-ASCII
- "Banner"
- "Big"
- "Standard"
- "Slant"
- "Small"

### Tips
1. Keep width under 80 characters for readability
2. Test your ASCII art in a monospace font
3. Avoid overly complex art that might not scale well
4. Use simple characters for better compatibility
5. Remember: backslashes need escaping in some contexts

## Full Example: Adding to Boot Sequence

In `Engine.cpp`:

```cpp
void Engine::ProcessBootSequence(float deltaTime) {
    m_BootTimer += deltaTime;
    
    static int bootStage = 0;
    
    if (bootStage == 0 && m_BootTimer > 0.5f) {
        m_AsciiArt->DisplayCachedToTerminal("boot", m_Terminal);
        m_Terminal->AddLine("");
        bootStage++;
    }
    else if (bootStage == 1 && m_BootTimer > 2.0f) {
        m_Terminal->AddLine("Calculated RAMsize: 66816233991524 Mb");
        m_Terminal->AddLine("Initiating Setup...");
        bootStage++;
    }
    // ... rest of boot sequence
}
```

## Performance Notes

- **Caching is recommended** for frequently used ASCII art
- Loading from file each time is fine for one-time displays
- Direct rendering bypasses terminal history (useful for splash screens)
- Cache all art during initialization for best performance