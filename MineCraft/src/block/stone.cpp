#include "stone.hpp"

Stone::Stone() : Block() {
    item_name = "Stone";
    type = BlockType::STONE;
}

Stone::~Stone() {}

void Stone::initialize() {
    item_name = "Stone";
    type = BlockType::STONE;
    initialize_shared_mesh();
}

void Stone::setup_texture() {

}

void Stone::draw_block_at(const cgp::environment_generic_structure& environment) {

    initialize_shared_mesh();
    

    static cgp::mesh_drawable stone_drawable;
    static bool drawable_initialized = false;
    
    if (!drawable_initialized) {
        stone_drawable.initialize_data_on_gpu(shared_cube_mesh);
        stone_drawable.material.color = cgp::vec3{0.1f, 0.1f, 0.1f};
        drawable_initialized = true;
    }
    
    stone_drawable.model.translation = position;
    cgp::draw(stone_drawable, environment);
}