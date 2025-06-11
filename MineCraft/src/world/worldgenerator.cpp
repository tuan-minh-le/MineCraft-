#include "worldgenerator.hpp"
#include "chunk.hpp"


WorldGenerator::WorldGenerator() {
}

WorldGenerator::~WorldGenerator() {}

void WorldGenerator::initialize() {}

void WorldGenerator::generateChunk(Chunk& chunk) {
    ChunkSize size = chunk.getSize();
    cgp::vec3 chunkWorldPos = chunk.getWorldPosition();
    for(int x = 0; x < size.width; x++) {
        for(int z = 0; z < size.depth; z++) {
            float worldX = chunkWorldPos.x + x;
            float worldZ = chunkWorldPos.z + z;
            
            int surfaceHeight = generateHeightAt(worldX, worldZ);
            
            // Fill the column from bottom to surface height
            for(int y = 0; y < size.height; y++) {
                float worldY = chunkWorldPos.y + y;
                
                BlockType blockType = getBlockTypeAt(worldX, worldY, worldZ, surfaceHeight);
                
                if(worldY >= chunkWorldPos.y && worldY < chunkWorldPos.y + size.height) {
                    chunk.setBlock(x, y, z, blockType);
                }
            }
        }
    }
    
    chunk.markAsGenerated();
}

void WorldGenerator::generateChunk(Chunk& chunk, const cgp::vec3& position) {
    ChunkSize size = chunk.getSize();
    chunk.setWorldPosition(position);
    cgp::vec3 chunkWorldPos = chunk.getWorldPosition();
    
    for(int x = 0; x < size.width; x++) {
        for(int z = 0; z < size.depth; z++) {
            // Convert local chunk coordinates to world coordinates
            float worldX = chunkWorldPos.x + x;
            float worldZ = chunkWorldPos.z + z;
            
            // Generate height using Perlin noise
            int surfaceHeight = generateHeightAt(worldX, worldZ);
            
            // Fill the column from bottom to surface height
            for(int y = 0; y < size.height; y++) {
                float worldY = chunkWorldPos.y + y;
                
                // Determine block type based on height and position
                BlockType blockType = getBlockTypeAt(worldX, worldY, worldZ, surfaceHeight);
                
                if(worldY >= chunkWorldPos.y && worldY < chunkWorldPos.y + size.height) {
                    chunk.setBlock(x, y, z, blockType);
                }
            }
        }
    }
    
    chunk.markAsGenerated();
}

int WorldGenerator::generateHeightAt(float worldX, float worldZ) {
    noise.SetSeed(1337);
    noise.SetNoiseType(FastNoise::PerlinFractal);
    
    noise.SetFrequency(0.5f);
    noise.SetFractalOctaves(3.f);
    noise.SetFractalLacunarity(2.1f);
    noise.SetFractalGain(0.8f);

    float perlinCoord_x = noiseOffset.x + worldX * noiseScale.x;
    float perlinCoord_z = noiseOffset.y + worldZ * noiseScale.y;
    
    float noiseValue = noise.GetPerlinFractal(perlinCoord_x, perlinCoord_z);
    noiseValue = std::pow(noiseValue, 4);
    int height = static_cast<int>(HEIGHTOFFSET + HEIGHTINTENSITY * noiseValue);
    
    return std::max(0, height); 
}

float WorldGenerator::generateTemperatureAt(float worldX, float worldZ){
    noise.SetSeed(1338);
    noise.SetNoiseType(FastNoise::Perlin);

    noise.SetFrequency(1.f);
    
    float perlinCoord_x = temperatureOffset.x + worldX * temperatureScale.x;
    float perlinCoord_z = temperatureOffset.y + worldZ * temperatureScale.y;

    return noise.GetPerlin(perlinCoord_x, perlinCoord_z);
}

float WorldGenerator::generateHumidityAt(float worldX, float worldZ){
    noise.SetSeed(1339);
    noise.SetNoiseType(FastNoise::Perlin);

    noise.SetFrequency(2.f);

    float perlinCoord_x = humidityOffset.x + worldX * humidityScale.x;
    float perlinCoord_z = humidityOffset.y + worldZ * humidityScale.y;

    return noise.GetPerlin(perlinCoord_x, perlinCoord_z);
}

BiomeType WorldGenerator::getBiomeTypeAt(float worldX, float worldZ){
    float temperature = generateTemperatureAt(worldX, worldZ);
    float humidity = generateHumidityAt(worldX, worldZ);

    // Debug: Print first 20 temperature values to see the range
    // static int debugCount = 0;
    // if(debugCount % 10000 == 0) {
    //     std::cout << "Debug " << debugCount << ": pos(" << worldX << "," << worldZ 
    //               << ") temp=" << temperature << " humidity=" << humidity << std::endl;
        
    // }

    // debugCount++;

    BiomeType result;
    if(temperature < 0.01f){
        result = SNOWBIOME;
    }
    else if(temperature > 0.2f){
        result = DESERT;
    }
    else{
        result = PLAINS;
    }

    return result;
}


BlockType WorldGenerator::getBlockTypeAt(float worldX, float worldY, float worldZ, int surfaceHeight) {
    int blockY = static_cast<int>(worldY);
    BiomeType biome = getBiomeTypeAt(worldX, worldZ);
    
    if(blockY > surfaceHeight) {
        return AIR;
    }
    
    // Surface block based on biome
    if(blockY == surfaceHeight) {
        switch(biome) {
            case DESERT:
                return SAND;
            case PLAINS:
                return GRASS; // Could be mud block
            case SNOWBIOME:
                return SNOW; // Could be snow
            default:
                return GRASS;
        }
    }
    
    int depth = surfaceHeight - blockY;
    
    if (depth <= 3) {
        switch(biome) {
            case DESERT:
                return SAND;
            case PLAINS:
                return GRASS;
            default:
                return GRASS;
        }
    }
    else if (depth <= 7) {
        return (biome == DESERT) ? SAND : STONE;
    }
    else if (blockY > 3) {
        return STONE;
    }
    else {
        return BEDROCK;
    }
}

void WorldGenerator::setNoiseScale(const cgp::vec2& scale){
    noiseScale = scale;
}

void WorldGenerator::setNoiseOffset(const cgp::vec2& offset){
    noiseOffset = offset;
}

cgp::vec2 WorldGenerator::getNoiseScale() const{
    return noiseScale;
}

cgp::vec2 WorldGenerator::getNoiseOffset() const{
    return noiseOffset;
}