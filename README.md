# CoalOS C++ - Setup Instructions

## Prerequisites

### Windows
- Visual Studio 2019+ (with C++ Desktop Development)
- CMake 3.15+
- Git

### Linux
```bash
sudo apt-get install build-essential cmake git libgl1-mesa-dev libfreetype6-dev
```

### macOS
```bash
brew install cmake freetype
```

## Step 1: Get GLAD

1. Go to https://glad.dav1d.de/
2. Set:
   - Language: **C/C++**
   - Specification: **OpenGL**
   - API gl: **Version 3.3** (or higher)
   - Profile: **Core**
3. Click **Generate**
4. Download the zip file
5. Extract and place files:
   - `include/glad/glad.h` → your project's `include/glad/glad.h`
   - `include/KHR/khrplatform.h` → your project's `include/KHR/khrplatform.h`
   - `src/glad.c` → your project's `src/glad.c` (NOT in include!)

## Step 2: Get a Monospace Font

You need a `.ttf` monospace font file. Some good options:

- **DejaVu Sans Mono** (free): https://dejavu-fonts.github.io/
- **Consolas** (Windows)
- **Courier New** (built-in on most systems)
- **JetBrains Mono** (free): https://www.jetbrains.com/lp/mono/

Place your font file at: `assets/fonts/monospace.ttf`

## Step 3: Build the Project

### Windows (Visual Studio)
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### Linux/macOS
```bash
mkdir build
cd build
cmake ..
make -j4
```

## Step 4: Run

```bash
cd build
./CoalOS
```

## Project Structure

```
CoalOS-CPP/
├── CMakeLists.txt
├── README.md
├── assets/
│   └── fonts/
│       └── monospace.ttf         # Add your font here!
├── build/                         # Build output (git ignored)
├── include/
│   ├── glad/
│   │   ├── glad.h                # Add from GLAD generator
│   │   └── glad.c                # Add from GLAD generator
│   ├── KHR/
│   │   └── khrplatform.h         # Add from GLAD generator
│   ├── core/
│   │   ├── Engine.h
│   │   └── GameState.h
│   ├── rendering/
│   │   └── TextRenderer.h
│   ├── systems/
│   │   ├── CommandParser.h
│   │   └── FileSystem.h
|   ├──tools/
|   |
│   └── ui/
│       └── Terminal.h
└── src/
    ├── main.cpp
    ├── core/
    │   ├── Engine.cpp
    │   └── GameState.cpp
    ├── rendering/
    │   └── TextRenderer.cpp
    ├── systems/
    │   ├── CommandParser.cpp
    │   └── FileSystem.cpp
    └── ui/
        └── Terminal.cpp
```

## Current Features

✅ Basic OpenGL window  
✅ Text rendering with FreeType  
✅ Scrolling terminal display  
✅ Command input system  
✅ Boot sequence animation  
✅ Basic commands: help, clear, ls, rm, cal, news  
✅ Virtual filesystem  

## Next Steps

1. **Add more commands**: iwlist, ssh, aireplay, aircrack
2. **Add tools**: SScrack, FTPea, MetaLib with their sub-menus
3. **Add audio**: Integrate miniaudio for sound effects
4. **Add visual effects**: CRT shader, scanlines, glitch effects
5. **Add progress bars**: For cracking/scanning animations
6. **Add networking system**: Device connections and hacking tools
7. **Save/load system**: Persistent game state

## Troubleshooting

### Font not loading
- Make sure the font path is correct: `assets/fonts/monospace.ttf`
- Check if the font file is a valid `.ttf` file
- Try using an absolute path for testing

### Linker errors with FreeType
- Windows: CMake should download FreeType automatically
- Linux: Install libfreetype6-dev: `sudo apt-get install libfreetype6-dev`

### GLAD errors
- Make sure you generated GLAD with OpenGL 3.3+ and Core profile
- Check that all three files are in the correct locations

## Controls

- **Type**: Regular keyboard input
- **Enter**: Submit command
- **Backspace**: Delete character
- (More controls to be added)

## Development Tips

1. Start by testing the basic window and font rendering
2. Add one command at a time and test thoroughly
3. Use the modular structure - each tool should be its own class
4. Keep the original Python version open for reference
5. Add features incrementally - don't try to do everything at once