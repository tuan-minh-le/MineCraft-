#pragma once

#include "cgp/cgp.hpp"
#include <string>

class Invenory{
    protected:
        std::string item_name;
        cgp::mesh_drawable inventory_texture;
    public:
        std::string getItemName() const;
};