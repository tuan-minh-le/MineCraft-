#include "entity/player.hpp"

Player::Player()
{

}

void Player::initialize(cgp::input_devices& inputs, cgp::window_structure& window){
    camera.initialize(inputs, window);
    camera.set_rotation_axis_y();
    set_hunger() = 20;
    speed = 1.0f;
    ind_inventory = 0;
    inventory.initialize(inventory_size);
    std::shared_ptr<Item> itemPtr = inventory.get_inventory()[ind_inventory];

    isGrounded = true;
    verticalVelocity = 0;
    gravity = 9.81f;
    dt = 0.01f;
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

void Player::handle_mouse_move(cgp::vec2 const& mouse_position_current, cgp::vec2 const& mouse_position_previous, cgp::mat4& camera_view_matrix) {
    
    if(cgp::norm(mouse_position_current - mouse_position_previous) < 0.01){
        //std::cout << "Ignoring input" << std::endl;
        return;
    }

    camera.action_mouse_move(camera_view_matrix);
    
    float& current_cam_pitch_rad = camera.camera_model.pitch;    
    
    float local_max_pitch_up_rad = max_pitch_up * cgp::Pi / 180.0f;
    float local_max_pitch_down_rad = max_pitch_down * cgp::Pi / 180.0f; 

    current_cam_pitch_rad = cgp::clamp(current_cam_pitch_rad, local_max_pitch_down_rad, local_max_pitch_up_rad);
    
    camera_view_matrix = camera.camera_model.matrix_view();
}

void Player::handle_keyboard_event(const cgp::inputs_keyboard_parameters& keyboard,cgp::mat4& camera_view_matrix){
    if (keyboard.is_pressed(GLFW_KEY_E) && inventory.get_opened_inventory()){
        inventory.close_inventory();
        inventory.set_opened_inventory() = false;               
    }
    else if (keyboard.is_pressed(GLFW_KEY_E))
    {
        inventory.open_inventory();
        inventory.set_opened_inventory() = true;
    }

    if (keyboard.is_pressed(GLFW_KEY_Q)){
        set_speed() = 0.05f;
    }

    if (keyboard.is_pressed(GLFW_KEY_LEFT_SHIFT)){
        set_speed() = 0.005f;
    }

    if (position.z <= 0) {
        position.z = 0;  
        isGrounded = true;    
        verticalVelocity = 0.0f; 
    }

    if (isGrounded) {
        verticalVelocity = 0.0f; 
        if (keyboard.is_pressed(GLFW_KEY_SPACE)) {
            verticalVelocity = 5.0f; 
            isGrounded = false;          
        }
    }
    else {
        verticalVelocity -= gravity * dt; 
    }

    
    position.y += verticalVelocity * dt;

    camera.camera_model.position_camera = position;
    camera_view_matrix = camera.camera_model.matrix_view();
}

void Player::move(float speed,const cgp::inputs_keyboard_parameters& keyboard,cgp::mat4& camera_view_matrix){

    static cgp::vec3 forward;
    static cgp::vec3 right;

    forward = camera.camera_model.front();
    right = camera.camera_model.right();
    
    if (cgp::norm(forward) > 0.01f) forward = cgp::normalize(forward);
    if (cgp::norm(right) > 0.01f) right = cgp::normalize(right);

    forward.y = 0;
    right.y = 0;

    if (keyboard.is_pressed(GLFW_KEY_W)){

        std::cout<< "speed" << speed<<std::endl;
        std::cout<< "position before" << position<<std::endl;

        position += speed * forward;
        std::cout<<"move z"<<std::endl;

        std::cout<< "position after" << position<<std::endl;
    }

    if (keyboard.is_pressed(GLFW_KEY_A)){
        position -= speed * right;
    }

    if (keyboard.is_pressed(GLFW_KEY_S)){
        position -= speed * forward; 
    }

    if (keyboard.is_pressed(GLFW_KEY_D)){
        position += speed * right;
    }
    camera.camera_model.position_camera = position;
    camera_view_matrix = camera.camera_model.matrix_view();

}

void Player::handle_mouse_event(const cgp::inputs_mouse_parameters& mouse){
    if (mouse.click.left){
        std::cout<<"bouton gauche"<<std::endl;
    }

    if (mouse.click.right){
        std::cout<<"bouton droit"<<std::endl;
    }

    if (mouse.scroll == 1){
        if (ind_inventory+1 >= inventory_size) {
            throw std::out_of_range("No next item");
        }
        else
        {
            std::shared_ptr<Item> itemPtr = inventory.get_inventory()[ind_inventory + 1];
        }
    }

    if (mouse.scroll == -1){
        if (ind_inventory-1 < 0) {
            throw std::out_of_range("No previous item");
        }
        else
        {
            std::shared_ptr<Item> itemPtr = inventory.get_inventory()[ind_inventory - 1];
        }
    }
}



// std::string Player::getNameObj()const{
//     return nameObj;
// }
// std::string& Player::setNameObj(){
//     return nameObj;
// }
// std::string Player::getNameTexture()const{
//     return nameTexture;
// }
// std::string& Player::setNameTexture(){
//     return nameTexture;
// }