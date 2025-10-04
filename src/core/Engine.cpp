#include "core/Engine.h"
#include "rendering/CRTShader.h"
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

    // Initialize game state
    m_GameState = std::make_unique<GameState>();

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
