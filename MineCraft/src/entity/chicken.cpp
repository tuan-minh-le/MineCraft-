#include "chicken.hpp"
#include <random>

void Chicken::move(Player& player, float speed){
    int rand_int = rand()%2;
    cgp::vec3 diff ={0,0,0};
    if (rand_int)
    {
        diff.x = 1;
    }
    else
    {
        diff.z=1;
    }
    if (cgp::norm(diff) > 0.01f) diff = cgp::normalize(diff);
    diff*=speed;

    position = position + diff;
    mesh_drawable.model.translation=position;
    //position.y = //fronction romain;
}

void Chicken::initialize(cgp::input_devices& inputs, cgp::window_structure& window, World* wrd){

    life = 10;

    filename_mesh_obj = "assets/Chicken/Chicken.obj";
    filename_mesh_texture = "assets/Chicken/chicken.png";

    position.x = 10.0f;
    position.y = 15.0f;
    position.z = 10.0f;
    
    speed = 0.5f;
    world = wrd;

    initialize_drawable_mesh();
    initialize_mesh();
    mesh_drawable.model.translation=position;

}