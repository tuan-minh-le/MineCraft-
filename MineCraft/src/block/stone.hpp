#pragma once
#include "block.hpp"



class Stone : public Block {
public:
    Stone();
    ~Stone();
    
    void initialize() override;
    void setup_texture() override;

    void draw_block_at(const cgp::vec3& position, const cgp::environment_generic_structure& environment) override;
};