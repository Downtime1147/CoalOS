# CoalOS Aesthetic Features Guide

## Summary of Changes

You now have three major aesthetic improvements:

1. **Larger text** (20pt instead of 16pt)
2. **Typewriter effect** for character-by-character display
3. **CRT shader** with multiple customizable effects

---

## New Commands

### `color` - Change Text Color
```bash
# Presets
color green
color amber
color cyan
color white
color red
color purple

# Custom RGB (0.0 to 1.0)
color rgb 1.0 0.5 0.0
```

### `crt` - Control CRT Effect
```bash
# Toggle on/off
crt on
crt off

# Adjust individual effects (0.0 to 1.0, chroma 0.0 to 2.0)
crt scanline 0.1    # Scanline intensity
crt curve 0.2       # Screen curvature
crt vignette 0.5    # Edge darkening
crt glow 0.3        # Phosphor glow
crt noise 0.05      # Static/grain
crt chroma 1.5      # Color separation

# Show help
crt
```

### `speed` - Typewriter Speed
```bash
speed 20     # Slow
speed 50     # Normal (default)
speed 100    # Fast  
speed 1000   # Instant
```

---

## Using Typewriter Effect in Code

### Basic Usage
```cpp
// In any command or function
m_Terminal->AddLineWithTypewriter("This text appears character by character", 50.0f);
```

### Parameters
- **First parameter**: The text string
- **Second parameter**: Characters per second (optional, defaults to 50.0)

### Example in a Command
```cpp
void CommandParser::CmdSomething(const std::vector<std::string>& args) {
    m_Terminal->AddLine("");  // Instant
    m_Terminal->AddLineWithTypewriter("Scanning for vulnerabilities...", 60.0f);
    m_Terminal->AddLineWithTypewriter("Found 3 open ports", 40.0f);
    m_Terminal->AddLine("");  // Instant
}
```

### Wait for Typing to Finish
```cpp
// Check if still typing
if (m_Terminal->IsTyping()) {
    // Don't add more typed text yet
    return;
}

// Safe to add more typewriter lines
m_Terminal->AddLineWithTypewriter("Next line", 50.0f);
```

### Mixed Usage
```cpp
// Instant line
m_Terminal->AddLine("Initiating attack...");

// Slow dramatic line
m_Terminal->AddLineWithTypewriter("Access granted.", 30.0f);

// Instant line
m_Terminal->AddLine("");
```

---

## CRT Shader Configuration

### In Code (Engine.cpp or elsewhere)
```cpp
// Access through CommandParser or directly
m_CRTShader->SetEnabled(true);
m_CRTShader->SetScanlineIntensity(0.08f);  // Subtle scanlines
m_CRTShader->SetCurvature(0.15f);          // Moderate curve
m_CRTShader->SetVignetteStrength(0.4f);    // Edge darkening
m_CRTShader->SetChromaticAberration(1.0f); // Color separation
m_CRTShader->SetGlowIntensity(0.2f);       // Phosphor glow
m_CRTShader->SetNoiseAmount(0.05f);        // Grain/static
```

### Default Values (Good Starting Point)
- Scanline: 0.08
- Curvature: 0.15
- Vignette: 0.4
- Chromatic Aberration: 1.0
- Glow: 0.2
- Noise: 0.05

### Recommended Presets

**Minimal CRT (subtle)**
```cpp
m_CRTShader->SetScanlineIntensity(0.03f);
m_CRTShader->SetCurvature(0.05f);
m_CRTShader->SetVignetteStrength(0.2f);
m_CRTShader->SetChromaticAberration(0.5f);
```

**Heavy CRT (very retro)**
```cpp
m_CRTShader->SetScanlineIntensity(0.15f);
m_CRTShader->SetCurvature(0.3f);
m_CRTShader->SetVignetteStrength(0.6f);
m_CRTShader->SetChromaticAberration(2.0f);
m_CRTShader->SetNoiseAmount(0.1f);
```

**No CRT (flat screen)**
```cpp
m_CRTShader->SetEnabled(false);
```

---

## Font Size Configuration

To change font size, edit `Engine.cpp`:

```cpp
// In Engine::Initialize()
if (!m_TextRenderer->Initialize("assets/fonts/monospace.ttf", 20)) {
    //                                                           ^^
    // Change this number (16 = small, 20 = medium, 24 = large)
```

---

## Files Added

Make sure these new files are in your project:

**Headers** (`include/rendering/`):
- `CRTShader.h`

**Source** (`src/rendering/`):
- `CRTShader.cpp`

**Modified Files**:
- `include/ui/Terminal.h` - Added typewriter functionality
- `src/ui/Terminal.cpp` - Implemented typewriter effect
- `include/systems/CommandParser.h` - Added new commands
- `src/systems/CommandParser.cpp` - Implemented color/crt/speed commands
- `include/core/Engine.h` - Added CRT shader member
- `src/core/Engine.cpp` - Integrated CRT rendering

---

## Troubleshooting

### CRT effect not appearing
- Make sure `m_CRTShader->Initialize()` succeeded
- Check that rendering happens between `BeginRender()` and `EndRender()`
- Verify shaders compiled (check console for errors)

### Typewriter effect too fast/slow
- Adjust speed with `m_Terminal->SetTypewriterSpeed(50.0f)`
- Or use different speeds per line in `AddLineWithTypewriter()`

### Text too small/large
- Change font size in `Engine::Initialize()`
- Adjust `LINE_HEIGHT` in `Terminal.cpp` if spacing is off

### Fullscreen issues
- Make sure `glfwGetPrimaryMonitor()` is called AFTER `glfwInit()`
- Check for null pointer before using monitor

---

## Performance Notes

- **CRT shader**: Minimal performance impact on modern GPUs (~1-2ms per frame)
- **Typewriter effect**: No performance impact, just animation logic
- **Font rendering**: Rendering 20pt vs 16pt has negligible impact

If you experience slowdown:
1. Disable CRT: `crt off`
2. Reduce shader complexity by lowering effect values
3. Check if font texture atlas is too large (rare)

---

## Future Enhancements

Ideas for extending these features:

1. **Save settings** - Store color/CRT preferences in a config file
2. **Color themes** - Predefined color schemes (hacker, retro, etc.)
3. **Per-line colors** - Different colors for errors, warnings, etc.
4. **Animation presets** - Save/load CRT effect combinations
5. **Sound on typing** - Keyboard click sounds with typewriter effect
6. **Custom shaders** - Load shader files from assets folder
7. **Glow effects** - Text glow to match CRT phosphor effect