#include "systems/FileSystem.h"
#include <algorithm>

FileSystem::FileSystem() {
}

FileSystem::~FileSystem() {
}

void FileSystem::AddFile(const std::string& filename) {
    m_Files.insert(filename);
}

void FileSystem::RemoveFile(const std::string& filename) {
    m_Files.erase(filename);
}

bool FileSystem::FileExists(const std::string& filename) const {
    return m_Files.find(filename) != m_Files.end();
}

std::vector<std::string> FileSystem::ListFiles() const {
    return std::vector<std::string>(m_Files.begin(), m_Files.end());
}

void FileSystem::Clear() {
    m_Files.clear();
}
