#include "Inventory.h"

void Inventory::addItem(Cube* cube, uint8 count)
{
    if(cube != nullptr)
    {
        if((u16)m_items[cube] + count > 255)
            m_items[cube] = 255;
        else 
            m_items[cube] += count;
    }
    selectedCube = cube;
}

void Inventory::deleteItem(Cube* cube, uint8 count)
{
    if(cube != nullptr)
    {
        if((int)m_items[cube] - count < 0)
            m_items[cube] = 0;
        else 
            m_items[cube] -= count;
    }
}