#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <vector>
#include <set>

class FileSystem {
public:
    FileSystem();
    ~FileSystem();

    void AddFile(const std::string& filename);
    void RemoveFile(const std::string& filename);
    bool FileExists(const std::string& filename) const;
    std::vector<std::string> ListFiles() const;
    void Clear();

private:
    std::set<std::string> m_Files;
};

#endif // FILESYSTEM_H
