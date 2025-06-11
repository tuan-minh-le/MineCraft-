#pragma once
#include "block/interractive.hpp"

class Craft_table : public Interractive {
private:
    bool opened;

public:
    Craft_table();
    
    void initialize() override;
    void setup_texture() override;
    void draw_block_at(const cgp::environment_generic_structure& environment) override;
    
    void action(int* entier);
    bool get_opened() const;
    bool& set_opened();
};