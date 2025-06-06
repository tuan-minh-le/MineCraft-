#pragma once
#include "block.hpp"



class Grass : public Block {
public:
    Grass();
    ~Grass();
    
    void initialize() override;
    void setup_texture() override;

    void draw_block_at(const cgp::environment_generic_structure& environment) override;
};