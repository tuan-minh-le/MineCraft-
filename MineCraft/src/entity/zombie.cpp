#include "zombie.hpp"

void Zombie::move(Player player, float speed){
    cgp::vec3 diff = player.getPosition() - position; 
    diff.y=0;
    if (cgp::norm(diff) > 0.01f) diff = cgp::normalize(diff);
    diff *=speed;

    position = position + diff;
    mesh_drawable.model.translation=position;
    //position.y = //fronction romain;
}

void Zombie::initialize(cgp::input_devices& inputs, cgp::window_structure& window, World* wrd){

    life = 20;

    filename_mesh_obj = "assets/Zombie/Zombie.obj";
    filename_mesh_texture = "assets/Zombie/zombie.png";
    
    position.x = 20.0f;
    position.y = 15.0f;
    position.z = 10.0f;

    speed = 0.8f;

    world = wrd;

    initialize_drawable_mesh();
    initialize_mesh();
    mesh_drawable.model.translation=position;
}