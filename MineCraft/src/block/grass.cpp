#include "grass.hpp"

Grass::Grass() : Block() {
    item_name = "Grass";


}

Grass::~Grass() {}

void Grass::initialize() {
    initialize_shared_mesh();
}

void Grass::setup_texture() {

}


void Grass::draw_block_at(const cgp::environment_generic_structure& environment) {

    initialize_shared_mesh();
    

    static cgp::mesh_drawable grass_drawable;
    static bool drawable_initialized = false;
    
    if (!drawable_initialized) {
        grass_drawable.initialize_data_on_gpu(shared_cube_mesh);
        grass_drawable.material.color = cgp::vec3{0.2f, 0.8f, 0.2f}; // Nice green color
        drawable_initialized = true;
    }
    
    grass_drawable.model.translation = position;
    cgp::draw(grass_drawable, environment);
}