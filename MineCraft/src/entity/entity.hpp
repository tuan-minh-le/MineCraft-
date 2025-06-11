#pragma once

#include "cgp/cgp.hpp"
#include "world/world.hpp"
#include "iostream"

class Entity{
    protected:
    int life;
    cgp::mesh_drawable entity_mesh;
    cgp::vec3 position;
    float speed;
    cgp::vec3 testPosition;
    World* world;

    public:
    
    int getCooHeight(World* world);
    virtual void initialize(cgp::input_devices& inputs, cgp::window_structure& window,World* wrd) = 0;
    virtual void move(float speed, const cgp::inputs_keyboard_parameters& keyboard,cgp::mat4& camera_view_matrix) = 0;

    //Getter and Setter

    int getLife() const;
    int& setLife();
    cgp::vec3 getPosition() const;
    cgp::vec3& setPosition();
    cgp::vec3 getTestPosition() const;
    cgp::vec3& setTestPosition();
    cgp::mesh_drawable getEntityMesh() const;
    cgp::mesh_drawable& setEntityMesh();

    float get_speed() const;
    float& set_speed();


};