#pragma once

#include "cgp/cgp.hpp"
#include "block/item.hpp" //Take care Forward declaration maybe
#include <vector>
#include <memory>
#include <string>

class Inventory{
    protected:
    
        bool opened_inventory = false;
        std::vector<std::shared_ptr<Item>> inventory;
        int inventory_size;

    public:

        Inventory();
        ~Inventory();

        void initialize(int size);
        void add_inventory (std::shared_ptr<Item> item, int ind);
        void erase_inventory (int ind);
        void switch_inventory(int ind1, int ind2);
        void open_inventory();
        void close_inventory();

        void draw_inventory();


        bool get_opened_inventory() const;
        bool& set_opened_inventory();

        std::vector<std::shared_ptr<Item>> get_inventory() const;
        std::vector<std::shared_ptr<Item>>& set_inventory();
};