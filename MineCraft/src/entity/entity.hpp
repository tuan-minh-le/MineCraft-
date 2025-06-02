#pragma once

#include "cgp/cgp.hpp"
#include "iostream"

class Entity{
    protected:
    int life;
    cgp::mesh_drawable entity_mesh;
    cgp::vec3 position;

    public:

    virtual void initialize(cgp::input_devices& inputs, cgp::window_structure& window) = 0;
    virtual void move(float speed, const cgp::inputs_keyboard_parameters& keyboard,cgp::mat4& camera_view_matrix) = 0;

    //Getter and Setter

    int getLife() const;
    int& setLife();
    cgp::vec3 getPosition() const;
    cgp::vec3& setPosition();
    cgp::mesh_drawable getEntityMesh() const;
    cgp::mesh_drawable& setEntityMesh();


};