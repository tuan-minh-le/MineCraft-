#include "block.hpp"

int Block::getDimension() const{
    return dimension;
} 

cgp::vec3 Block::getPosition() const{
    return position;
}

cgp::vec3& Block::setPosition(){
    return position;
}
