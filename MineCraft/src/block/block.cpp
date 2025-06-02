#include "block.hpp"

cgp::mesh Block::shared_cube_mesh;
bool Block::mesh_initialized = false;

Block::Block() {}

void Block::initialize_shared_mesh() {
    if (!mesh_initialized) {
        shared_cube_mesh = cgp::mesh_primitive_cube({0,0,0}, 1.0f);
        mesh_initialized = true;
    }
}

void Block::draw_cube_at(const cgp::vec3& position, const cgp::environment_generic_structure& environment) {
    initialize_shared_mesh();
    
    static cgp::mesh_drawable cube_drawable;
    static bool drawable_initialized = false;
    
    if (!drawable_initialized) {
        cube_drawable.initialize_data_on_gpu(shared_cube_mesh);
        drawable_initialized = true;
    }
    
    cube_drawable.model.translation = position;
    cgp::draw(cube_drawable, environment);
}

int Block::getDimension() const {
    return dimension;
}