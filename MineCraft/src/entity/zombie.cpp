#include "zombie.hpp"

void Zombie::move(Player& player, float speed){
    cgp::vec3 diff = player.getPosition() - position; 
    diff.y=0;
    // std::cout<<cgp::norm(diff)<<std::endl;
    if (cgp::norm(diff) > 0.05f) diff = cgp::normalize(diff);
    else
    {
        player.setLife()-=1;
    }
    
    diff *=speed/10;

    position = position + diff;
    mesh_drawable.model.translation=position;
    position.y = getCooHeight(world);
}

void Zombie::initialize(const cgp::vec3& p_position, cgp::input_devices& inputs, cgp::window_structure& window, World* wrd){

    life = 20;

    filename_mesh_obj = "assets/Zombie/Zombie.obj";
    filename_mesh_texture = "assets/Zombie/zombie.png";
    
    position = p_position;

    speed = 0.8f;

    world = wrd;

    initialize_drawable_mesh();
    initialize_mesh();
    mesh_drawable.model.translation=position;
}