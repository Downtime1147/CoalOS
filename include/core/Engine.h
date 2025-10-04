#ifndef ENGINE_H
#define ENGINE_H

#include <memory>
#include <string>
#include "rendering/TextRenderer.h"
#include "rendering/CRTShader.h"
#include "ui/Terminal.h"
#include "ui/AsciiArt.h"
#include "systems/CommandParser.h"
#include "systems/FileSystem.h"
#include "systems/SaveManager.h"
#include "core/GameState.h"
#include "core/Settings.h"

class CRTShader;

class Engine {
public:
    Engine(unsigned int width, unsigned int height);
    ~Engine();

    bool Initialize();
    void Update(float deltaTime);
    void Render();
    void Shutdown();

    void OnResize(int width, int height);
    void OnKeyPress(int key, int scancode, int action, int mods);
    
    // Public save function for CommandParser access
    void SaveGameData();
    void SaveSettings();
    void ApplySettings();

private:
    unsigned int m_Width;
    unsigned int m_Height;

    std::unique_ptr<TextRenderer> m_TextRenderer;
    std::unique_ptr<CRTShader> m_CRTShader;
    std::unique_ptr<Terminal> m_Terminal;
    std::unique_ptr<CommandParser> m_CommandParser;
    std::unique_ptr<FileSystem> m_FileSystem;
    std::unique_ptr<GameState> m_GameState;
    std::unique_ptr<AsciiArt> m_AsciiArt;
    std::unique_ptr<SaveManager> m_SaveManager;
    
    Settings m_Settings;

    bool m_IsBooting;
    float m_BootTimer;

    void ProcessBootSequence(float deltaTime);
    void GenerateNewGameData();
};

#endif // ENGINE_H
