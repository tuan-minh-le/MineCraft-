#pragma once
#include "block.hpp"

class Cactus : public Block {
public:
    Cactus();
    ~Cactus();
    
    void initialize() override;
    void setup_texture() override;

    void draw_block_at(const cgp::environment_generic_structure& environment) override;
};