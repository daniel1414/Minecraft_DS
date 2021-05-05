#pragma once

#include "Cube.h"

#include <unordered_map>

class Inventory
{
public:
    Inventory() = default;

    void addItem(Cube* cube, uint8 count);
    void deleteItem(Cube* cube, uint8 count);

    Cube* selectedCube = nullptr;
private:
    std::unordered_map<Cube*, uint8> m_items;
};