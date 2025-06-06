#include "primary_world.hpp"

Primary_World::Primary_World(){}

Primary_World::~Primary_World(){}

void Primary_World::initialize(){
    //primary world initialization
	std::vector<int> positions = {0, 1, 2};
	//std::vector<Block*> vectorBlockType;
	for(int x : positions){
		for(int y : positions){
			for(int z : positions){
				cgp::vec3 position = {x, y, z};
				Grass* grass;
				grass = new Grass();
				grass->setPosition() = position;
				vectorBlockType.push_back(grass);
			}
		}
	}
}


std::vector<Block*> Primary_World::getVectorBlockType() const{
    return vectorBlockType;
}