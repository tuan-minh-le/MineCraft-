#include "worldgenerator.hpp"

WorldGenerator::WorldGenerator(){}

WorldGenerator::~WorldGenerator(){}

void WorldGenerator::initialize(){
    tempData.resize(chunkSize.depth * chunkSize.height * chunkSize.width);
    for(int x = 0; x < chunkSize.width; x++){
        for(int z = 0; z < chunkSize.depth; z++){
            float perlinCoord_x = noiseOffset.x + x / (float)chunkSize.width * noiseScale.x;
            float perlinCoord_y = noiseOffset.y + z / (float)chunkSize.depth * noiseScale.y;
            int HeightGen = HEIGHTINTENSITY * (cgp::noise_perlin({perlinCoord_x, perlinCoord_y}));
            std::cout << "Height Gen: " << HeightGen << std::endl;
            HeightGen = std::min(HeightGen, chunkSize.height - 1);
            for(int y = HeightGen; y >= 0; y--){
                int blockType;
                
                if(y == HeightGen) blockType = 1;

                if(y < HeightGen && y > HeightGen - 4) blockType = 2;

                if(y <= HeightGen - 4 && y > 0) blockType = 3;

                if(y == 0) blockType = 4;

                setData(x, y, z) = blockType;
            }
        }
    }
}

void WorldGenerator::setCenter(const cgp::vec3& center){
    chunkCenter = center;

    chunkWorldMin.x = std::floor(chunkCenter.x - chunkSize.width * 0.5f);
    chunkWorldMin.y = std::floor(chunkCenter.y - chunkSize.height * 0.5f);
    chunkWorldMin.z = std::floor(chunkCenter.z - chunkSize.depth * 0.5f);
}

cgp::vec3 WorldGenerator::localtoWorld(const cgp::vec3& localPos) const{
    return chunkWorldMin + localPos;
}

cgp::vec3 WorldGenerator::worldtoLocal(const cgp::vec3& worldPos) const{
    return worldPos - chunkWorldMin;
}

int WorldGenerator::getData(int x, int y, int z) const{
    if(x < 0 || x >= chunkSize.width || y < 0 || y >= chunkSize.height || z < 0 || z >= chunkSize.depth){
        std::cout << "Invalid coordinates" << std::endl;
    }
    int idx = z * (chunkSize.width * chunkSize.height) + y * chunkSize.width + x;
    return tempData[idx];
}

int& WorldGenerator::setData(int x, int y, int z){
    if(x < 0 || x >= chunkSize.width || y < 0 || y >= chunkSize.height || z < 0 || z >= chunkSize.depth){
        std::cout << "Invalid coordinates" << std::endl;
    }
    int idx = z * (chunkSize.width * chunkSize.height) + y * chunkSize.width + x;
    return tempData[idx];
}

void WorldGenerator::drawChunk(const cgp::environment_generic_structure& environment){
    for(int x = 0; x < chunkSize.width; x++) {
        for(int y = 0; y < chunkSize.height; y++) {
            for(int z = 0; z < chunkSize.depth; z++) {
                int blockType = getData(x, y, z);
                if(blockType > 0) {
                    cgp::vec3 worldPos = localtoWorld({static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)});
                    
                    // Draw different block types
                    switch(blockType) {
                        case 1: // Grass
        
                            grass.draw_block_at(worldPos, environment);
                            // std::cout << "Drew grass at: " << worldPos << std::endl;
                            break;
                        case 2: // Sand
                            grass.draw_block_at(worldPos, environment);
                            // std::cout << "Drew sand at: " << cgp::vec3(x, y, z) << std::endl;
                            break;
                        case 3: // Stone
                            sand.draw_block_at(worldPos, environment);
                            // std::cout << "Drew stone at: " << worldPos << std::endl;
                            break;
                        case 4:
                            stone.draw_block_at(worldPos, environment);
                            // std::cout << "Drew stone at: " << worldPos << std::endl;
                            break;
                    }
                }
            }
        }
    }
}