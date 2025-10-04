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

## Building the Project

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


## Current Features

✅ Basic OpenGL window  
✅ Text rendering with FreeType  
✅ Scrolling terminal display  
✅ Command input system  
✅ Boot sequence animation  
✅ Basic commands: help, clear, ls, rm, cal, news
✅ Virtual filesystem
✅ Togglable CRT Shaders and Typewriting effect

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