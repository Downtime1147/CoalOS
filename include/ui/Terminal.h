#ifndef TERMINAL_H
#define TERMINAL_H

#include <string>
#include <vector>
#include <deque>
#include <glm/glm.hpp>
#include "rendering/TextRenderer.h"

class Terminal {
public:
    Terminal(unsigned int width, unsigned int height);
    ~Terminal();

    void Initialize();
    void Update(float deltaTime);
    void Render(TextRenderer* renderer);

    void AddLine(const std::string& line);
    void AddChar(char c);
    void DeleteChar();
    void SubmitInput();
    
    // Typewriter effect
    void AddLineWithTypewriter(const std::string& line, float charsPerSecond = 50.0f);
    void SetTypewriterSpeed(float charsPerSecond) { m_TypewriterSpeed = charsPerSecond; }
    bool IsTyping() const { return m_IsTyping; }
    
    std::string GetCurrentInput() const { return m_CurrentInput; }
    void SetPrompt(const std::string& prompt) { m_Prompt = prompt; }
    void Clear();
    
    // Color management
    void SetTextColor(float r, float g, float b);
    glm::vec3 GetTextColor() const { return m_TextColor; }

private:
    unsigned int m_Width;
    unsigned int m_Height;
    
    std::deque<std::string> m_Lines;
    std::string m_CurrentInput;
    std::string m_Prompt;
    glm::vec3 m_TextColor;  // RGB color
    
    // Typewriter effect state
    bool m_IsTyping;
    std::string m_TypewriterBuffer;
    std::string m_CurrentTypingLine;
    float m_TypewriterTimer;
    float m_TypewriterSpeed;  // characters per second
    size_t m_TypewriterIndex;
    
    unsigned int m_MaxVisibleLines;
    float m_CursorBlinkTimer;
    bool m_CursorVisible;
    
    const float CURSOR_BLINK_RATE = 0.5f;
    const float LINE_HEIGHT = 20.0f;
    const float PADDING_LEFT = 10.0f;
    const float PADDING_TOP = 10.0f;
};

#endif // TERMINAL_H