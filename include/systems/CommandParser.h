#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <string>
#include <map>
#include <functional>
#include <vector>

class FileSystem;
class Terminal;

class CommandParser {
public:
    using CommandFunc = std::function<void(const std::vector<std::string>&)>;

    CommandParser(FileSystem* fs, Terminal* terminal);
    ~CommandParser();

    void Initialize();
    void ParseAndExecute(const std::string& input);
    void RegisterCommand(const std::string& name, CommandFunc func, const std::string& help);

private:
    FileSystem* m_FileSystem;
    Terminal* m_Terminal;
    
    struct CommandInfo {
        CommandFunc function;
        std::string helpText;
    };
    
    std::map<std::string, CommandInfo> m_Commands;

    std::vector<std::string> Tokenize(const std::string& input);
    
    // Built-in commands
    void CmdHelp(const std::vector<std::string>& args);
    void CmdClear(const std::vector<std::string>& args);
    void CmdLs(const std::vector<std::string>& args);
    void CmdRm(const std::vector<std::string>& args);
    void CmdCal(const std::vector<std::string>& args);
    void CmdNews(const std::vector<std::string>& args);
    void CmdRestart(const std::vector<std::string>& args);
    void CmdLogout(const std::vector<std::string>& args);
};

#endif // COMMANDPARSER_H
