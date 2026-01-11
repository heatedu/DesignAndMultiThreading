#pragma once
#include "FileSystemNode.h"

class File : public FileSystemNode {
private:
    std::string content;

public:
    explicit File(const std::string& name);

    void setContent(const std::string& data);
    std::string getContent() const;

    bool isFile() const override;
    void display(int depth) const override;
};
