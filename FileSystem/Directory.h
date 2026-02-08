#pragma once
#include "FileSystemNode.h"

class Directory : public FileSystemNode {
public:
    explicit Directory(const std::string& name);

    bool isFile() const override;
    void display(int depth) const override;
};
