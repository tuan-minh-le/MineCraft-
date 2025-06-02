#include "worldgenerator.hpp"

WorldGenerator::WorldGenerator(){}

WorldGenerator::~WorldGenerator(){}

void WorldGenerator::initialize(){
    tempData.resize(chunkSize.depth * chunkSize.height * chunkSize.width);
    for(int x = 0; x < chunkSize.width; x++){
        for(int z = 0; z < chunkSize.depth; z++){
            float perlinCoord_x = noiseOffset.x + x / (float)chunkSize.width * noiseScale.x;
            float perlinCoord_y = noiseOffset.y + z / (float)chunkSize.depth * noiseScale.y;
            int HeightGen = int(cgp::noise_perlin({perlinCoord_x, perlinCoord_y}));
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