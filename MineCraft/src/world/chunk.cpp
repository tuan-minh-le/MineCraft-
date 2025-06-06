#include "chunk.hpp"

Chunk::Chunk():chunkWorldPosition({0, 0, 0}){}

Chunk::Chunk(const cgp::vec3& worldPos):chunkWorldPosition(worldPos){}

Chunk::~Chunk(){}

void Chunk::initialize(){
    blockData.resize(chunkSize.width * chunkSize.height * chunkSize.depth, AIR);

    grass.initialize();
    stone.initialize();
    sand.initialize();
}

void Chunk::setWorldPosition(const cgp::vec3& position){
    chunkWorldPosition = position;
}

void Chunk::setBlock(int x, int y , int z, BlockType blockType){
    if (!isValidCoordinate(x, y, z)) {
        std::cerr << "Invalid coordinates: " << x << ", " << y << ", " << z << std::endl;
        return;
    }
    int index = coordinateToIndex(x, y, z);
    blockData[index] = blockType;
}

BlockType Chunk::getBlock(int x, int y, int z) const{
    if(!isValidCoordinate(x, y, z)){
        return AIR;
    }

    int index = coordinateToIndex(x, y, z);
    return blockData[index];
}

bool Chunk::isBlockSolid(int x, int y, int z) const{
    return getBlock(x, y, z) != AIR;
}

bool Chunk::shouldRenderFace(int x, int y, int z, int faceDirection) const{
    // Testing 6-connectivity without checking diagonals
    static const int directions[6][3] = {
        {-1, 0, 0}, {1, 0, 0}, 
        {0, -1, 0}, {0, 1, 0},
        {0, 0, -1}, {0, 0, 1}
    };

    int neighborX = x + directions[faceDirection][0];
    int neighborY = y + directions[faceDirection][1];
    int neighborZ = z + directions[faceDirection][2];

    return !isBlockSolid(neighborX, neighborY, neighborZ);
}

void Chunk::render(const cgp::environment_generic_structure& environment){
    for(int x = 0; x < chunkSize.width; x++){
        for(int y = 0; y < chunkSize.height; y++){
            for(int z = 0; z < chunkSize.depth; z++){
                BlockType block = getBlock(x, y, z);
                if(block != AIR){
                    bool hasVisibleFace = false;
                    for(int face = 0; face < 6; face++){
                        if(shouldRenderFace(x, y, z , face)){
                            hasVisibleFace = true;
                            break;
                        }
                    }

                    if(hasVisibleFace){
                        drawBlockAt(x, y, z, block, environment);
                    }
                }

                
            }
        }
    }
}

void Chunk::drawBlockAt(int x, int y, int z, BlockType block, const cgp::environment_generic_structure& environment){
        cgp::vec3 worldPos = localToWorld({
        static_cast<float>(x), 
        static_cast<float>(y), 
        static_cast<float>(z)
    });
    
    switch(block) {
        case GRASS:
            grass.setPosition() = worldPos;
            grass.draw_block_at(environment);
            break;
        case DIRT:
            grass.setPosition() = worldPos;
            grass.draw_block_at(environment);
            break;
        case SAND:
            sand.setPosition() = worldPos;
            sand.draw_block_at(environment);
            break;
        case STONE:
            stone.setPosition() = worldPos;
            stone.draw_block_at(environment);
            break;
        case BEDROCK:
            stone.setPosition() = worldPos;
            stone.draw_block_at(environment);
            break;
        case AIR:
            break;
    }
}

cgp::vec3 Chunk::localToWorld(const cgp::vec3& localPos) const {
    return chunkWorldPosition + localPos;
}

cgp::vec3 Chunk::worldToLocal(const cgp::vec3& worldPos) const {
    return worldPos - chunkWorldPosition;
}

bool Chunk::isValidCoordinate(int x, int y, int z) const{
    return x >= 0 && x < chunkSize.width &&
        y >= 0 && y < chunkSize.height &&
        z >= 0 && z < chunkSize.depth;
}

int Chunk::coordinateToIndex(int x, int y, int z) const {
    return z * (chunkSize.width * chunkSize.height) + y * chunkSize.width + x;
}