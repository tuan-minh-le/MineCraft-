#pragma once
#include "block.hpp"



class Grass : public Block {
public:
    Grass();
    ~Grass();
    
    void initialize() override;
    void setup_texture() override;

    static void draw_grass_at(const cgp::vec3& position, const cgp::environment_generic_structure& environment);
};