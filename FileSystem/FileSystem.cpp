#include "FileSystem.h"
#include <sstream>

FileSystem::FileSystem() {
    root = std::make_shared<Directory>("/");
}

bool FileSystem::isValidPath(const std::string& path) const {
    return !path.empty() && path[0] == '/';
}

bool FileSystem::createPath(const std::string& path) {
    if (!isValidPath(path)) return false;

    std::stringstream ss(path);
    std::string part;
    std::vector<std::string> components;

    while (getline(ss, part, '/')) {
        if (!part.empty()) components.push_back(part);
    }

    std::shared_ptr<FileSystemNode> current = root;

    for (size_t i = 0; i + 1 < components.size(); i++) {
        if (!current->hasChild(components[i])) {
            current->addChild(components[i],
                std::make_shared<Directory>(components[i]));
        }
        current = current->getChild(components[i]);
        if (current->isFile()) return false;
    }

    std::string last = components.back();
    if (current->hasChild(last)) return false;

    std::shared_ptr<FileSystemNode> node =
        (last.find('.') != std::string::npos)
        ? std::static_pointer_cast<FileSystemNode>(std::make_shared<File>(last))
        : std::static_pointer_cast<FileSystemNode>(std::make_shared<Directory>(last));

    current->addChild(last, node);
    return true;
}

std::shared_ptr<FileSystemNode>
FileSystem::getNode(const std::string& path) const {
    if (!isValidPath(path)) return nullptr;
    if (path == "/") return root;

    std::stringstream ss(path);
    std::string part;
    std::shared_ptr<FileSystemNode> current = root;

    while (getline(ss, part, '/')) {
        if (part.empty()) continue;
        if (!current->hasChild(part)) return nullptr;
        current = current->getChild(part);
    }
    return current;
}

bool FileSystem::deletePath(const std::string& path) {
    if (!isValidPath(path) || path == "/") return false;

    std::string parentPath = getParentPath(path);
    auto parent = getNode(parentPath);
    if (!parent || parent->isFile()) return false;

    std::string name = path.substr(path.find_last_of('/') + 1);
    return parent->removeChild(name);
}

std::string FileSystem::getParentPath(const std::string& path) const {
    auto pos = path.find_last_of('/');
    return (pos <= 0) ? "/" : path.substr(0, pos);
}

bool FileSystem::writeFile(const std::string& path,
    const std::string& content) {
    auto node = getNode(path);
    if (!node || !node->isFile()) return false;

    std::static_pointer_cast<File>(node)->setContent(content);
    return true;
}

std::string FileSystem::readFile(const std::string& path) const {
    auto node = getNode(path);
    if (!node || !node->isFile()) return "";
    return std::static_pointer_cast<File>(node)->getContent();
}

void FileSystem::display() const {
    root->display(0);
}
