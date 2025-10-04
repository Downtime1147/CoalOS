#include "rendering/CRTShader.h"
#include <iostream>
#include <GLFW/glfw3.h>

// CRT Post-processing shader
const char* crtVertexShader = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
}
)";

const char* crtFragmentShader = R"(
#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float time;
uniform vec2 resolution;
uniform float scanlineIntensity;
uniform float curvature;
uniform float vignetteStrength;
uniform float chromaticAberration;
uniform float glowIntensity;
uniform float noiseAmount;

// Random noise function
float random(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

// CRT screen curvature
vec2 curveScreen(vec2 uv, float amount) {
    uv = uv * 2.0 - 1.0;
    vec2 offset = abs(uv.yx) / vec2(6.0, 4.0);
    uv = uv + uv * offset * offset * amount;
    uv = uv * 0.5 + 0.5;
    return uv;
}

void main()
{
    vec2 uv = TexCoords;
    
    // Apply curvature
    if (curvature > 0.0) {
        uv = curveScreen(uv, curvature);
    }
    
    // Out of bounds check for curved screen
    if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0) {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }
    
    // Chromatic aberration
    vec3 col;
    if (chromaticAberration > 0.0) {
        float aberration = chromaticAberration * 0.002;
        col.r = texture(screenTexture, vec2(uv.x + aberration, uv.y)).r;
        col.g = texture(screenTexture, uv).g;
        col.b = texture(screenTexture, vec2(uv.x - aberration, uv.y)).b;
    } else {
        col = texture(screenTexture, uv).rgb;
    }
    
    // Scanlines
    if (scanlineIntensity > 0.0) {
        float scanline = sin(uv.y * resolution.y * 2.0) * scanlineIntensity;
        col -= scanline;
    }
    
    // Horizontal scanline roll effect
    float roll = sin(uv.y * 100.0 + time * 2.0) * 0.002;
    col += roll;
    
    // Vignette
    if (vignetteStrength > 0.0) {
        vec2 vignetteUV = uv * (1.0 - uv.yx);
        float vignette = vignetteUV.x * vignetteUV.y * 20.0;  // Changed from 15.0 to 20.0 (less dark)
        vignette = pow(vignette, vignetteStrength * 0.5);     // Reduced strength multiplier
        col *= vignette;
    }
    
    // Phosphor glow/bloom
    if (glowIntensity > 0.0) {
        vec3 glow = texture(screenTexture, uv).rgb * glowIntensity;
        col += glow * 0.3;
    }
    
    // Screen flicker (reduced intensity)
    float flicker = 0.98 + 0.02 * sin(time * 50.0);  // Changed from 0.95-1.05 to 0.98-1.0
    col *= flicker;
    
    // Random noise/grain
    if (noiseAmount > 0.0) {
        float noise = random(uv * time) * noiseAmount;
        col += noise * 0.1;
    }
    
    // Output final color (removed warm color temperature shift for better visibility)
    FragColor = vec4(col, 1.0);
}
)";

CRTShader::CRTShader() 
    : m_FBO(0), m_TextureColorbuffer(0), m_RBO(0), 
      m_QuadVAO(0), m_QuadVBO(0), m_ShaderProgram(0),
      m_Width(0), m_Height(0), m_Time(0.0f),
      m_ScanlineIntensity(0.03f), m_Curvature(0.05f),     // Reduced from 0.08 and 0.15
      m_VignetteStrength(0.15f), m_ChromaticAberration(0.3f), // Reduced from 0.4 and 1.0
      m_GlowIntensity(0.1f), m_NoiseAmount(0.02f), m_Enabled(true) { // Reduced from 0.2 and 0.05
}

CRTShader::~CRTShader() {
    if (m_FBO) glDeleteFramebuffers(1, &m_FBO);
    if (m_TextureColorbuffer) glDeleteTextures(1, &m_TextureColorbuffer);
    if (m_RBO) glDeleteRenderbuffers(1, &m_RBO);
    if (m_QuadVAO) glDeleteVertexArrays(1, &m_QuadVAO);
    if (m_QuadVBO) glDeleteBuffers(1, &m_QuadVBO);
    if (m_ShaderProgram) glDeleteProgram(m_ShaderProgram);
}

bool CRTShader::Initialize(unsigned int width, unsigned int height) {
    m_Width = width;
    m_Height = height;
    
    if (!CreateShader()) {
        return false;
    }
    
    if (!CreateFramebuffer()) {
        return false;
    }
    
    SetupQuad();
    
    std::cout << "CRT Shader initialized successfully" << std::endl;
    return true;
}

void CRTShader::Resize(unsigned int width, unsigned int height) {
    m_Width = width;
    m_Height = height;
    
    // Recreate framebuffer with new size
    if (m_FBO) glDeleteFramebuffers(1, &m_FBO);
    if (m_TextureColorbuffer) glDeleteTextures(1, &m_TextureColorbuffer);
    if (m_RBO) glDeleteRenderbuffers(1, &m_RBO);
    
    CreateFramebuffer();
}

bool CRTShader::CreateFramebuffer() {
    // Generate framebuffer
    glGenFramebuffers(1, &m_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    
    // Create texture attachment
    glGenTextures(1, &m_TextureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, m_TextureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureColorbuffer, 0);
    
    // Create renderbuffer for depth and stencil
    glGenRenderbuffers(1, &m_RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
    
    // Check framebuffer completeness
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "ERROR: Framebuffer is not complete!" << std::endl;
        return false;
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

bool CRTShader::CreateShader() {
    // Compile vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &crtVertexShader, NULL);
    glCompileShader(vertexShader);
    
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR: CRT Vertex shader compilation failed\n" << infoLog << std::endl;
        return false;
    }
    
    // Compile fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &crtFragmentShader, NULL);
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR: CRT Fragment shader compilation failed\n" << infoLog << std::endl;
        return false;
    }
    
    // Link shader program
    m_ShaderProgram = glCreateProgram();
    glAttachShader(m_ShaderProgram, vertexShader);
    glAttachShader(m_ShaderProgram, fragmentShader);
    glLinkProgram(m_ShaderProgram);
    
    glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_ShaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR: CRT Shader program linking failed\n" << infoLog << std::endl;
        return false;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return true;
}

void CRTShader::SetupQuad() {
    float quadVertices[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    
    glGenVertexArrays(1, &m_QuadVAO);
    glGenBuffers(1, &m_QuadVBO);
    glBindVertexArray(m_QuadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void CRTShader::BeginRender() {
    if (!m_Enabled) return;
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CRTShader::EndRender() {
    if (!m_Enabled) return;
    
    // Bind default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Use CRT shader
    glUseProgram(m_ShaderProgram);
    
    // Update time
    m_Time = static_cast<float>(glfwGetTime());
    
    // Set uniforms
    glUniform1f(glGetUniformLocation(m_ShaderProgram, "time"), m_Time);
    glUniform2f(glGetUniformLocation(m_ShaderProgram, "resolution"), 
                static_cast<float>(m_Width), static_cast<float>(m_Height));
    glUniform1f(glGetUniformLocation(m_ShaderProgram, "scanlineIntensity"), m_ScanlineIntensity);
    glUniform1f(glGetUniformLocation(m_ShaderProgram, "curvature"), m_Curvature);
    glUniform1f(glGetUniformLocation(m_ShaderProgram, "vignetteStrength"), m_VignetteStrength);
    glUniform1f(glGetUniformLocation(m_ShaderProgram, "chromaticAberration"), m_ChromaticAberration);
    glUniform1f(glGetUniformLocation(m_ShaderProgram, "glowIntensity"), m_GlowIntensity);
    glUniform1f(glGetUniformLocation(m_ShaderProgram, "noiseAmount"), m_NoiseAmount);
    
    // Bind texture
    glBindVertexArray(m_QuadVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_TextureColorbuffer);
    
    // Draw quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glBindVertexArray(0);
}
