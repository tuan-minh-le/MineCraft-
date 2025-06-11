#include "entity/player.hpp"

Player::Player()
{

}

static bool lastE = false;

void Player::initialize(const cgp::vec3& p_position, cgp::input_devices& inputs, cgp::window_structure& window, World* wrd){
    camera.initialize(inputs, window);
    camera.set_rotation_axis_y();
    set_hunger() = 20;
    setLife() = 20;
    speed = 0.01f;
    ind_inventory = 0;

    inventory.initialize(inventory_size);
    craft.initialize(craft_size);

    item_in_hand = inventory.get_inventory()[ind_inventory][0];
    world = wrd;
    isGrounded = true;
    isCreativeMode = true;
    verticalVelocity = 0;
    gravity = 119.81f;
    dt = 0.05f;
    position = p_position;
    
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

    if (keyboard.is_pressed(GLFW_KEY_P) && !lastE)
    {   
        position.x = 10.0f;
        position.y = 15.0f;
        position.z = 10.0f;
    }

    set_speed() = 0.1;

    if (keyboard.is_pressed(GLFW_KEY_Q)){
        set_speed() = 0.2f;
    }

    if (keyboard.is_pressed(GLFW_KEY_LEFT_SHIFT)){
        set_speed() = 0.05f;
        
    }

    if (keyboard.is_pressed(GLFW_KEY_1)) ind_inventory = 0;
    if (keyboard.is_pressed(GLFW_KEY_2)) ind_inventory = 1;
    if (keyboard.is_pressed(GLFW_KEY_3)) ind_inventory = 2;
    if (keyboard.is_pressed(GLFW_KEY_4)) ind_inventory = 3;
    if (keyboard.is_pressed(GLFW_KEY_5)) ind_inventory = 4;
    if (keyboard.is_pressed(GLFW_KEY_6)) ind_inventory = 5;
    if (keyboard.is_pressed(GLFW_KEY_7)) ind_inventory = 6;
    if (keyboard.is_pressed(GLFW_KEY_8)) ind_inventory = 7;
    if (keyboard.is_pressed(GLFW_KEY_9)) ind_inventory = 8;

    if (inventory.get_inventory()[ind_inventory].empty())
    {
        item_in_hand = nullptr;
    }
    else{
        item_in_hand = inventory.get_inventory()[ind_inventory][0];
    }

    if (position.y <= 0) {
        position.y = 0;  
        isGrounded = true;    
        verticalVelocity = 0.0f; 
    }
    if(gravityAplication()==true){
        isGrounded = false; 
        // std::cout<<isGrounded<<std::endl;
        // std::cout<<"FUCKMINH"<<std::endl;
    }
    if (colision()==true) {
        verticalVelocity += gravity * dt; 
        position.y += verticalVelocity * dt;  
        isGrounded = true; 
        
    }
    

    verticalVelocity = 0.0f;

    if(!isCreativeMode){
        if (isGrounded) {
        verticalVelocity = 0.0f; 
        if (keyboard.is_pressed(GLFW_KEY_SPACE)) {
            verticalVelocity = 80.0f; 
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
    if(!isGrounded){
        //std::cout<<"oui / "<<position.y<<std::endl;
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

        // if(colision()==true){
        //     position -= speed * forward;
        // }
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
            if((*world).getBlock(hitblock) != BlockType::BEDROCK && inventory.add_inventory(std::shared_ptr<Item>(((*world).getBlockObject(hitblock))))){
                std::cout<<"cassé"<<std::endl;
                (*world).setBlock(hitblock,AIR);
            }
        }        
    }
    if (ImGui::IsMouseDown(1) && !inventory.get_opened_inventory()) {
        cgp::vec3 hitblock;
        cgp::vec3 hitnormal;
        if(check_cube(camera.camera_model.position(),camera.camera_model.front(),5.0f, hitblock,hitnormal) && std::dynamic_pointer_cast<Block>(item_in_hand)){
            BlockType type = std::dynamic_pointer_cast<Block>(item_in_hand)->get_type();
            if(inventory.erase_inventory(ind_inventory)){
                std::cout<<"bloc posé de " <<item_in_hand->getItemName() << " a la pos" << hitnormal+hitblock <<std::endl;
                (*world).setBlock(hitblock+hitnormal,type);
            }
        }
    }

    if (ImGui::IsMouseClicked(1)){
        cgp::vec3 hitblock;
        cgp::vec3 hitnormal;
        int hunger = get_hunger();
        if(check_cube(camera.camera_model.position(),camera.camera_model.front(),5.0f, hitblock,hitnormal) && (*world).getBlock(hitblock) == BlockType::BEDROCK){
            Interractive* interractive_block = dynamic_cast<Interractive*>((*world).getBlockObject(hitblock));
            std::cout<<"table de craft"<<std::endl;
            interractive_block->action(&hunger);
        }
        else if(std::dynamic_pointer_cast<Tool>(item_in_hand) && !std::dynamic_pointer_cast<Interractive>(item_in_hand)){
            std::shared_ptr<Tool> tool = std::dynamic_pointer_cast<Tool>(item_in_hand);
            std::cout<<"manger"<<hunger<<std::endl;
            tool->action(&hunger);
            std::cout<<"mangera"<<hunger<<std::endl;
            set_hunger()= hunger;
        }
        
    }

    if (ImGui::GetIO().MouseWheel<0){
        if (ind_inventory+1 < inventory_size/4)
        {
            ind_inventory++;
            if (inventory.get_inventory()[ind_inventory].empty())
            {
                item_in_hand = nullptr;
            }
            else{
                item_in_hand = inventory.get_inventory()[ind_inventory][0];
            }
        }
    }

    if (ImGui::GetIO().MouseWheel>0){
        if (ind_inventory - 1 >= 0)
        {
            ind_inventory--;
            if (inventory.get_inventory()[ind_inventory].empty())
            {
                item_in_hand = nullptr;
            }
            else{
                item_in_hand = inventory.get_inventory()[ind_inventory][0];
            }
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

        if ((*world).getBlock(blockPos)) {
            std::cout << (*world).getBlock(blockPos) << std::endl;
            hitBlock = blockPos;
            hitNormal = vec3({0,0,0});
            hitNormal[axis] = -step[axis];
            return true;
        }
    }

    return false;
}

bool Player::colision(){

    if((*world).getChunkAt(position) != nullptr){
    int cpt = 0;
    for(auto val: (*world).getChunkAt(position)->getBlockObjectList()){
        cpt += 1;
        if(val!=nullptr){

        if((val->getPosition()[0]  - 0.5f <= position.x && position.x <= val->getPosition()[0]  + 0.5f) && (val->getPosition()[1]  + 0.5f <= position.y && position.y <= val->getPosition()[1]  + 1.5f) && (val->getPosition()[2]  - 0.5f <= position.z && position.z <=  val->getPosition()[2]  + 0.5)){
            return true;
            }
        }  
    }
    }
    return false;
}


bool Player::gravityAplication(){
    if(colision()){
    if((*world).getChunkAt(position) != nullptr){
    int cpt = 0;
    for(auto val: (*world).getChunkAt(position)->getBlockObjectList()){
        cpt += 1;
        if(val!=nullptr){
            //std::cout<<position.y-1.0f<<" / "<<val->getPosition()[1]  - 0.5f<<std::endl;
        if((val->getPosition()[0]  - 0.5f <= position.x && position.x <= val->getPosition()[0]  + 0.5f) && (val->getPosition()[1]  - 0.5f <= position.y-1.0f && position.y-1 <= val->getPosition()[1]  + 0.5f) && (val->getPosition()[2]  - 0.5f <= position.z && position.z <=  val->getPosition()[2]  + 0.5)){
            return true;
            }
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