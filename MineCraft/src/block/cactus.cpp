#include "cactus.hpp"

Cactus::Cactus() : Block() {
    item_name = "Cactus";
    type = BlockType::CACTUS;
}

Cactus::~Cactus() {}

void Cactus::initialize() {
    initialize_shared_mesh();
}

void Cactus::setup_texture() {

}

void Cactus::draw_block_at(const cgp::environment_generic_structure& environment) {

    initialize_shared_mesh();
    
    static cgp::mesh_drawable cactus_drawable;
    static bool drawable_initialized = false;
    
    if (!drawable_initialized) {
        cactus_drawable.initialize_data_on_gpu(shared_cube_mesh);
        cactus_drawable.texture.load_and_initialize_texture_2d_on_gpu("assets/cactus.png");
        drawable_initialized = true;
    }
    
    cactus_drawable.model.translation = position;
    cgp::draw(cactus_drawable, environment);
}