#include "core/Engine.h"
#include <iostream>
#include <GLFW/glfw3.h>

Engine::Engine(unsigned int width, unsigned int height)
    : m_Width(width), m_Height(height), m_IsBooting(true), m_BootTimer(0.0f) {
}

Engine::~Engine() {
}

bool Engine::Initialize() {
    std::cout << "Initializing Engine..." << std::endl;

    // Initialize text renderer
    m_TextRenderer = std::make_unique<TextRenderer>(m_Width, m_Height);
    if (!m_TextRenderer->Initialize("assets/fonts/monospace.ttf", 16)) {
        std::cerr << "Failed to initialize text renderer" << std::endl;
        return false;
    }

    // Initialize terminal
    m_Terminal = std::make_unique<Terminal>(m_Width, m_Height);
    m_Terminal->Initialize();

    // Initialize file system
    m_FileSystem = std::make_unique<FileSystem>();
    m_FileSystem->AddFile("rockyou.pwd"); // Starting file

    // Initialize command parser
    m_CommandParser = std::make_unique<CommandParser>(m_FileSystem.get(), m_Terminal.get());
    m_CommandParser->Initialize();

    // Initialize game state
    m_GameState = std::make_unique<GameState>();

    // Add initial boot message
    m_Terminal->AddLine("CoalOS Boot Loader v1.0");
    m_Terminal->AddLine("Initializing...");
    m_Terminal->AddLine("");

    std::cout << "Engine initialized successfully" << std::endl;
    return true;
}

void Engine::Update(float deltaTime) {
    if (m_IsBooting) {
        ProcessBootSequence(deltaTime);
    }

    m_Terminal->Update(deltaTime);
}

void Engine::Render() {
    // Render terminal
    m_Terminal->Render(m_TextRenderer.get());
}

void Engine::Shutdown() {
    std::cout << "Shutting down engine..." << std::endl;
}

void Engine::OnResize(int width, int height) {
    m_Width = width;
    m_Height = height;
    if (m_TextRenderer) {
        m_TextRenderer->UpdateProjection(width, height);
    }
}

void Engine::OnKeyPress(int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS && action != GLFW_REPEAT) {
        return;
    }

    // Skip input during boot
    if (m_IsBooting) {
        return;
    }

    // Handle special keys
    if (key == GLFW_KEY_ENTER) {
        std::string command = m_Terminal->GetCurrentInput();
        m_Terminal->SubmitInput();
        m_CommandParser->ParseAndExecute(command);
    }
    else if (key == GLFW_KEY_BACKSPACE) {
        m_Terminal->DeleteChar();
    }
    else if (key == GLFW_KEY_UP) {
        // TODO: Command history navigation
    }
    else if (key == GLFW_KEY_DOWN) {
        // TODO: Command history navigation
    }
    else if (key >= 32 && key <= 126) { // Printable ASCII characters
        char c = static_cast<char>(key);
        // Handle shift for uppercase and symbols
        if (!(mods & GLFW_MOD_SHIFT)) {
            c = tolower(c);
        }
        m_Terminal->AddChar(c);
    }
}

void Engine::ProcessBootSequence(float deltaTime) {
    m_BootTimer += deltaTime;

    // Simple boot sequence - add messages at intervals
    static int bootStage = 0;
    
    if (bootStage == 0 && m_BootTimer > 1.0f) {
        m_Terminal->AddLine("Calculated RAMsize: 66816233991524 Mb");
        m_Terminal->AddLine("Initiating Setup...");
        bootStage++;
    }
    else if (bootStage == 1 && m_BootTimer > 2.0f) {
        m_Terminal->AddLine("BIOS load sequence:");
        m_Terminal->AddLine("  stepfunction(), load_clientside.jss...");
        m_Terminal->AddLine("  root access obtained!");
        bootStage++;
    }
    else if (bootStage == 2 && m_BootTimer > 3.5f) {
        m_Terminal->AddLine("Importing additional Dependencies and utils:");
        m_Terminal->AddLine("  - FTpea ver.8.4.6 ... [ OK ]");
        m_Terminal->AddLine("  - SScrack ver.2.1.3 ... [ OK ]");
        m_Terminal->AddLine("  - Nmap ver.8.9.1 ... [ OK ]");
        bootStage++;
    }
    else if (bootStage == 3 && m_BootTimer > 5.0f) {
        m_Terminal->AddLine("");
        m_Terminal->AddLine("! Setup Complete !");
        m_Terminal->AddLine("");
        m_Terminal->AddLine("Coal OS - ver 1.4.6");
        m_Terminal->AddLine("Developed by 'Downtime' and 'Dr. Mass'");
        m_Terminal->AddLine("----------------------------------------------");
        m_Terminal->AddLine("");
        m_Terminal->SetPrompt("root:~$ ");
        m_IsBooting = false;
        bootStage++;
    }
}
