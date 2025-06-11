#include "snow.hpp"

Snow::Snow() : Block() {
    item_name = "Snow";
    type = BlockType::SNOW;
}

Snow::~Snow() {}

void Snow::initialize() {
    initialize_shared_mesh();
}

void Snow::setup_texture() {

}


void Snow::draw_block_at(const cgp::environment_generic_structure& environment) {

    initialize_shared_mesh();
    

    static cgp::mesh_drawable snow_drawable;
    static bool drawable_initialized = false;
    
    if (!drawable_initialized) {
        snow_drawable.initialize_data_on_gpu(shared_cube_mesh);
        snow_drawable.material.color = cgp::vec3{1.0f, 1.0f, 1.0f}; 
        drawable_initialized = true;
    }
    
    snow_drawable.model.translation = position;
    cgp::draw(snow_drawable, environment);
}