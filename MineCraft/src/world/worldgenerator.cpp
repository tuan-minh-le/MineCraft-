#include "worldgenerator.hpp"


WorldGenerator::WorldGenerator() {
    noise.SetSeed(1337);
    noise.SetFrequency(1.f);
}

WorldGenerator::~WorldGenerator() {}

void WorldGenerator::initialize() {}

void WorldGenerator::generateChunk(Chunk& chunk) {
    ChunkSize size = chunk.getSize();
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
                
                // Only set blocks that are within this chunk
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
    noise.SetNoiseType(FastNoise::Perlin);
    noise.SetFractalOctaves(4.f);
    noise.SetFractalLacunarity(2.1f);
    noise.SetFractalGain(0.8f);

    float perlinCoord_x = noiseOffset.x + worldX * noiseScale.x;
    float perlinCoord_z = noiseOffset.y + worldZ * noiseScale.y;
    
    float noiseValue = noise.GetPerlinFractal(perlinCoord_x, perlinCoord_z);
    int height = static_cast<int>(HEIGHTOFFSET + HEIGHTINTENSITY * noiseValue);
    
    return std::max(0, height); 
}

BlockType WorldGenerator::getBlockTypeAt(float worldX, float worldY, float worldZ, int surfaceHeight) {
    int blockY = static_cast<int>(worldY);
    
    if(blockY > surfaceHeight) {
        return AIR;
    }
    else if(blockY <= surfaceHeight && blockY >= surfaceHeight - 3) {
        return GRASS; 
    }
    else if(blockY <= surfaceHeight - 3 && blockY >= surfaceHeight - 7) {
        return SAND; 
    }
    else if(blockY > 0) {
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