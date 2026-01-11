#pragma once
#include <string>
#include <vector>
#include <memory>

class Screen;

class Theatre {
private:
    int id;
    std::string name;
    std::vector<std::shared_ptr<Screen>> screens;

public:
    Theatre(int theatreId, const std::string& theatreName);
    
    void addScreen(std::shared_ptr<Screen> screen);
    int getTheatreId() const;
    std::vector<std::shared_ptr<Screen>> getScreens() const;
};

