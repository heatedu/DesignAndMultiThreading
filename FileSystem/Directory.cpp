#include "Directory.h"
#include <iostream>

Directory::Directory(const std::string& name)
    : FileSystemNode(name) {
}

bool Directory::isFile() const {
    return false;
}

void Directory::display(int depth) const {
    std::string indent(depth * 2, ' ');
    std::cout << indent << "## " << name
        << " (" << children.size() << " items)\n";

    for (const auto& child : getChildren()) {
        child->display(depth + 1);
    }
}
