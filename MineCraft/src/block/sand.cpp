#include "sand.hpp"

Sand::Sand() : Block() {}

Sand::~Sand() {}

void Sand::initialize() {
    item_name = "Sand Block";
    type = BlockType::SAND;
    initialize_shared_mesh();
}

void Sand::setup_texture() {

}

void Sand::draw_block_at(const cgp::environment_generic_structure& environment) {

    initialize_shared_mesh();
    

    static cgp::mesh_drawable sand_drawable;
    static bool drawable_initialized = false;
    
    if (!drawable_initialized) {
        sand_drawable.initialize_data_on_gpu(shared_cube_mesh);
        sand_drawable.material.color = cgp::vec3{0.9f, 0.9f, 0.1f};
        drawable_initialized = true;
    }
    
    sand_drawable.model.translation = position;
    cgp::draw(sand_drawable, environment);
}