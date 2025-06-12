#include "leaf.hpp"

Leaf::Leaf() : Block() {
    item_name = "Leaf";
    type = BlockType::LEAF;
}

Leaf::~Leaf() {}

void Leaf::initialize() {
    initialize_shared_mesh();
}

void Leaf::setup_texture() {

}

void Leaf::draw_block_at(const cgp::environment_generic_structure& environment) {

    initialize_shared_mesh();
    
    static cgp::mesh_drawable leaf_drawable;
    static bool drawable_initialized = false;
    
    if (!drawable_initialized) {
        leaf_drawable.initialize_data_on_gpu(shared_cube_mesh);
        leaf_drawable.texture.load_and_initialize_texture_2d_on_gpu("assets/leaf.png");
        drawable_initialized = true;
    }
    
    leaf_drawable.model.translation = position;
    cgp::draw(leaf_drawable, environment);
}