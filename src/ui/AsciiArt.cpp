#include "ui/AsciiArt.h"
#include "ui/Terminal.h"
#include "rendering/TextRenderer.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <glm/glm.hpp>

AsciiArt::AsciiArt() {
}

AsciiArt::~AsciiArt() {
}

bool AsciiArt::LoadFromFile(const std::string& filepath) {
    return LoadLines(filepath, m_Lines);
}

bool AsciiArt::LoadAndCache(const std::string& name, const std::string& filepath) {
    std::vector<std::string> lines;
    if (LoadLines(filepath, lines)) {
        m_Cache[name] = lines;
        return true;
    }
    return false;
}

const std::vector<std::string>* AsciiArt::GetCached(const std::string& name) const {
    auto it = m_Cache.find(name);
    if (it != m_Cache.end()) {
        return &it->second;
    }
    return nullptr;
}

void AsciiArt::DisplayToTerminal(Terminal* terminal) const {
    if (!terminal) return;
    
    for (const auto& line : m_Lines) {
        terminal->AddLine(line);
    }
}

void AsciiArt::DisplayCachedToTerminal(const std::string& name, Terminal* terminal) const {
    if (!terminal) return;
    
    const auto* lines = GetCached(name);
    if (lines) {
        for (const auto& line : *lines) {
            terminal->AddLine(line);
        }
    } else {
        std::cerr << "ASCII art '" << name << "' not found in cache" << std::endl;
    }
}

void AsciiArt::RenderDirect(TextRenderer* renderer, float x, float y, float scale) const {
    if (!renderer) return;
    
    glm::vec3 color(0.0f, 1.0f, 0.0f); // Green
    float lineHeight = renderer->GetFontHeight() * scale * 1.2f;
    float currentY = y;
    
    for (const auto& line : m_Lines) {
        renderer->RenderText(line, x, currentY, scale, color);
        currentY -= lineHeight;
    }
}

void AsciiArt::RenderCachedDirect(const std::string& name, TextRenderer* renderer, float x, float y, float scale) const {
    if (!renderer) return;
    
    const auto* lines = GetCached(name);
    if (!lines) {
        std::cerr << "ASCII art '" << name << "' not found in cache" << std::endl;
        return;
    }
    
    glm::vec3 color(0.0f, 1.0f, 0.0f); // Green
    float lineHeight = renderer->GetFontHeight() * scale * 1.2f;
    float currentY = y;
    
    for (const auto& line : *lines) {
        renderer->RenderText(line, x, currentY, scale, color);
        currentY -= lineHeight;
    }
}

size_t AsciiArt::GetMaxWidth() const {
    size_t maxWidth = 0;
    for (const auto& line : m_Lines) {
        maxWidth = std::max(maxWidth, line.length());
    }
    return maxWidth;
}

void AsciiArt::Clear() {
    m_Lines.clear();
}

void AsciiArt::ClearCache() {
    m_Cache.clear();
}

bool AsciiArt::LoadLines(const std::string& filepath, std::vector<std::string>& outLines) {
    std::ifstream file(filepath);
    
    if (!file.is_open()) {
        std::cerr << "Failed to open ASCII art file: " << filepath << std::endl;
        return false;
    }
    
    outLines.clear();
    std::string line;
    
    while (std::getline(file, line)) {
        outLines.push_back(line);
    }
    
    file.close();
    
    if (outLines.empty()) {
        std::cerr << "Warning: ASCII art file is empty: " << filepath << std::endl;
        return false;
    }
    
    std::cout << "Loaded ASCII art: " << filepath << " (" << outLines.size() << " lines)" << std::endl;
    return true;
}
