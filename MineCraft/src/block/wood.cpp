#include "wood.hpp"

Wood::Wood() : Block() {
    item_name = "Wood";
    type = BlockType::WOOD;
}

Wood::~Wood() {}

void Wood::initialize() {
    initialize_shared_mesh();
}

void Wood::setup_texture() {

}

void Wood::draw_block_at(const cgp::environment_generic_structure& environment) {

    initialize_shared_mesh();
    
    static cgp::mesh_drawable wood_drawable;
    static bool drawable_initialized = false;
    
    if (!drawable_initialized) {
        wood_drawable.initialize_data_on_gpu(shared_cube_mesh);
        wood_drawable.texture.load_and_initialize_texture_2d_on_gpu("assets/wood.jpg");
        drawable_initialized = true;
    }
    
    wood_drawable.model.translation = position;
    cgp::draw(wood_drawable, environment);
}