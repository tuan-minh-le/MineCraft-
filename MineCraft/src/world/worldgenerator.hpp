#pragma once
#include "cgp/cgp.hpp"
#include "chunk.hpp"
#include "../extern/fastnoise/FastNoise.h"

const int HEIGHTOFFSET = 5;
const float HEIGHTINTENSITY = 2000.f; 

enum BiomeType{
    DESERT = 0,
    PLAINS = 1, 
    SNOWBIOME = 2
};

class WorldGenerator {
private:
    cgp::vec2 noiseScale = {0.01, 0.01};
    cgp::vec2 noiseOffset = {0, 0};

    cgp::vec2 temperatureScale = {0.004, 0.004};
    cgp::vec2 temperatureOffset = {600, 600};

    cgp::vec2 humidityScale = {0.006, 0.006};
    cgp::vec2 humidityOffset = {500, 500};


    FastNoise noise;
    
public:
    WorldGenerator();
    ~WorldGenerator();
    
    void initialize();
    
    void generateChunk(Chunk& chunk);

    void generateChunk(Chunk& chunk, const cgp::vec3& position);
    
    int generateHeightAt(float worldX, float worldZ);
    BlockType getBlockTypeAt(float worldX, float worldY, float worldZ, int surfaceHeight);

    float generateTemperatureAt(float worldX, float worldZ);
    float generateHumidityAt(float worldX, float worldZ);

    BiomeType getBiomeTypeAt(float worldX, float worldZ);

    void setNoiseScale(const cgp::vec2& scale);
    void setNoiseOffset(const cgp::vec2& offset);

    cgp::vec2 getNoiseScale() const;
    cgp::vec2 getNoiseOffset() const;
};

