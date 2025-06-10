#pragma once

#include "cgp/cgp.hpp"
#include "iostream"
#include "block/item.hpp"
#include <vector>
#include <memory>
#include <algorithm>
#include "entity.hpp"
#include "inventory/inventory.hpp"
#include "world/chunk.hpp"
#include "world/primary_world.hpp"
#include "world/world.hpp"

class Player : public Entity
{
private:
    cgp::camera_controller_first_person_euler camera;
    int hunger;
    Inventory inventory;
    Inventory craft;
    std::shared_ptr<Item> item_in_hand;
    float speed;
    int ind_inventory;

    bool isGrounded;
    bool isCreativeMode;
    float verticalVelocity;
    float gravity;
    float dt;
    World world;
    Primary_World primary_world;
    
    // std::string nameObj = "../../assets/Minecraft_steve/minecraft_steve.obj";
    // std::string nameTexture = "../../assets/Minecraft_steve/minecraft_steve_skin.jpg";

public:
    Player();
    ~Player();

    void initialize(cgp::input_devices& inputs, cgp::window_structure& window, World& wrd);

    float max_pitch_up = 85.0f;
    float max_pitch_down = -85.0f;

    int inventory_size = 36;
    int craft_size = 5;

    int get_hunger() const;
    int& set_hunger();

    Inventory& get_inventory();
    Inventory& set_inventory();

    Inventory& get_craft();
    Inventory& set_craft();

    std::shared_ptr<Item> get_item_in_hand() const;
    std::shared_ptr<Item>& set_item_in_hand();

    int get_item_ind() const;
    int& set_item_ind();

    float get_speed() const;
    float& set_speed();

    cgp::camera_controller_first_person_euler get_camera() const;
    cgp::camera_controller_first_person_euler& set_camera();

    void move(float speed,const cgp::inputs_keyboard_parameters& keyboard,cgp::mat4& camera_view_matrix);

    void handle_mouse_move(cgp::vec2 const& mouse_position_current, cgp::vec2 const& mouse_position_previous, cgp::mat4& camera_view_matrix);

    void handle_keyboard_event(const cgp::inputs_keyboard_parameters& keyboard,cgp::mat4& camera_view_matrix);

    void handle_mouse_event(const cgp::inputs_mouse_parameters& mouse);

    bool check_cube(const cgp::vec3& origin, const cgp::vec3& direction, float maxDistance, cgp::vec3& hitBlock, cgp::vec3& hitNormal);

    float normeCarre();
    
    bool colision();
    // std::string getNameObj()const;
    // std::string& setNameObj();
    // std::string getNameTexture()const;
    // std::string& setNameTexture();
};
