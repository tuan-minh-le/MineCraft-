#include "block.hpp"

Block::Block():position({0,0,0}){}

Block::Block(const cgp::vec3& position):position(position){}



int Block::getDimension() const{
    return dimension;
} 

cgp::vec3 Block::getPosition() const{
    return position;
}

cgp::vec3& Block::setPosition(){
    return position;
}
