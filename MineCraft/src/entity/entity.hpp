#pragma once

#include "cgp/cgp.hpp"
#include "iostream"

class Entity{
    protected:
    int life;
    cgp::mesh_drawable entity_mesh;
    cgp::vec3 position;

    public:

    virtual void initialize() = 0;
    virtual void movement();

    //Getter and Setter

    int getLife() const;
    int& setLife();
    cgp::vec3 getPosition() const;
    cgp::vec3& setPosition();
    cgp::mesh_drawable getEntityMesh() const;
    cgp::mesh_drawable& setEntityMesh();


};