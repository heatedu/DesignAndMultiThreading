#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <chrono>

class FileSystemNode {
protected:
    std::string name;
    std::unordered_map<std::string, std::shared_ptr<FileSystemNode>> children;
    std::chrono::system_clock::time_point createdAt;
    std::chrono::system_clock::time_point modifiedAt;

    void updateModifiedTime();

public:
    explicit FileSystemNode(const std::string& name);
    virtual ~FileSystemNode() = default;

    const std::string& getName() const;
    bool hasChild(const std::string& name) const;
    std::shared_ptr<FileSystemNode> getChild(const std::string& name) const;
    void addChild(const std::string& name,
        std::shared_ptr<FileSystemNode> child);
    bool removeChild(const std::string& name);

    std::vector<std::shared_ptr<FileSystemNode>> getChildren() const;

    virtual bool isFile() const = 0;
    virtual void display(int depth) const = 0;
};
