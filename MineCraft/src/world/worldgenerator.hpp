#pragma once
#include "cgp/cgp.hpp"
#include "block/grass.hpp"
#include "block/sand.hpp"
#include "block/stone.hpp"

struct ChunkSize{
    int width;
    int height;
    int depth;
};

const int HEIGHTOFFSET = 60;
const float HEIGHTINTENSITY = 31.f; 

class WorldGenerator{
    private:
        ChunkSize chunkSize = {16, 16, 16};
        cgp::vec2 noiseScale = {1, 1};
        cgp::vec2 noiseOffset = {0, 0};

        Grass grass;
        Stone stone;
        Sand sand;
        
        cgp::vec3 chunkCenter;
        cgp::vec3 chunkWorldMin;
    
        std::vector<int> tempData;
    public:
        WorldGenerator();
        ~WorldGenerator();
        

        void initialize();
        void setCenter(const cgp::vec3& center);

        cgp::vec3 localtoWorld(const cgp::vec3& localPos) const;
        cgp::vec3 worldtoLocal(const cgp::vec3& worldPos) const;

        void drawChunk(const cgp::environment_generic_structure& environment);
        int getData(int x, int y, int z) const;
        int& setData(int x, int y, int z);

};