#include "ui/Terminal.h"
#include <glm/glm.hpp>

Terminal::Terminal(unsigned int width, unsigned int height)
    : m_Width(width), m_Height(height), m_Prompt(""), 
      m_CursorBlinkTimer(0.0f), m_CursorVisible(true) {
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
}

void Terminal::Render(TextRenderer* renderer) {
    if (!renderer) return;

    float y = m_Height - PADDING_TOP;
    glm::vec3 textColor(0.0f, 1.0f, 0.0f); // Green terminal text

    // Calculate how many lines to skip (for scrolling)
    size_t startLine = 0;
    if (m_Lines.size() > m_MaxVisibleLines - 1) {
        startLine = m_Lines.size() - (m_MaxVisibleLines - 1);
    }

    // Render scrolled lines
    for (size_t i = startLine; i < m_Lines.size(); ++i) {
        y -= LINE_HEIGHT;
        renderer->RenderText(m_Lines[i], PADDING_LEFT, y, 1.0f, textColor);
    }

    // Render current input line with prompt
    y -= LINE_HEIGHT;
    std::string inputLine = m_Prompt + m_CurrentInput;
    
    // Add cursor
    if (m_CursorVisible && !m_Prompt.empty()) {
        inputLine += "_";
    }
    
    renderer->RenderText(inputLine, PADDING_LEFT, y, 1.0f, textColor);
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
