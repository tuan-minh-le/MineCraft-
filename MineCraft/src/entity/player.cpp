#include "entity/player.hpp"

Player::Player()
{

}

static bool lastE = false;

void Player::initialize(cgp::input_devices& inputs, cgp::window_structure& window, World& wrd){
    camera.initialize(inputs, window);
    camera.set_rotation_axis_y();
    set_hunger() = 20;
    setLife() = 20;
    speed = 1.0f;
    ind_inventory = 0;

    inventory.initialize(inventory_size);
    craft.initialize(craft_size);

    item_in_hand = inventory.get_inventory()[ind_inventory][0];
    primary_world.initialize();
    world = wrd;
    isGrounded = true;
    isCreativeMode = true;
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

Inventory& Player::get_inventory(){
    return inventory;
}

Inventory& Player::set_inventory(){
    return inventory;
}

Inventory& Player::get_craft(){
    return craft;
}
Inventory& Player::set_craft(){
    return craft;
}

cgp::camera_controller_first_person_euler Player::get_camera() const{
    return camera;
}

cgp::camera_controller_first_person_euler& Player::set_camera(){
    return camera;
}

std::shared_ptr<Item> Player::get_item_in_hand() const{
    return item_in_hand;
}

std::shared_ptr<Item>& Player::set_item_in_hand(){
    return item_in_hand;
}

int Player::get_item_ind() const{
    return ind_inventory;
}

int& Player::set_item_ind(){
    return ind_inventory;
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

    if (keyboard.is_pressed(GLFW_KEY_E) && !lastE)
    {
        inventory.set_opened_inventory() = !inventory.get_opened_inventory();
    }

    lastE = keyboard.is_pressed(GLFW_KEY_E);

    if (keyboard.is_pressed(GLFW_KEY_Q)){
        set_speed() = 0.05f;
    }

    if (keyboard.is_pressed(GLFW_KEY_LEFT_SHIFT)){
        set_speed() = 0.005f;
    }

    if (position.y <= 0) {
        position.y = 0;  
        isGrounded = true;    
        verticalVelocity = 0.0f; 
    }
    if (colision()==true) {
        verticalVelocity += gravity * dt; 
        position.y += verticalVelocity * dt;  
        isGrounded = false; 
    }

    verticalVelocity = 0.0f;

    if(!isCreativeMode){
        if (isGrounded) {
        verticalVelocity = 0.0f; 
        if (keyboard.is_pressed(GLFW_KEY_SPACE)) {
            verticalVelocity = 350.0f; 
            isGrounded = false;          
        }
        }
        else {
            verticalVelocity -= gravity * dt; 
        }
    }
    else{
        if(keyboard.is_pressed(GLFW_KEY_SPACE)){
            verticalVelocity = 20.0;
        }
        if(keyboard.is_pressed(GLFW_KEY_LEFT_CONTROL)){
            verticalVelocity = -20.0;
        }
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
        position += speed * forward;

        if(colision()==true){
            position -= speed * forward;
        }
    }

    if (keyboard.is_pressed(GLFW_KEY_A)){
        position -= speed * right;
        if(colision()==true){
            position += speed * right;
        }
    }

    if (keyboard.is_pressed(GLFW_KEY_S)){
        position -= speed * forward; 
        if(colision()==true){
            position += speed * forward;
        }
    }

    if (keyboard.is_pressed(GLFW_KEY_D)){
        position += speed * right;
         if(colision()==true){
            position -= speed * right;
        }
    }
    camera.camera_model.position_camera = position;
    camera_view_matrix = camera.camera_model.matrix_view();
}

void Player::handle_mouse_event(const cgp::inputs_mouse_parameters& mouse){
    if (mouse.click.left && !inventory.get_opened_inventory()){
        cgp::vec3 hitblock;
        cgp::vec3 hitnormal;
        if(check_cube(camera.camera_model.position(),camera.camera_model.front(), 5.0f, hitblock, hitnormal)){
            std::cout<<"casser bloc à pos: "<<hitblock<<std::endl;
            inventory.add_inventory(std::shared_ptr<Item>((world.getBlockObject(hitblock))));
            std::cout<<"casser"<<hitblock<<std::endl;
            world.setBlock(hitblock,AIR);
            std::cout<<"c'est fait"<<hitblock<<std::endl;
        }
    }

    if (mouse.click.right && !inventory.get_opened_inventory()){
        cgp::vec3 hitblock;
        cgp::vec3 hitnormal;
        if(check_cube(camera.camera_model.position(),camera.camera_model.front(),5.0f, hitblock,hitnormal) && std::dynamic_pointer_cast<Block>(item_in_hand)){
            std::cout<<"poser bloc à pos: "<<hitblock+hitnormal<<std::endl;
            inventory.erase_inventory(ind_inventory);
            world.setBlock(hitblock+hitnormal,GRASS);
        }
    }

    if (ImGui::GetIO().MouseWheel<0){
        if (ind_inventory+1 < inventory_size/4)
        {
            ind_inventory++;
        }
    }

    if (ImGui::GetIO().MouseWheel>0){
        if (ind_inventory - 1 >= 0)
        {
            ind_inventory--;
        }
    }
}

bool Player::check_cube(const cgp::vec3& origin, const cgp::vec3& direction, float maxDistance, cgp::vec3& hitBlock, cgp::vec3& hitNormal) {
    using namespace cgp;

    vec3 pos = origin;
    vec3 blockPos = vec3(std::floor(pos.x), std::floor(pos.y), std::floor(pos.z));

    vec3 deltaDist = vec3(
        direction.x != 0.0f ? std::abs(1.0f / direction.x) : std::numeric_limits<float>::infinity(),
        direction.y != 0.0f ? std::abs(1.0f / direction.y) : std::numeric_limits<float>::infinity(),
        direction.z != 0.0f ? std::abs(1.0f / direction.z) : std::numeric_limits<float>::infinity()
    );

    vec3 step;
    vec3 sideDist;

    for (int i = 0; i < 3; ++i) {
        if (direction[i] < 0) {
            step[i] = -1;
            sideDist[i] = (pos[i] - blockPos[i]) * deltaDist[i];
        } else {
            step[i] = 1;
            sideDist[i] = (blockPos[i] + 1.0f - pos[i]) * deltaDist[i];
        }
    }

    float distance = 0.0f;
    hitNormal = vec3({0,0,0}); // important : reset

    while (distance < maxDistance) {
        int axis;
        if (sideDist.x < sideDist.y) {
            axis = (sideDist.x < sideDist.z) ? 0 : 2;
        } else {
            axis = (sideDist.y < sideDist.z) ? 1 : 2;
        }

        blockPos[axis] += step[axis];
        distance += deltaDist[axis];
        sideDist[axis] += deltaDist[axis];

        std::cout << "Block position: " << blockPos << " is a " << world.getBlock(blockPos) << std::endl;

        if (world.getBlock(blockPos)) {
            hitBlock = blockPos;
            hitNormal = vec3({0,0,0});
            hitNormal[axis] = -step[axis];
            return true;
        }
    }

    return false;
}

bool Player::colision(){
    for(size_t i = 0; i < primary_world.getVectorBlockType().size(); ++i){
        // std::cout<<primary_world.getVectorBlockType()[i]->getPosition()[0] - 0.5f << " <= " << position.x <<" <= " << primary_world.getVectorBlockType()[i]->getPosition()[0] + 0.5f   << " / " << primary_world.getVectorBlockType()[i]->getPosition()[1] - 0.5f << " <= " << position.y <<" <= " << primary_world.getVectorBlockType()[i]->getPosition()[1] + 0.5f   << " / "<< primary_world.getVectorBlockType()[i]->getPosition()[2] - 0.5f << " <= " << position.z <<" <= " << primary_world.getVectorBlockType()[i]->getPosition()[2] + 0.5f   << " / " << std::endl;
        if((primary_world.getVectorBlockType()[i]->getPosition()[0] - 0.5f <= position.x && position.x <= primary_world.getVectorBlockType()[i]->getPosition()[0] + 0.5f) && (primary_world.getVectorBlockType()[i]->getPosition()[1] - 0.5f <= position.y && position.y <= primary_world.getVectorBlockType()[i]->getPosition()[1] + 0.5f) && (primary_world.getVectorBlockType()[i]->getPosition()[2] - 0.5f <= position.z && position.z <=  primary_world.getVectorBlockType()[i]->getPosition()[2] + 0.5)){
            // std::cout<<"Colision"<<std::endl;
            return true;
        }
    }
    // std::cout<<"Pas de Colision"<<std::endl;
    return false;
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