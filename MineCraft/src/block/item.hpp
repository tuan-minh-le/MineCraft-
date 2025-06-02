#pragma once

#include "cgp/cgp.hpp"
#include <string>

class Item{
    protected:
        std::string item_name;
        cgp::mesh_drawable inventory_texture;
    public:
        std::string getItemName() const;
};