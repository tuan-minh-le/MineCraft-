#include "cgp/cgp.hpp"
#include "iostream"
#include "entity.hpp"

using namespace cgp;
//Methodes

// int Entity::getCooHeight(World& entity_world){
//     // std::cout<<"lol3"<<std::endl;
//     cgp::vec3 testposition = {position.x,1.0f,position.z};
//     if(entity_world.getChunkAt(testposition) != nullptr){
//         // std::cout<<"lol4  "<<entity_world.getChunkAt(testposition)<<std::endl;
//         // std::cout<<"lol5  "<<entity_world.getChunkAt(testposition)->getSize().height<<std::endl;
//     for(int i = 0; entity_world.getChunkAt(testposition)->getSize().height; i++){
//         // std::cout<<"lol6  "<<i<<std::endl;
//         if(entity_world.getChunkAt(testposition)->getBlock(testposition.x, i, testposition.z) == 0) {
//             return i;
//         }
//     }
//     return 0;
// }
// }

//----------Chatgpt Version-----------------
int Entity::getCooHeight(World* entity_world){
    cgp::vec3 testposition = {position.x, 1.0f, position.z};

    if (entity_world->getChunkAt(testposition) != nullptr) {
        int maxHeight = entity_world->getChunkAt(testposition)->getSize().height;

        for (int i = maxHeight - 1; i >= 0; --i) {
            if (entity_world->getChunkAt(testposition)->getBlock(testposition.x, i, testposition.z) != 0) {
                // Retourne le premier bloc solide trouvé en partant du haut
                return i + 1; // position du haut du bloc
            }
        }
    }

    // Aucun bloc trouvé, retourne 0 par défaut
    return 0;
}

//Setters getters

float Entity::get_speed() const{
    return speed;
}

float& Entity::set_speed(){
    return speed;
}

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
cgp::vec3 Entity::getTestPosition() const{
    return testPosition;
};
cgp::vec3& Entity::setTestPosition(){
    return testPosition;
};

cgp::mesh_drawable Entity::getEntityMesh() const{
    return entity_mesh;
}
cgp::mesh_drawable& Entity::setEntityMesh(){
    return entity_mesh;
}