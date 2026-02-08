#pragma once
#include "Directory.h"
#include "File.h"

class FileSystem {
private:
    std::shared_ptr<FileSystemNode> root;

    bool isValidPath(const std::string& path) const;
    std::shared_ptr<FileSystemNode> getNode(const std::string& path) const;
    std::string getParentPath(const std::string& path) const;

public:
    FileSystem();

    bool createPath(const std::string& path);
    bool deletePath(const std::string& path);

    bool writeFile(const std::string& path, const std::string& content);
    std::string readFile(const std::string& path) const;

    void display() const;
};
