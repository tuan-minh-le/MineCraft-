#pragma once

#include "cgp/cgp.hpp"
#include "iostream"
#include "block/item.hpp"
#include "block/grass.hpp"
#include <vector>
#include <memory>

class Player
{
private:
    cgp::camera_controller_first_person_euler camera;
    int hunger;
    std::vector<std::shared_ptr<Item>> inventory;

public:
    Player();
    ~Player();

    float max_pitch_up = 85.0f;
    float max_pitch_down = -85.0f;

    int inventory_size = 40;

    int get_hunger() const;
    int& set_hunger();

    cgp::camera_controller_first_person_euler get_camera() const;
    cgp::camera_controller_first_person_euler& set_camera();

    void add_inventory (Item &item, int ind);
    void erase_inventory (int ind);
    void switch_inventory(Item &item1, Item &item2);

    std::vector<std::shared_ptr<Item>> get_inventory() const;
    std::vector<std::shared_ptr<Item>>& set_inventory();

    void handle_mouse_move(cgp::vec2 const& mouse_position_current, cgp::vec2 const& mouse_position_previous, cgp::mat4& camera_view_matrix);
};
