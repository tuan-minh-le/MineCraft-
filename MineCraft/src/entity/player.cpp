#include "entity/player.hpp"

Player::Player() : hunger(20)
{
    for (int i = 0; i < inventory_size; i++)
    {
        inventory.push_back(std::make_shared<Grass>());
    }
}

void Player::initialize(cgp::input_devices& inputs, cgp::window_structure& window){
    camera.initialize(inputs, window);
    camera.set_rotation_axis_y();
}

Player::~Player()
{

}

int Player::get_hunger() const{
    return hunger;
}

int& Player::set_hunger(){
    return hunger;
}

cgp::camera_controller_first_person_euler Player::get_camera() const{
    return camera;
}

cgp::camera_controller_first_person_euler& Player::set_camera(){
    return camera;
}

void Player::add_inventory (Item &item, int ind){

}

void Player::erase_inventory (int ind){

}

void Player::switch_inventory(Item &item1, Item &item2){

}

std::vector<std::shared_ptr<Item>> Player::get_inventory() const{
    return inventory;
}

std::vector<std::shared_ptr<Item>>& Player::set_inventory(){
    return inventory;
}

void Player::handle_mouse_move(cgp::vec2 const& mouse_position_current, cgp::vec2 const& mouse_position_previous, cgp::mat4& camera_view_matrix) {
    
    if(cgp::norm(mouse_position_current - mouse_position_previous) < 0.01){
        std::cout << "Ignoring input" << std::endl;
        return;
    }

    camera.action_mouse_move(camera_view_matrix);
    
    float& current_cam_pitch_rad = camera.camera_model.pitch;    
    
    float local_max_pitch_up_rad = max_pitch_up * cgp::Pi / 180.0f;
    float local_max_pitch_down_rad = max_pitch_down * cgp::Pi / 180.0f; 

    current_cam_pitch_rad = cgp::clamp(current_cam_pitch_rad, local_max_pitch_down_rad, local_max_pitch_up_rad);
    
    camera_view_matrix = camera.camera_model.matrix_view();
}