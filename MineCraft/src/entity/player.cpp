#include "entity/player.hpp"

Player::Player() : hunger(20)
{

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

float Player::get_speed() const{
    return speed;
}

float& Player::set_speed(){
    return speed;
}

cgp::camera_controller_first_person_euler Player::get_camera() const{
    return camera;
}

cgp::camera_controller_first_person_euler& Player::set_camera(){
    return camera;
}

void Player::add_inventory (std::shared_ptr<Item> item, int ind){
    if (ind > inventory_size) {
        throw std::out_of_range("Index out of range");
    }
    if (ind == inventory_size) {
        inventory.push_back(item);
    } else {
        inventory.insert(inventory.begin() + ind, item);
    }
}

void Player::erase_inventory (int ind){
    if (ind >= inventory_size) {
        throw std::out_of_range("Index out of range");
    }
    inventory.erase(inventory.begin() + ind);
}

void Player::switch_inventory(int ind1, int ind2){
    if (ind1 >= inventory_size || ind2 >= inventory_size) {
        throw std::out_of_range("Index out of range");
    }
    std::swap(inventory[ind1], inventory[ind2]);
}

std::vector<std::shared_ptr<Item>> Player::get_inventory() const{
    return inventory;
}

std::vector<std::shared_ptr<Item>>& Player::set_inventory(){
    return inventory;
}

bool Player::get_opened_inventory() const{
    return opened_inventory;
}

bool& Player::set_opened_inventory(){
    return opened_inventory;
}

void Player::open_inventory(){

}

void Player::close_inventory(){

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

void Player::handle_keyboard_event(const cgp::inputs_keyboard_parameters& keyboard){
    if (keyboard.is_pressed(GLFW_KEY_Z)){
        position[0] += get_speed() * 0.5; 
    }

    if (keyboard.is_pressed(GLFW_KEY_Q)){
        position[1] -= get_speed() * 0.5; 
    }

    if (keyboard.is_pressed(GLFW_KEY_S)){
        position[0] -= get_speed() * 0.5; 
    }

    if (keyboard.is_pressed(GLFW_KEY_D)){
        position[1] += get_speed() * 0.5; 
    }

    if (keyboard.is_pressed(GLFW_KEY_E) && opened_inventory){
        close_inventory();               
    }
    else if (keyboard.is_pressed(GLFW_KEY_E))
    {
        open_inventory();
    }

    if (keyboard.is_pressed(GLFW_KEY_A)){
        set_speed() = 2.0f;
    }

    if (keyboard.is_pressed(GLFW_KEY_LEFT_SHIFT)){
        set_speed() = 0.5f;
    }
    
    if (keyboard.is_pressed(GLFW_KEY_SPACE)){
        
    }
}