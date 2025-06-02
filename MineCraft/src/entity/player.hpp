#pragma once

#include "cgp/cgp.hpp"
#include "iostream"
#include "block/item.hpp"
#include "block/grass.hpp"
#include <vector>
#include <memory>
#include "entity.hpp"

class Player : public Entity
{
private:
    cgp::camera_controller_first_person_euler camera;
    int hunger;
    std::vector<std::shared_ptr<Item>> inventory;
    bool opened_inventory = false;
    float speed = 1.0f;

public:
    Player();
    ~Player();

    void initialize(cgp::input_devices& inputs, cgp::window_structure& window);

    float max_pitch_up = 85.0f;
    float max_pitch_down = -85.0f;

    int inventory_size = 40;

    int get_hunger() const;
    int& set_hunger();

    float get_speed() const;
    float& set_speed();

    cgp::camera_controller_first_person_euler get_camera() const;
    cgp::camera_controller_first_person_euler& set_camera();

    void add_inventory (std::shared_ptr<Item> item, int ind);
    void erase_inventory (int ind);
    void switch_inventory(int ind1, int ind2);
    void open_inventory();
    void close_inventory();

    bool get_opened_inventory() const;
    bool& set_opened_inventory();

    void move(float speed,const cgp::inputs_keyboard_parameters& keyboard);

    std::vector<std::shared_ptr<Item>> get_inventory() const;
    std::vector<std::shared_ptr<Item>>& set_inventory();

    void handle_mouse_move(cgp::vec2 const& mouse_position_current, cgp::vec2 const& mouse_position_previous, cgp::mat4& camera_view_matrix);

    void handle_keyboard_event(const cgp::inputs_keyboard_parameters& keyboard);
};
