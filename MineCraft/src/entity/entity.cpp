#include "cgp/cgp.hpp"
#include "iostream"
#include "entity.hpp"

using namespace cgp;
//Methodes




//Setters getters

int Entity::getLife() const{
    return life;
};
int& Entity::setLife(){
    return life;
};
cgp::vec3 Entity::getPosition() const{
    return position;
};
cgp::vec3& Entity::setPosition(){
    return position;
};
cgp::mesh_drawable Entity::getEntityMesh() const{
    return entity_mesh;
}
cgp::mesh_drawable& Entity::setEntityMesh(){
    return entity_mesh;
}