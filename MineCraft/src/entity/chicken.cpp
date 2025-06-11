#include "chicken.hpp"

void Chicken::move(float speed,const cgp::inputs_keyboard_parameters& keyboard,cgp::mat4& camera_view_matrix){

}

void Chicken::initialize(cgp::input_devices& inputs, cgp::window_structure& window, World& wrd){

    life = 10;
    filename_mesh_obj = "assets/Chicken/Chicken.obj";
    filename_mesh_texture = "assets/Chicken/chicken.png";

    position.x = 10.0f;
    position.y = 15.0f;
    position.z = 10.0f;

    initialize_drawable_mesh();
    initialize_mesh();
    mesh_drawable.model.translation=position;

}