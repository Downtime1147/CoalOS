#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include <string>

struct Character {
    unsigned int TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    unsigned int Advance;
};

class TextRenderer {
public:
    TextRenderer(unsigned int width, unsigned int height);
    ~TextRenderer();

    bool Initialize(const std::string& fontPath, unsigned int fontSize);
    void RenderText(const std::string& text, float x, float y, float scale, glm::vec3 color);
    void UpdateProjection(unsigned int width, unsigned int height);

    unsigned int GetFontHeight() const { return m_FontHeight; }
    unsigned int GetCharWidth(char c) const;

private:
    std::map<char, Character> m_Characters;
    unsigned int m_VAO, m_VBO;
    unsigned int m_ShaderProgram;
    glm::mat4 m_Projection;
    unsigned int m_FontHeight;

    bool LoadFont(const std::string& fontPath, unsigned int fontSize);
    bool CreateShaders();
};

#endif // TEXTRENDERER_H
