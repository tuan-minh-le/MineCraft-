#pragma once

#include "cgp/cgp.hpp"
#include "block/item.hpp" //Take care Forward declaration maybe
#include "block/grass.hpp"
#include "block/stone.hpp"
#include "block/raw_chicken.hpp"
#include <vector>
#include <memory>
#include <string>

class Craft_table;

enum class SlotType {
    INVENTORY,
    CRAFT,
    TABLE_CRAFT
};

struct DragPayload {
    int index;
    SlotType source;
};

class Inventory{
    protected:
    
        bool opened_inventory = false;
        std::vector<std::vector<std::shared_ptr<Item>>> inventory;
        int inventory_size;
        bool little;

    public:

        Inventory();
        ~Inventory();

        int max_size = 64;

        void initialize(int size,bool add_craft,bool liitle);
        bool add_inventory (std::shared_ptr<Item> item);
        bool erase_inventory (int ind);
        void switch_inventory(int ind1, int ind2, SlotType from_type, SlotType to_type, Inventory& from_inv,bool split, bool one);
        void open_inventory();
        void close_inventory();

        void fast_switch_inventory(int ind,SlotType from_type, Inventory& inv);
        bool check_craft();
		void finish_craft(bool erase);


        bool get_opened_inventory() const;
        bool& set_opened_inventory();

        int get_inventory_size() const;
        int& set_inventory_size();

        std::vector<std::vector<std::shared_ptr<Item>>>& get_inventory();
        std::vector<std::vector<std::shared_ptr<Item>>>& set_inventory();
};