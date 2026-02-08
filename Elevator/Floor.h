#pragma once

// Represents a floor in a building
class Floor {
private:
    int floorNumber;

public:
    Floor(int floorNumber);
    
    int getFloorNumber() const { return floorNumber; }
};

