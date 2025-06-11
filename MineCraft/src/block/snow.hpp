#pragma once
#include "block.hpp"



class Snow : public Block {
public:
    Snow();
    ~Snow();
    
    void initialize() override;
    void setup_texture() override;

    void draw_block_at(const cgp::environment_generic_structure& environment) override;
};