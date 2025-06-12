#pragma once
#include "cgp/cgp.hpp"
#include "block/block.hpp"
#include <vector>

// Vegetation types based on biomes
enum VegetationType {
    TREE_OAK = 0,      // Plains biome - stacked wood blocks
    TREE_PINE = 1,     // Snow biome - taller stacked wood blocks
    CACTUS_T = 2,        // Desert biome - stacked cactus blocks
    DEAD_BUSH = 3      // Desert biome - single block bush
};

// Simple structure to represent vegetation as stacked blocks
struct VegetationStructure {
    cgp::vec3 basePosition;
    VegetationType type;
    std::vector<std::pair<cgp::vec3, BlockType>> blocks; // Position and block type pairs
    
    VegetationStructure(const cgp::vec3& pos, VegetationType vegType);
    void generateStructure();
    
private:
    void generateOakTree();
    void generatePineTree();
    void generateCactus();
    void generateDeadBush();
};