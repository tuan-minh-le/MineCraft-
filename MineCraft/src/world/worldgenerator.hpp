#pragma once
#include "cgp/cgp.hpp"
#include "chunk.hpp"

const int HEIGHTOFFSET = 10;
const float HEIGHTINTENSITY = 20.f; 

class WorldGenerator {
private:
    cgp::vec2 noiseScale = {0.01, 0.01};
    cgp::vec2 noiseOffset = {0, 0};
    
public:
    WorldGenerator();
    ~WorldGenerator();
    
    void initialize();
    
    void generateChunk(Chunk& chunk);

    void generateChunk(Chunk& chunk, const cgp::vec3& position);
    
    int generateHeightAt(float worldX, float worldZ);
    BlockType getBlockTypeAt(float worldX, float worldY, float worldZ, int surfaceHeight);

    void setNoiseScale(const cgp::vec2& scale);
    void setNoiseOffset(const cgp::vec2& offset);

    cgp::vec2 getNoiseScale() const;
    cgp::vec2 getNoiseOffset() const;
};

