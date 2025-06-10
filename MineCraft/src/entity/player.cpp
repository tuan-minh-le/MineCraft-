#include "entity/player.hpp"

Player::Player()
{

}

static bool lastE = false;

void Player::initialize(cgp::input_devices& inputs, cgp::window_structure& window){
    camera.initialize(inputs, window);
    camera.set_rotation_axis_y();
    set_hunger() = 20;
    setLife() = 20;
    speed = 1.0f;
    ind_inventory = 0;
    inventory.initialize(inventory_size);
    std::shared_ptr<Item> itemPtr = inventory.get_inventory()[ind_inventory];
    primary_world.initialize();
    world.initialize(1, 1, 20);
    isGrounded = true;
    isCreativeMode = true;
    verticalVelocity = 0;
    gravity = 119.81f;
    dt = 0.01f;
    position.x = 10.0f;
    position.y = 15.0f;
    position.z = 10.0f;
    
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

Inventory Player::get_inventory() const{
    return inventory;
}

Inventory& Player::set_inventory(){
    return inventory;
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

    lastE = false; 
    if (keyboard.is_pressed(GLFW_KEY_E) && !lastE)
    {
        inventory.set_opened_inventory() = !inventory.get_opened_inventory();
    }

    lastE = keyboard.is_pressed(GLFW_KEY_E);

    if (keyboard.is_pressed(GLFW_KEY_P) && !lastE)
    {   
        position.x = 10.0f;
        position.y = 15.0f;
        position.z = 10.0f;
    }

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
        // std::cout<<"move z"<<std::endl;
        // std::cout<<"move z"<<std::endl;

        // std::cout<< "position after" << position<<std::endl;
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
    if (mouse.click.left){
        cgp::vec3 hitblock;
        cgp::vec3 hitnormal;
        if(check_cube(camera.camera_model.position(),camera.camera_model.front(),5.0f, hitblock,hitnormal)){
            //casser_cube
        }
    }

    if (mouse.click.right){
        cgp::vec3 hitblock;
        cgp::vec3 hitnormal;
        if(check_cube(camera.camera_model.position(),camera.camera_model.front(),5.0f, hitblock,hitnormal)){
            //Poser_cube
        }
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

bool Player::check_cube(const cgp::vec3& origin, const cgp::vec3& direction, float maxDistance, cgp::vec3& hitBlock, cgp::vec3& hitNormal) {
    cgp::vec3 pos = origin;
    cgp::vec3 blockPos = cgp::vec3({std::floor(pos.x),std::floor(pos.y),std::floor(pos.z)});
    
    cgp::vec3 deltaDist = cgp::vec3({std::abs(1.0f/direction.x),std::abs(1.0f/direction.y),std::abs(1.0f/direction.z)});
    cgp::vec3 step;
    cgp::vec3 sideDist;

    for (int i = 0; i < 3; ++i) {
        if (direction[i] < 0) {
            step[i] = -1;
            sideDist[i] = (pos[i] - blockPos[i]) * deltaDist[i];
        } else {
            step[i] = 1;
            sideDist[i] = (blockPos[i] + 1.0f - pos[i]) * deltaDist[i];
        }
    }

    // std::cout<<"Position "<<pos<<"Delta "<<deltaDist<<"step "<<step<<"Sidedist "<<sideDist<<std::endl; 

    for (float distance = 0.0f; distance < maxDistance;) {

        int axis;
        if (sideDist.x < sideDist.y){
            axis = (sideDist.x < sideDist.z) ? 0 : 2;
        } else {
            axis = (sideDist.y < sideDist.z) ? 1 : 2;
        }
        // auto blockpos_int = worldToLocal(blockPos);

        // if (isSolidBlock(blockpos_int.x,blockpos_int.y,blockpos_int.z)){
        //     hitBlock = blockPos;
        //     hitNormal[axis] = -step[axis];
        //     return true;
        // }

        blockPos[axis] += step[axis];
        distance = sideDist[axis];
        // std::cout<<"Distance"<< blockPos<<std::endl;
        sideDist[axis] += deltaDist[axis];
    }

    return false;
}

    


bool Player::colision(){


    if(world.getChunkAt(position) != nullptr){




    int cpt = 0;
    for(auto val: world.getChunkAt(position)->getBlockObjectList()){
        cpt += 1;
        if(val!=nullptr){

        
        if((val->getPosition()[0]  - 0.5f <= position.x && position.x <= val->getPosition()[0]  + 0.5f) && (val->getPosition()[1]  - 0.5f <= position.y && position.y <= val->getPosition()[1]  + 0.5f) && (val->getPosition()[2]  - 0.5f <= position.z && position.z <=  val->getPosition()[2]  + 0.5)){
            return true;
            }
        }  
    }
    }

    return false;
}

float Player::normeCarre(){
    return getPosition()[0]*getPosition()[0]+getPosition()[1]*getPosition()[1]+getPosition()[2]*getPosition()[2];
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