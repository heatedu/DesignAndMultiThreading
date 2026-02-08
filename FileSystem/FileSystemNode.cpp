#include "FileSystemNode.h"

FileSystemNode::FileSystemNode(const std::string& name)
    : name(name),
    createdAt(std::chrono::system_clock::now()),
    modifiedAt(createdAt) {
}

void FileSystemNode::updateModifiedTime() {
    modifiedAt = std::chrono::system_clock::now();
}

const std::string& FileSystemNode::getName() const {
    return name;
}

bool FileSystemNode::hasChild(const std::string& name) const {
    return children.count(name) > 0;
}

std::shared_ptr<FileSystemNode>
FileSystemNode::getChild(const std::string& name) const {
    auto it = children.find(name);
    return it != children.end() ? it->second : nullptr;
}

void FileSystemNode::addChild(const std::string& name,
    std::shared_ptr<FileSystemNode> child) {
    children[name] = child;
    updateModifiedTime();
}

bool FileSystemNode::removeChild(const std::string& name) {
    return children.erase(name) > 0;
}

std::vector<std::shared_ptr<FileSystemNode>>
FileSystemNode::getChildren() const {
    std::vector<std::shared_ptr<FileSystemNode>> result;
    for (auto& pair : children) {
        result.push_back(pair.second);
    }
    return result;
}
