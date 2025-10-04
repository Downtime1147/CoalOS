#include <iostream>
#include "core/Engine.h"
#include "rendering/CRTShader.h"
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
    if (!m_TextRenderer->Initialize("assets/fonts/monospace.ttf", 20)) {  // Changed from 16 to 20
        std::cerr << "Failed to initialize text renderer" << std::endl;
        return false;
    }
    
    // Initialize CRT shader
    m_CRTShader = std::make_unique<CRTShader>();
    if (!m_CRTShader->Initialize(m_Width, m_Height)) {
        std::cerr << "Failed to initialize CRT shader" << std::endl;
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
    m_CommandParser->SetCRTShader(m_CRTShader.get());  // Give access to CRT shader
    m_CommandParser->SetEngine(this);  // Give access to Engine for saving

    // Initialize game state
    m_GameState = std::make_unique<GameState>();
    
    // Initialize save manager
    m_SaveManager = std::make_unique<SaveManager>();
    
    // Load settings first
    if (Settings::SettingsExist("settings.json")) {
        m_Settings.LoadFromFile("settings.json");
    } else {
        m_Settings = Settings::GetDefaults();
        m_Settings.SaveToFile("settings.json");
    }
    
    // Apply loaded settings
    ApplySettings();
    
    // Load or create save data
    std::vector<std::string> savedInventory;
    std::vector<GameState::NetworkDevice> savedDevices;
    
    if (m_SaveManager->SaveExists("save.json")) {
        std::cout << "Loading existing save..." << std::endl;
        if (m_SaveManager->LoadGame("save.json", savedInventory, savedDevices)) {
            // Restore inventory
            for (const auto& file : savedInventory) {
                m_FileSystem->AddFile(file);
            }
            // Restore network devices
            m_GameState->LoadDevices(savedDevices);
        } else {
            std::cerr << "Failed to load save, generating new data..." << std::endl;
            GenerateNewGameData();
        }
    } else {
        std::cout << "No save found, generating new game data..." << std::endl;
        GenerateNewGameData();
    }

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
    // Begin rendering to CRT framebuffer
    m_CRTShader->BeginRender();
    
    // Render terminal
    m_Terminal->Render(m_TextRenderer.get());
    
    // Apply CRT effect
    m_CRTShader->EndRender();
}

void Engine::Shutdown() {
    std::cout << "Shutting down engine..." << std::endl;
    
    // Save settings and game data before shutdown
    SaveSettings();
    SaveGameData();
}

void Engine::GenerateNewGameData() {
    // Generate random network devices
    auto devices = SaveManager::GenerateRandomNetworks(20);
    m_GameState->LoadDevices(devices);
    
    // Save initial data
    SaveGameData();
}

void Engine::SaveGameData() {
    if (!m_SaveManager || !m_FileSystem || !m_GameState) {
        return;
    }
    
    std::vector<std::string> inventory = m_FileSystem->ListFiles();
    std::vector<GameState::NetworkDevice> devices = m_GameState->GetAllDevices();
    
    m_SaveManager->SaveGame("save.json", inventory, devices);
}

void Engine::SaveSettings() {
    // Collect current settings from components
    m_Settings.textColor = m_Terminal->GetTextColor();
    m_Settings.typewriterSpeed = 50.0f; // Terminal doesn't expose this currently
    
    if (m_CRTShader) {
        m_Settings.crtEnabled = m_CRTShader->IsEnabled();
        // CRT shader doesn't expose getters, so settings are already stored in m_Settings
    }
    
    m_Settings.SaveToFile("settings.json");
}

void Engine::ApplySettings() {
    // Apply text color
    if (m_Terminal) {
        m_Terminal->SetTextColor(m_Settings.textColor.r, m_Settings.textColor.g, m_Settings.textColor.b);
    }
    
    // Apply typewriter speed
    if (m_Terminal) {
        m_Terminal->SetTypewriterSpeed(m_Settings.typewriterSpeed);
    }
    
    // Apply CRT settings
    if (m_CRTShader) {
        m_CRTShader->SetEnabled(m_Settings.crtEnabled);
        m_CRTShader->SetScanlineIntensity(m_Settings.scanlineIntensity);
        m_CRTShader->SetCurvature(m_Settings.curvature);
        m_CRTShader->SetVignetteStrength(m_Settings.vignetteStrength);
        m_CRTShader->SetChromaticAberration(m_Settings.chromaticAberration);
        m_CRTShader->SetGlowIntensity(m_Settings.glowIntensity);
        m_CRTShader->SetNoiseAmount(m_Settings.noiseAmount);
    }
}

void Engine::OnResize(int width, int height) {
    m_Width = width;
    m_Height = height;
    if (m_TextRenderer) {
        m_TextRenderer->UpdateProjection(width, height);
    }
    if (m_CRTShader) {
        m_CRTShader->Resize(width, height);
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
        m_Terminal->AddLineWithTypewriter("Calculated RAMsize: 66816233991524 Mb", 80.0f);
        m_Terminal->AddLineWithTypewriter("Initiating Setup...", 60.0f);
        bootStage++;
    }
    else if (bootStage == 1 && m_BootTimer > 2.0f && !m_Terminal->IsTyping()) {
        m_Terminal->AddLineWithTypewriter("BIOS load sequence:", 50.0f);
        m_Terminal->AddLineWithTypewriter("  stepfunction(), load_clientside.jss...", 80.0f);
        m_Terminal->AddLineWithTypewriter("  root access obtained!", 70.0f);
        bootStage++;
    }
    else if (bootStage == 2 && m_BootTimer > 3.5f && !m_Terminal->IsTyping()) {
        m_Terminal->AddLineWithTypewriter("Importing additional Dependencies and utils:", 60.0f);
        m_Terminal->AddLineWithTypewriter("  - FTpea ver.8.4.6 ... [ OK ]", 100.0f);
        m_Terminal->AddLineWithTypewriter("  - SScrack ver.2.1.3 ... [ OK ]", 100.0f);
        m_Terminal->AddLineWithTypewriter("  - Nmap ver.8.9.1 ... [ OK ]", 100.0f);
        bootStage++;
    }
    else if (bootStage == 3 && m_BootTimer > 5.0f && !m_Terminal->IsTyping()) {
        m_Terminal->AddLine("");
        m_Terminal->AddLineWithTypewriter("! Setup Complete !", 40.0f);
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
