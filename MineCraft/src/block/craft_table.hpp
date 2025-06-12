#pragma once
#include "block/interractive.hpp"
#include "inventory/inventory.hpp"

class Craft_table : public Interractive {
private:
    bool opened;
    Inventory craft_inventory;

public:
    Craft_table();
    
    void initialize() override;
    void setup_texture() override;
    void draw_block_at(const cgp::environment_generic_structure& environment) override;
    
    void action();
    bool get_opened() const;
    bool& set_opened();
};