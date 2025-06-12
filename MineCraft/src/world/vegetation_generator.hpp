#pragma once
#include "cgp/cgp.hpp"
#include "vegetation.hpp"
#include "world/worldgenerator.hpp"
#include "world/chunk.hpp"
#include <vector>
#include <random>
#include <memory>
#include <map>

// Forward declarations
class World;
class Chunk;

class VegetationGenerator {
private:
    // Bridson algorithm parameters for each biome
    struct BiomeVegetationConfig {
        float minDistance;      // 'r' parameter
        int maxAttempts;        // 'k' parameter
        float density;          // Probability of spawning vegetation
        std::vector<VegetationType> vegetationTypes;
        std::vector<float> typeWeights; // Probability weights for each type
    };

    std::map<BiomeType, BiomeVegetationConfig> biomeConfigs;
    WorldGenerator* worldGenerator;
    
    // Bridson grid for spatial distribution
    float cellSize;
    int gridWidth, gridHeight;
    std::vector<std::vector<std::vector<cgp::vec2>>> bridsonGrid;
    std::vector<cgp::vec2> activePoints;
    
    std::mt19937 rng;
    
public:
    VegetationGenerator(WorldGenerator* wg);
    ~VegetationGenerator() = default;
    
    // Main generation function - now places blocks directly into chunk
    void generateVegetationForChunk(Chunk& chunk, const cgp::vec3& chunkWorldPos);
    
    // Alternative function that returns structures for manual placement
    std::vector<VegetationStructure> generateVegetationStructures(
        const cgp::vec3& chunkWorldPos, 
        const ChunkSize& chunkSize);
    
private:
    // Configuration
    void initializeBiomeConfigs();
    
    // Bridson algorithm implementation
    void initializeBridsonGrid(const cgp::vec3& chunkWorldPos, const ChunkSize& chunkSize, float minDistance);
    void generateVegetationBridson(
        const cgp::vec3& chunkWorldPos,
        const ChunkSize& chunkSize,
        const BiomeVegetationConfig& config,
        std::vector<VegetationStructure>& vegetation);
    
    // Helper functions
    cgp::vec2 generateRandomPointAround(const cgp::vec2& point, float minDistance);
    bool isValidPoint(const cgp::vec2& point, float minDistance);
    void addToBridsonGrid(const cgp::vec2& point);
    bool shouldSpawnVegetation(const cgp::vec3& worldPos, const BiomeVegetationConfig& config);
    void createVegetationAt(
        const cgp::vec3& worldPos,
        const BiomeVegetationConfig& config,
        std::vector<VegetationStructure>& vegetation);
    VegetationType selectVegetationType(const BiomeVegetationConfig& config);
    
    // Block placement helpers
    void placeVegetationInChunk(Chunk& chunk, const VegetationStructure& vegetation, const cgp::vec3& chunkWorldPos);
    bool isPositionInChunk(const cgp::vec3& worldPos, const cgp::vec3& chunkWorldPos, const ChunkSize& chunkSize);
};