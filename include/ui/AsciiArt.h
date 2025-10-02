#ifndef ASCIIART_H
#define ASCIIART_H

#include <string>
#include <vector>
#include <map>

class Terminal;
class TextRenderer;

class AsciiArt {
public:
    AsciiArt();
    ~AsciiArt();

    // Load ASCII art from a file
    bool LoadFromFile(const std::string& filepath);
    
    // Load and cache ASCII art with a name
    bool LoadAndCache(const std::string& name, const std::string& filepath);
    
    // Get cached ASCII art
    const std::vector<std::string>* GetCached(const std::string& name) const;
    
    // Display ASCII art to terminal
    void DisplayToTerminal(Terminal* terminal) const;
    void DisplayCachedToTerminal(const std::string& name, Terminal* terminal) const;
    
    // Render ASCII art directly (without going through terminal)
    void RenderDirect(TextRenderer* renderer, float x, float y, float scale) const;
    void RenderCachedDirect(const std::string& name, TextRenderer* renderer, float x, float y, float scale) const;
    
    // Get dimensions
    size_t GetLineCount() const { return m_Lines.size(); }
    size_t GetMaxWidth() const;
    
    // Clear current art
    void Clear();
    
    // Clear cache
    void ClearCache();

private:
    std::vector<std::string> m_Lines;
    std::map<std::string, std::vector<std::string>> m_Cache;
    
    bool LoadLines(const std::string& filepath, std::vector<std::string>& outLines);
};

#endif // ASCIIART_H
