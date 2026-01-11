#include "FileSystem.h"
#include <iostream>

int main() {
    FileSystem fs;

    fs.createPath("/documents");
    fs.createPath("/documents/design");
    fs.createPath("/documents/design/requirements.txt");

    fs.writeFile("/documents/design/requirements.txt",
        "Low Level Design File System");

    fs.display();

    std::cout << "\nFile Content:\n";
    std::cout << fs.readFile("/documents/design/requirements.txt") << "\n";

    return 0;
}
