#include "mob.hpp"

void Mob::initialize(){

}

void Mob::initialize_mesh(){
    if (!mesh_initialized) {
        mesh = cgp::mesh_load_file_obj(filename_mesh_obj);
        mesh_initialized = true;
    }
}

void Mob::initialize_drawable_mesh(){

    if (!mesh_drawable_initialized) {
        initialize_mesh();
        
        // Initialize grass drawable
        mesh_drawable.initialize_data_on_gpu(mesh);
        mesh_drawable.texture.load_and_initialize_texture_2d_on_gpu(filename_mesh_texture);
        
        mesh_drawable_initialized = true;
    }

}


cgp::mesh Mob::get_mesh() const{
    return mesh;
}

cgp::mesh_drawable Mob::get_mesh_drawable() const{
    return mesh_drawable;
}

