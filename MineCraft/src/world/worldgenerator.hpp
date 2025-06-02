#pragma once
#include "cgp/cgp.hpp"

struct ChunkSize{
    int width;
    int height;
    int depth;
};

const int HEIGHTOFFSET = 60;
const float HEIGHTINTENSITY = 5.f; 

class WorldGenerator{
    private:
        ChunkSize chunkSize = {16, 16, 16};
        cgp::vec2 noiseScale = {1, 1};
        cgp::vec2 noiseOffset = {0, 0};

        std::vector<int> tempData;
    public:
        WorldGenerator();
        ~WorldGenerator();
        
        void initialize();

        int getData(int x, int y, int z) const;
        int& setData(int x, int y, int z);

};