#include "systems/CommandParser.h"
#include "systems/FileSystem.h"
#include "ui/Terminal.h"
#include <sstream>
#include <algorithm>
#include <ctime>

CommandParser::CommandParser(FileSystem* fs, Terminal* terminal)
    : m_FileSystem(fs), m_Terminal(terminal) {
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