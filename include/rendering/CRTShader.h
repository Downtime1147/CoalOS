#ifndef CRTSHADER_H
#define CRTSHADER_H

#include <glad/glad.h>

class CRTShader {
public:
    CRTShader();
    ~CRTShader();

    bool Initialize(unsigned int width, unsigned int height);
    void Resize(unsigned int width, unsigned int height);
    
    // Begin rendering to framebuffer
    void BeginRender();
    
    // End rendering and apply CRT effect
    void EndRender();
    
    // Configuration
    void SetScanlineIntensity(float intensity) { m_ScanlineIntensity = intensity; }
    void SetCurvature(float amount) { m_Curvature = amount; }
    void SetVignetteStrength(float strength) { m_VignetteStrength = strength; }
    void SetChromaticAberration(float amount) { m_ChromaticAberration = amount; }
    void SetGlowIntensity(float intensity) { m_GlowIntensity = intensity; }
    void SetNoiseAmount(float amount) { m_NoiseAmount = amount; }
    void SetEnabled(bool enabled) { m_Enabled = enabled; }
    
    bool IsEnabled() const { return m_Enabled; }

private:
    bool CreateFramebuffer();
    bool CreateShader();
    void SetupQuad();
    
    unsigned int m_FBO;           // Framebuffer object
    unsigned int m_TextureColorbuffer; // Texture attachment
    unsigned int m_RBO;           // Renderbuffer object
    unsigned int m_QuadVAO, m_QuadVBO;
    unsigned int m_ShaderProgram;
    
    unsigned int m_Width, m_Height;
    float m_Time;
    
    // Effect parameters
    float m_ScanlineIntensity;
    float m_Curvature;
    float m_VignetteStrength;
    float m_ChromaticAberration;
    float m_GlowIntensity;
    float m_NoiseAmount;
    bool m_Enabled;
};

#endif // CRTSHADER_H
