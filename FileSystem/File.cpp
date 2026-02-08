#include "File.h"
#include <iostream>

File::File(const std::string& name)
    : FileSystemNode(name) {
}

void File::setContent(const std::string& data) {
    content = data;
    updateModifiedTime();
}

std::string File::getContent() const {
    return content;
}

bool File::isFile() const {
    return true;
}

void File::display(int depth) const {
    std::string indent(depth * 2, ' ');
    std::cout << indent << "# " << name << "\n";
}
