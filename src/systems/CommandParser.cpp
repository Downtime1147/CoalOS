#include "core/Engine.h"
#include "systems/CommandParser.h"
#include "systems/FileSystem.h"
#include "ui/Terminal.h"
#include "rendering/CRTShader.h"
#include <sstream>
#include <algorithm>
#include <ctime>

CommandParser::CommandParser(FileSystem* fs, Terminal* terminal)
    : m_FileSystem(fs), m_Terminal(terminal), m_CRTShader(nullptr), m_Engine(nullptr) {
}

CommandParser::~CommandParser() {
}

void CommandParser::Initialize() {
    // Register built-in commands
    RegisterCommand("help", [this](const auto& args) { CmdHelp(args); }, 
        "displays list of commands");
    RegisterCommand("clear", [this](const auto& args) { CmdClear(args); }, 
        "clears the screen");
    RegisterCommand("ls", [this](const auto& args) { CmdLs(args); }, 
        "display everything in your filesystem");
    RegisterCommand("rm", [this](const auto& args) { CmdRm(args); }, 
        "remove an item from the filesystem");
    RegisterCommand("cal", [this](const auto& args) { CmdCal(args); }, 
        "prints the current date and time");
    RegisterCommand("news", [this](const auto& args) { CmdNews(args); }, 
        "display the latest news reports");
    RegisterCommand("restart", [this](const auto& args) { CmdRestart(args); }, 
        "restarts CoalOS");
    RegisterCommand("logout", [this](const auto& args) { CmdLogout(args); }, 
        "exit coalOS");
    RegisterCommand("color", [this](const auto& args) { CmdColor(args); }, 
        "change terminal text color");
    RegisterCommand("crt", [this](const auto& args) { CmdCRT(args); }, 
        "toggle or adjust CRT effect");
    RegisterCommand("speed", [this](const auto& args) { CmdSpeed(args); }, 
        "adjust typewriter text speed");
    RegisterCommand("save", [this](const auto& args) { CmdSave(args); }, 
        "save current game state");
}

void CommandParser::ParseAndExecute(const std::string& input) {
    if (input.empty()) {
        return;
    }

    std::vector<std::string> tokens = Tokenize(input);
    if (tokens.empty()) {
        return;
    }

    std::string command = tokens[0];
    std::transform(command.begin(), command.end(), command.begin(), ::tolower);

    auto it = m_Commands.find(command);
    if (it != m_Commands.end()) {
        it->second.function(tokens);
    } else {
        m_Terminal->AddLine("Command invalid. Type 'help' for a list of commands ...");
        m_Terminal->AddLine("");
    }
}

void CommandParser::RegisterCommand(const std::string& name, CommandFunc func, const std::string& help) {
    m_Commands[name] = {func, help};
}

std::vector<std::string> CommandParser::Tokenize(const std::string& input) {
    std::vector<std::string> tokens;
    std::istringstream iss(input);
    std::string token;
    
    while (iss >> token) {
        tokens.push_back(token);
    }
    
    return tokens;
}

void CommandParser::CmdHelp(const std::vector<std::string>& args) {
    m_Terminal->AddLine("");
    m_Terminal->AddLine("List of commands:");
    m_Terminal->AddLine("");
    
    for (const auto& cmd : m_Commands) {
        std::string line = "     " + cmd.first;
        // Pad to align help text
        while (line.length() < 15) line += " ";
        line += " - " + cmd.second.helpText;
        m_Terminal->AddLine(line);
        m_Terminal->AddLine("");
    }
}

void CommandParser::CmdClear(const std::vector<std::string>& args) {
    m_Terminal->Clear();
}

void CommandParser::CmdLs(const std::vector<std::string>& args) {
    m_Terminal->AddLine("");
    std::vector<std::string> files = m_FileSystem->ListFiles();
    
    if (files.empty()) {
        m_Terminal->AddLine("Filesystem is empty");
    } else {
        std::string fileList = "[ ";
        for (size_t i = 0; i < files.size(); ++i) {
            fileList += files[i];
            if (i < files.size() - 1) {
                fileList += ", ";
            }
        }
        fileList += " ]";
        m_Terminal->AddLine(fileList);
    }
    m_Terminal->AddLine("");
}

void CommandParser::CmdSave(const std::vector<std::string>& args) {
    m_Terminal->AddLine("");
    
    if (!m_Engine) {
        m_Terminal->AddLine("Error: Save system not available");
        m_Terminal->AddLine("");
        return;
    }
    
    m_Terminal->AddLineWithTypewriter("Saving game state...", 40.0f);
    m_Engine->SaveGameData();
    m_Terminal->AddLine("Game saved successfully!");
    m_Terminal->AddLine("");
}

void CommandParser::CmdCRT(const std::vector<std::string>& args) {
    if (!m_CRTShader) {
        m_Terminal->AddLine("");
        m_Terminal->AddLine("Error: CRT shader not available");
        m_Terminal->AddLine("");
        return;
    }
    
    m_Terminal->AddLine("");
    
    if (args.size() < 2) {
        m_Terminal->AddLine("CRT Effect Controls:");
        m_Terminal->AddLine("");
        m_Terminal->AddLine("  crt on/off        - Toggle effect");
        m_Terminal->AddLine("  crt scanline <n>  - Scanline intensity (0.0-1.0)");
        m_Terminal->AddLine("  crt curve <n>     - Screen curvature (0.0-1.0)");
        m_Terminal->AddLine("  crt vignette <n>  - Vignette strength (0.0-1.0)");
        m_Terminal->AddLine("  crt glow <n>      - Glow intensity (0.0-1.0)");
        m_Terminal->AddLine("  crt noise <n>     - Noise amount (0.0-1.0)");
        m_Terminal->AddLine("  crt chroma <n>    - Chromatic aberration (0.0-2.0)");
        m_Terminal->AddLine("");
        m_Terminal->AddLine("Current: " + std::string(m_CRTShader->IsEnabled() ? "ON" : "OFF"));
        m_Terminal->AddLine("");
        return;
    }
    
    std::string option = args[1];
    std::transform(option.begin(), option.end(), option.begin(), ::tolower);
    
    if (option == "on") {
        m_CRTShader->SetEnabled(true);
        m_Terminal->AddLine("CRT effect enabled");
    }
    else if (option == "off") {
        m_CRTShader->SetEnabled(false);
        m_Terminal->AddLine("CRT effect disabled");
    }
    else if (args.size() >= 3) {
        try {
            float value = std::stof(args[2]);
            
            if (option == "scanline") {
                m_CRTShader->SetScanlineIntensity(value);
                m_Terminal->AddLine("Scanline intensity set to " + std::to_string(value));
            }
            else if (option == "curve") {
                m_CRTShader->SetCurvature(value);
                m_Terminal->AddLine("Screen curvature set to " + std::to_string(value));
            }
            else if (option == "vignette") {
                m_CRTShader->SetVignetteStrength(value);
                m_Terminal->AddLine("Vignette strength set to " + std::to_string(value));
            }
            else if (option == "glow") {
                m_CRTShader->SetGlowIntensity(value);
                m_Terminal->AddLine("Glow intensity set to " + std::to_string(value));
            }
            else if (option == "noise") {
                m_CRTShader->SetNoiseAmount(value);
                m_Terminal->AddLine("Noise amount set to " + std::to_string(value));
            }
            else if (option == "chroma") {
                m_CRTShader->SetChromaticAberration(value);
                m_Terminal->AddLine("Chromatic aberration set to " + std::to_string(value));
            }
            else {
                m_Terminal->AddLine("Unknown CRT option: " + option);
            }
        } catch (...) {
            m_Terminal->AddLine("Error: Invalid value");
        }
    }
    else {
        m_Terminal->AddLine("Usage: crt <option> <value>");
    }
    
    m_Terminal->AddLine("");
}

void CommandParser::CmdSpeed(const std::vector<std::string>& args) {
    m_Terminal->AddLine("");
    
    if (args.size() < 2) {
        m_Terminal->AddLine("Usage: speed <characters per second>");
        m_Terminal->AddLine("Examples:");
        m_Terminal->AddLine("  speed 20    - Slow typewriter");
        m_Terminal->AddLine("  speed 50    - Normal (default)");
        m_Terminal->AddLine("  speed 100   - Fast");
        m_Terminal->AddLine("  speed 1000  - Instant");
        m_Terminal->AddLine("");
        return;
    }
    
    try {
        float speed = std::stof(args[1]);
        if (speed < 1.0f) speed = 1.0f;
        if (speed > 10000.0f) speed = 10000.0f;
        
        m_Terminal->SetTypewriterSpeed(speed);
        m_Terminal->AddLine("Typewriter speed set to " + std::to_string(speed) + " chars/sec");
    } catch (...) {
        m_Terminal->AddLine("Error: Invalid speed value");
    }
    
    m_Terminal->AddLine("");
}

void CommandParser::CmdRm(const std::vector<std::string>& args) {
    m_Terminal->AddLine("");
    
    if (args.size() < 2) {
        m_Terminal->AddLine("Usage: rm <filename>");
        m_Terminal->AddLine("");
        return;
    }
    
    std::string filename = args[1];
    
    if (m_FileSystem->FileExists(filename)) {
        m_FileSystem->RemoveFile(filename);
        m_Terminal->AddLine("Removed: " + filename);
    } else {
        m_Terminal->AddLine(filename + " does not exist inside filesystem ...");
    }
    
    m_Terminal->AddLine("");
}

void CommandParser::CmdCal(const std::vector<std::string>& args) {
    m_Terminal->AddLine("");
    
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%H : %M     %d / %m / %Y", localTime);
    
    m_Terminal->AddLine(std::string(buffer));
    m_Terminal->AddLine("");
}

void CommandParser::CmdNews(const std::vector<std::string>& args) {
    // Simple news headlines - you can expand this later
    static const char* headlines[] = {
        "Breaking: Quantum computer breaks RSA encryption",
        "Cybersecurity firm reports major data breach",
        "New AI algorithm predicts network vulnerabilities",
        "Hackers target critical infrastructure worldwide",
        "Government announces new cybersecurity initiative"
    };
    
    m_Terminal->AddLine("");
    int index = std::rand() % 5;
    m_Terminal->AddLine(headlines[index]);
    m_Terminal->AddLine("");
}

void CommandParser::CmdRestart(const std::vector<std::string>& args) {
    m_Terminal->Clear();
    m_Terminal->AddLine("Restarting CoalOS...");
    m_Terminal->AddLine("");
    // TODO: Trigger actual restart through Engine
}

void CommandParser::CmdLogout(const std::vector<std::string>& args) {
    m_Terminal->AddLine("");
    m_Terminal->AddLine("Goodbye...");
    m_Terminal->AddLine("");
    // TODO: Trigger exit through Engine
}

void CommandParser::CmdColor(const std::vector<std::string>& args) {
    m_Terminal->AddLine("");
    
    if (args.size() < 2) {
        m_Terminal->AddLine("Usage: color <preset|rgb>");
        m_Terminal->AddLine("");
        m_Terminal->AddLine("Presets:");
        m_Terminal->AddLine("  green    - Classic green terminal");
        m_Terminal->AddLine("  amber    - Amber monochrome");
        m_Terminal->AddLine("  white    - White text");
        m_Terminal->AddLine("  cyan     - Cyan blue");
        m_Terminal->AddLine("  red      - Red text");
        m_Terminal->AddLine("  purple   - Purple/magenta");
        m_Terminal->AddLine("");
        m_Terminal->AddLine("RGB: color rgb <r> <g> <b>");
        m_Terminal->AddLine("  Values from 0.0 to 1.0");
        m_Terminal->AddLine("  Example: color rgb 1.0 0.5 0.0");
        m_Terminal->AddLine("");
        return;
    }
    
    std::string colorChoice = args[1];
    std::transform(colorChoice.begin(), colorChoice.end(), colorChoice.begin(), ::tolower);
    
    if (colorChoice == "rgb" || colorChoice == "custom") {
        if (args.size() < 5) {
            m_Terminal->AddLine("Error: RGB requires 3 values");
            m_Terminal->AddLine("Usage: color rgb <r> <g> <b>");
            m_Terminal->AddLine("Values from 0.0 to 1.0");
            m_Terminal->AddLine("");
            return;
        }
        
        try {
            float r = std::stof(args[2]);
            float g = std::stof(args[3]);
            float b = std::stof(args[4]);
            
            // Clamp values
            r = std::max(0.0f, std::min(1.0f, r));
            g = std::max(0.0f, std::min(1.0f, g));
            b = std::max(0.0f, std::min(1.0f, b));
            
            m_Terminal->SetTextColor(r, g, b);
            m_Terminal->AddLine("Color set to RGB(" + 
                std::to_string(r) + ", " + 
                std::to_string(g) + ", " + 
                std::to_string(b) + ")");
        } catch (...) {
            m_Terminal->AddLine("Error: Invalid RGB values");
        }
    }
    else if (colorChoice == "green") {
        m_Terminal->SetTextColor(0.0f, 1.0f, 0.0f);
        m_Terminal->AddLine("Color set to green");
    }
    else if (colorChoice == "amber" || colorChoice == "orange") {
        m_Terminal->SetTextColor(1.0f, 0.75f, 0.0f);
        m_Terminal->AddLine("Color set to amber");
    }
    else if (colorChoice == "white") {
        m_Terminal->SetTextColor(1.0f, 1.0f, 1.0f);
        m_Terminal->AddLine("Color set to white");
    }
    else if (colorChoice == "cyan" || colorChoice == "blue") {
        m_Terminal->SetTextColor(0.0f, 1.0f, 1.0f);
        m_Terminal->AddLine("Color set to cyan");
    }
    else if (colorChoice == "red") {
        m_Terminal->SetTextColor(1.0f, 0.0f, 0.0f);
        m_Terminal->AddLine("Color set to red");
    }
    else if (colorChoice == "purple" || colorChoice == "magenta") {
        m_Terminal->SetTextColor(1.0f, 0.0f, 1.0f);
        m_Terminal->AddLine("Color set to purple");
    }
    else {
        m_Terminal->AddLine("Unknown color preset: " + colorChoice);
        m_Terminal->AddLine("Type 'color' for help");
    }
    
    m_Terminal->AddLine("");
}