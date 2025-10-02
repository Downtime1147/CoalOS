# CoalOS C++ Development Guide

## Table of Contents
1. [Project Architecture](#project-architecture)
2. [Adding New Commands](#adding-new-commands)
3. [Creating New Tools](#creating-new-tools)
4. [Adding Network Devices](#adding-network-devices)
5. [Working with Audio](#working-with-audio)
6. [Visual Effects](#visual-effects)
7. [Learning Resources](#learning-resources)

---

## Project Architecture

### Core Components

**Engine** (`core/Engine.h/.cpp`)
- Main game loop coordinator
- Manages all subsystems
- Handles window events and input

**Terminal** (`ui/Terminal.h/.cpp`)
- Display output and scrolling
- Input buffer management
- Cursor rendering

**CommandParser** (`systems/CommandParser.h/.cpp`)
- Command registration system
- Input tokenization and routing
- Extensible via lambda functions or member functions

**GameState** (`core/GameState.h/.cpp`)
- Stores game mode (main terminal, tools, remote connection)
- Network device database
- Connection state tracking

**FileSystem** (`systems/FileSystem.h/.cpp`)
- Virtual file management (Inventory equivalent)
- File existence checking
- Simple add/remove operations

### Data Flow
```
User Input → GLFW → Engine → Terminal (display)
                  ↓
          CommandParser (parse)
                  ↓
          Execute Command → Modify GameState/FileSystem
                  ↓
          Terminal (output) → TextRenderer → Screen
```

---

## Adding New Commands

Commands are registered in `CommandParser::Initialize()` using a simple pattern.

### Step 1: Add Command Declaration (CommandParser.h)
```cpp
private:
    void CmdYourCommand(const std::vector<std::string>& args);
```

### Step 2: Register Command (CommandParser.cpp Initialize())
```cpp
RegisterCommand("yourcommand", 
    [this](const auto& args) { CmdYourCommand(args); }, 
    "description of your command");
```

### Step 3: Implement Command (CommandParser.cpp)
```cpp
void CommandParser::CmdYourCommand(const std::vector<std::string>& args) {
    m_Terminal->AddLine("");
    
    // Check arguments
    if (args.size() < 2) {
        m_Terminal->AddLine("Usage: yourcommand <parameter>");
        m_Terminal->AddLine("");
        return;
    }
    
    // Do something
    std::string param = args[1];
    m_Terminal->AddLine("You entered: " + param);
    
    // Access filesystem
    if (m_FileSystem->FileExists("somefile.txt")) {
        // Do something
    }
    
    m_Terminal->AddLine("");
}
```

### Example: Implementing `iwlist` (Network Scan)

```cpp
// In CommandParser::Initialize()
RegisterCommand("iwlist", [this](const auto& args) { CmdIwlist(args); }, 
    "scans for and displays nearby wireless internet connections");

// Implementation
void CommandParser::CmdIwlist(const std::vector<std::string>& args) {
    m_Terminal->AddLine("");
    m_Terminal->AddLine("Scanning...");
    
    // Could add a delay here or use a progress indicator
    
    m_Terminal->AddLine("9 wireless connections found...");
    m_Terminal->AddLine("");
    
    // Display network list (could come from GameState)
    m_Terminal->AddLine("Cell 01 - Address: 735.92.44.64.846");
    m_Terminal->AddLine("          ESSID: \"Zx@P!o33Q7\"");
    m_Terminal->AddLine("          Quality: 70/70  Signal level: -30 dBm");
    m_Terminal->AddLine("");
    
    m_Terminal->AddLine("Cell 02 - Address: 653.67.49.847.46");
    m_Terminal->AddLine("          ESSID: \"Hal's_Comp\"");
    m_Terminal->AddLine("          Quality: 65/70  Signal level: -35 dBm");
    m_Terminal->AddLine("");
    // ... etc
}
```

---

## Creating New Tools

Tools are complex commands with their own sub-menu systems (like FTPea, SScrack, MetaLib).

### Architecture Pattern

Each tool should be its own class inheriting from a base `Tool` class.

### Step 1: Create Base Tool Class

**include/tools/Tool.h**
```cpp
#ifndef TOOL_H
#define TOOL_H

#include <string>
#include <vector>

class Terminal;
class FileSystem;
class GameState;

class Tool {
public:
    Tool(Terminal* terminal, FileSystem* fs, GameState* state);
    virtual ~Tool();
    
    // Called when tool is entered
    virtual void Enter() = 0;
    
    // Called each frame while active
    virtual void Update(float deltaTime) = 0;
    
    // Process input for this tool
    virtual void ProcessInput(const std::string& input) = 0;
    
    // Called when exiting tool
    virtual void Exit() = 0;
    
    // Check if tool should return to main terminal
    bool ShouldExit() const { return m_ShouldExit; }

protected:
    Terminal* m_Terminal;
    FileSystem* m_FileSystem;
    GameState* m_GameState;
    bool m_ShouldExit;
    
    void DisplayBanner(const std::string& bannerPath);
};

#endif
```

### Step 2: Create Specific Tool (Example: SScrack)

**include/tools/SScrack.h**
```cpp
#ifndef SSCRACK_H
#define SSCRACK_H

#include "tools/Tool.h"
#include <string>

class SScrack : public Tool {
public:
    SScrack(Terminal* terminal, FileSystem* fs, GameState* state);
    ~SScrack();
    
    void Enter() override;
    void Update(float deltaTime) override;
    void ProcessInput(const std::string& input) override;
    void Exit() override;

private:
    enum class State {
        MENU,
        ATTACKING,
        COMPLETE
    };
    
    State m_State;
    float m_AttackTimer;
    std::string m_TargetIP;
    std::string m_TargetMAC;
    std::string m_TargetPort;
    
    void DisplayHelp();
    void StartPortHack();
    void ProcessAttack(float deltaTime);
};

#endif
```

**src/tools/SScrack.cpp**
```cpp
#include "tools/SScrack.h"
#include "ui/Terminal.h"
#include "core/GameState.h"

SScrack::SScrack(Terminal* terminal, FileSystem* fs, GameState* state)
    : Tool(terminal, fs, state), m_State(State::MENU), m_AttackTimer(0.0f) {
}

SScrack::~SScrack() {
}

void SScrack::Enter() {
    DisplayBanner("assets/art/sscrack.txt");
    m_Terminal->AddLine("");
    m_Terminal->SetPrompt("root ~ SScrack:~$ ");
}

void SScrack::Update(float deltaTime) {
    if (m_State == State::ATTACKING) {
        ProcessAttack(deltaTime);
    }
}

void SScrack::ProcessInput(const std::string& input) {
    if (input == "exit") {
        m_ShouldExit = true;
        return;
    }
    
    if (input == "help") {
        DisplayHelp();
        return;
    }
    
    if (input == "porthack") {
        StartPortHack();
        return;
    }
    
    m_Terminal->AddLine("Invalid command. Type 'help' for options.");
    m_Terminal->AddLine("");
}

void SScrack::Exit() {
    m_Terminal->SetPrompt("root:~$ ");
}

void SScrack::DisplayHelp() {
    m_Terminal->AddLine("");
    m_Terminal->AddLine("------------------------------");
    m_Terminal->AddLine("porthack - cracks SSH vulnerable ports");
    m_Terminal->AddLine("exit     - disconnect from SScrack");
    m_Terminal->AddLine("help     - displays this help");
    m_Terminal->AddLine("------------------------------");
    m_Terminal->AddLine("");
}

void SScrack::StartPortHack() {
    m_Terminal->AddLine("");
    m_Terminal->AddLine("Enter target IP: ");
    // Note: You'll need to implement a special input mode
    // or use a callback system for multi-stage input
}

void SScrack::ProcessAttack(float deltaTime) {
    m_AttackTimer += deltaTime;
    
    // Show progress messages at intervals
    // Eventually complete and show password
}
```

### Step 3: Integrate Tool into Engine

**Engine.h**
```cpp
#include "tools/SScrack.h"

class Engine {
private:
    std::unique_ptr<SScrack> m_SScrackTool;
    Tool* m_ActiveTool;
};
```

**Engine.cpp**
```cpp
bool Engine::Initialize() {
    // ... existing initialization
    
    m_SScrackTool = std::make_unique<SScrack>(
        m_Terminal.get(), 
        m_FileSystem.get(), 
        m_GameState.get()
    );
    m_ActiveTool = nullptr;
}

void Engine::Update(float deltaTime) {
    if (m_ActiveTool) {
        m_ActiveTool->Update(deltaTime);
        
        if (m_ActiveTool->ShouldExit()) {
            m_ActiveTool->Exit();
            m_ActiveTool = nullptr;
        }
    } else {
        // Normal terminal mode
        m_Terminal->Update(deltaTime);
    }
}

void Engine::OnKeyPress(int key, int scancode, int action, int mods) {
    // ... handle input
    
    if (key == GLFW_KEY_ENTER) {
        std::string input = m_Terminal->GetCurrentInput();
        m_Terminal->SubmitInput();
        
        if (m_ActiveTool) {
            m_ActiveTool->ProcessInput(input);
        } else {
            m_CommandParser->ParseAndExecute(input);
        }
    }
}
```

**CommandParser.cpp - Add command to launch tool**
```cpp
void CommandParser::CmdSScrack(const std::vector<std::string>& args) {
    // Signal to Engine to activate SScrack tool
    // You'll need a callback or flag system for this
}
```

---

## Adding Network Devices

Devices are stored in `GameState`. Add new ones in `GameState::InitializeNetworkDevices()`.

```cpp
void GameState::InitializeNetworkDevices() {
    AddDevice("192.168.1.100", {
        "192.168.1.100",           // IP
        "MyNetwork",               // ESSID/Name
        "MyPassword123",           // Password
        "Windows 11"               // Operating System
    });
    
    // Add more devices...
}
```

To use devices in tools:
```cpp
GameState::NetworkDevice* device = m_GameState->GetDevice("192.168.1.100");
if (device) {
    m_Terminal->AddLine("OS: " + device->os);
    m_Terminal->AddLine("Password: " + device->password);
}
```

---

## Working with Audio

The project includes miniaudio (header-only library).

### Basic Sound Playback

**Create AudioManager.h**
```cpp
#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <string>
#include <map>

class AudioManager {
public:
    AudioManager();
    ~AudioManager();
    
    bool Initialize();
    void Shutdown();
    
    bool LoadSound(const std::string& name, const std::string& filepath);
    void PlaySound(const std::string& name);
    void PlayMusic(const std::string& filepath, bool loop = true);
    void StopMusic();

private:
    ma_engine m_Engine;
    std::map<std::string, ma_sound> m_Sounds;
    ma_sound* m_CurrentMusic;
};

#endif
```

### Usage Example
```cpp
// In Engine::Initialize()
m_AudioManager = std::make_unique<AudioManager>();
m_AudioManager->Initialize();
m_AudioManager->LoadSound("error", "assets/sounds/error.wav");
m_AudioManager->LoadSound("success", "assets/sounds/success.wav");

// In code
m_AudioManager->PlaySound("error");
```

---

## Visual Effects

### Progress Bar System

**include/ui/ProgressBar.h**
```cpp
#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <string>

class Terminal;

class ProgressBar {
public:
    ProgressBar(int width = 50);
    
    void SetProgress(float percent); // 0.0 to 1.0
    std::string Render(const std::string& prefix = "") const;
    void DisplayToTerminal(Terminal* terminal, const std::string& prefix = "") const;

private:
    int m_Width;
    float m_Progress;
};

#endif
```

### CRT Shader Effect

For authentic terminal look, add a post-processing shader:

```glsl
// Fragment shader for CRT effect
uniform float time;
uniform vec2 resolution;

void main() {
    vec2 uv = gl_FragCoord.xy / resolution;
    
    // Scanlines
    float scanline = sin(uv.y * 800.0) * 0.04;
    
    // Curvature (optional)
    uv = uv * 2.0 - 1.0;
    uv *= 1.0 + 0.1 * length(uv);
    uv = (uv + 1.0) * 0.5;
    
    // Sample texture
    vec3 col = texture(screenTexture, uv).rgb;
    col -= scanline;
    
    // Vignette
    float vignette = smoothstep(0.7, 0.2, length(uv - 0.5));
    col *= vignette;
    
    gl_FragColor = vec4(col, 1.0);
}
```

---

## Learning Resources

### OpenGL & Graphics
- **LearnOpenGL.com** - Comprehensive modern OpenGL tutorial
  https://learnopengl.com/
- **OpenGL Tutorial** - More detailed graphics explanations
  http://www.opengl-tutorial.org/
- **The Cherno's OpenGL Series** (YouTube) - Great video tutorials
  https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2

### C++ Best Practices
- **C++ Core Guidelines** - Modern C++ standards
  https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
- **Effective Modern C++** (Book) by Scott Meyers
- **CPPReference** - Complete language reference
  https://en.cppreference.com/

### CMake
- **CMake Tutorial** - Official documentation
  https://cmake.org/cmake/help/latest/guide/tutorial/index.html
- **Modern CMake** - Best practices guide
  https://cliutils.gitlab.io/modern-cmake/

### Game Architecture
- **Game Programming Patterns** (Free online book)
  https://gameprogrammingpatterns.com/
- **Handmade Hero** - Educational game development series
  https://handmadehero.org/

### Audio Programming
- **miniaudio Documentation**
  https://miniaud.io/docs/manual/index.html
- **Audio Programming** - General concepts
  https://www.objc.io/issues/24-audio/audio-api-overview/

### Text Rendering
- **FreeType Documentation**
  https://freetype.org/freetype2/docs/documentation.html
- **Text Rendering Hacks** - LearnOpenGL
  https://learnopengl.com/In-Practice/Text-Rendering

### Design Patterns for This Project
- **Command Pattern** - Your command system
- **State Machine** - Game modes and tool states
- **Observer Pattern** - Event system (if needed)
- **Factory Pattern** - Creating tools/commands

### Useful Tools
- **RenderDoc** - Graphics debugging
  https://renderdoc.org/
- **Valgrind** - Memory leak detection (Linux)
- **Visual Studio Debugger** - Excellent for C++ debugging
- **GDB** - Command-line debugging

### ASCII Art Resources
- **Text to ASCII Art Generator (TAAG)**
  https://patorjk.com/software/taag/
- **ASCII Art Archive**
  https://www.asciiart.eu/
- **ASCII Generator** - Image to ASCII
  https://www.ascii-art-generator.org/

---

## Development Workflow Tips

### 1. Start Small
- Implement one command at a time
- Test thoroughly before moving to next feature
- Keep the Python version running for reference

### 2. Use Git
```bash
git init
git add .
git commit -m "Initial commit"

# Before adding new feature
git checkout -b feature/sscrack-tool
# ... make changes ...
git commit -am "Add SScrack tool"
git checkout main
git merge feature/sscrack-tool
```

### 3. Debug Effectively
- Use `std::cout` for quick debugging
- Learn your debugger (gdb/Visual Studio)
- Add logging system for complex issues

### 4. Performance Monitoring
```cpp
// Simple frame time monitoring
auto start = std::chrono::high_resolution_clock::now();
// ... do work ...
auto end = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
std::cout << "Frame time: " << duration.count() << "ms" << std::endl;
```

### 5. Code Organization
- One class per file
- Group related functionality
- Keep functions small (< 50 lines ideally)
- Comment complex logic

### 6. Testing Checklist
- [ ] Does it compile without warnings?
- [ ] No memory leaks (use Valgrind/sanitizers)
- [ ] Handles invalid input gracefully
- [ ] Works at different window sizes
- [ ] Framerate stays smooth (60+ FPS)

---

## Next Steps Roadmap

### Phase 1: Core Functionality
- [ ] Implement remaining simple commands (iwlist, nmap)
- [ ] Add progress bar utility
- [ ] Create Tool base class
- [ ] Test terminal scrolling with lots of output

### Phase 2: First Tool
- [ ] Implement SScrack or FTPea completely
- [ ] Add multi-stage input handling
- [ ] Test tool state management
- [ ] Add appropriate ASCII art banners

### Phase 3: Polish
- [ ] Integrate audio system
- [ ] Add boot animation improvements
- [ ] Implement CRT shader effect
- [ ] Add command history (up/down arrows)

### Phase 4: Remaining Tools
- [ ] Implement all tools from Python version
- [ ] Add remote connection system
- [ ] Implement MetaLib and payloads
- [ ] Add webcam image display

### Phase 5: Game Feel
- [ ] Sound effects for all actions
- [ ] Smooth animations
- [ ] Visual feedback
- [ ] Save/load system

---

## Common Pitfalls to Avoid

1. **Don't over-engineer early** - Get it working, then refactor
2. **Watch for memory leaks** - Use smart pointers
3. **Test input edge cases** - Empty strings, special characters
4. **Keep shaders simple** - Complex shaders tank performance
5. **Profile before optimizing** - Don't guess where bottlenecks are
6. **Commit often** - Small, logical commits are easier to debug
7. **Read compiler warnings** - They often catch bugs early

---

Good luck with your project! Start with small, testable additions and gradually build up complexity.