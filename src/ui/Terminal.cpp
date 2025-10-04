#include "ui/Terminal.h"
#include <glm/glm.hpp>

Terminal::Terminal(unsigned int width, unsigned int height)
    : m_Width(width), m_Height(height), m_Prompt(""), 
      m_CursorBlinkTimer(0.0f), m_CursorVisible(true),
      m_TextColor(1.0f, 0.5f, 0.0f),  // Default green
      m_IsTyping(false), m_TypewriterTimer(0.0f), 
      m_TypewriterSpeed(50.0f), m_TypewriterIndex(0) {
    m_MaxVisibleLines = static_cast<unsigned int>((height - PADDING_TOP * 2) / LINE_HEIGHT);
}

Terminal::~Terminal() {
}

void Terminal::Initialize() {
    m_Lines.clear();
    m_CurrentInput.clear();
}

void Terminal::Update(float deltaTime) {
    // Update cursor blink
    m_CursorBlinkTimer += deltaTime;
    if (m_CursorBlinkTimer >= CURSOR_BLINK_RATE) {
        m_CursorVisible = !m_CursorVisible;
        m_CursorBlinkTimer = 0.0f;
    }
    
    // Update typewriter effect
    if (m_IsTyping) {
        m_TypewriterTimer += deltaTime;
        float timePerChar = 1.0f / m_TypewriterSpeed;
        
        while (m_TypewriterTimer >= timePerChar && m_TypewriterIndex < m_TypewriterBuffer.length()) {
            m_CurrentTypingLine += m_TypewriterBuffer[m_TypewriterIndex];
            m_TypewriterIndex++;
            m_TypewriterTimer -= timePerChar;
            
            // Update the last line in the buffer
            if (!m_Lines.empty()) {
                m_Lines.back() = m_CurrentTypingLine;
            }
        }
        
        // Finish typing
        if (m_TypewriterIndex >= m_TypewriterBuffer.length()) {
            m_IsTyping = false;
            m_TypewriterBuffer.clear();
            m_CurrentTypingLine.clear();
            m_TypewriterIndex = 0;
            m_TypewriterTimer = 0.0f;
        }
    }
}

void Terminal::Render(TextRenderer* renderer) {
    if (!renderer) return;

    float y = m_Height - PADDING_TOP;

    // Calculate how many lines to skip (for scrolling)
    size_t startLine = 0;
    if (m_Lines.size() > m_MaxVisibleLines - 1) {
        startLine = m_Lines.size() - (m_MaxVisibleLines - 1);
    }

    // Render scrolled lines
    for (size_t i = startLine; i < m_Lines.size(); ++i) {
        y -= LINE_HEIGHT;
        renderer->RenderText(m_Lines[i], PADDING_LEFT, y, 1.0f, m_TextColor);
    }

    // Render current input line with prompt
    y -= LINE_HEIGHT;
    std::string inputLine = m_Prompt + m_CurrentInput;
    
    // Add cursor
    if (m_CursorVisible && !m_Prompt.empty()) {
        inputLine += "_";
    }
    
    renderer->RenderText(inputLine, PADDING_LEFT, y, 1.0f, m_TextColor);
}

void Terminal::AddLine(const std::string& line) {
    m_Lines.push_back(line);
    
    // Limit history (keep last 1000 lines)
    if (m_Lines.size() > 1000) {
        m_Lines.pop_front();
    }
}

void Terminal::AddChar(char c) {
    m_CurrentInput += c;
}

void Terminal::DeleteChar() {
    if (!m_CurrentInput.empty()) {
        m_CurrentInput.pop_back();
    }
}

void Terminal::SubmitInput() {
    // Add the input line to history
    AddLine(m_Prompt + m_CurrentInput);
    m_CurrentInput.clear();
}

void Terminal::Clear() {
    m_Lines.clear();
}

void Terminal::SetTextColor(float r, float g, float b) {
    m_TextColor = glm::vec3(r, g, b);
}

void Terminal::AddLineWithTypewriter(const std::string& line, float charsPerSecond) {
    if (m_IsTyping) {
        // If already typing, queue it as a normal line
        AddLine(line);
        return;
    }
    
    m_IsTyping = true;
    m_TypewriterBuffer = line;
    m_CurrentTypingLine.clear();
    m_TypewriterIndex = 0;
    m_TypewriterTimer = 0.0f;
    m_TypewriterSpeed = charsPerSecond;
    
    // Add an empty line that will be filled
    m_Lines.push_back("");
}
