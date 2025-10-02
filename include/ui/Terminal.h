#ifndef TERMINAL_H
#define TERMINAL_H

#include <string>
#include <vector>
#include <deque>
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
    
    std::string GetCurrentInput() const { return m_CurrentInput; }
    void SetPrompt(const std::string& prompt) { m_Prompt = prompt; }
    void Clear();

private:
    unsigned int m_Width;
    unsigned int m_Height;
    
    std::deque<std::string> m_Lines;
    std::string m_CurrentInput;
    std::string m_Prompt;
    
    unsigned int m_MaxVisibleLines;
    float m_CursorBlinkTimer;
    bool m_CursorVisible;
    
    const float CURSOR_BLINK_RATE = 0.5f;
    const float LINE_HEIGHT = 20.0f;
    const float PADDING_LEFT = 10.0f;
    const float PADDING_TOP = 10.0f;
};

#endif // TERMINAL_H
