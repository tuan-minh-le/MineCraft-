#include "vegetation.hpp"
#include <random>
#include <iostream>

VegetationStructure::VegetationStructure(const cgp::vec3& pos, VegetationType vegType) 
    : basePosition(pos), type(vegType) {
    generateStructure();
}

void VegetationStructure::generateStructure() {
    blocks.clear();
    
    switch(type) {
        case TREE_OAK:
            generateOakTree();
            break;
        case TREE_PINE:
            generatePineTree();
            break;
        case CACTUS_T:
            generateCactus();
            break;
        case DEAD_BUSH:
            generateDeadBush();
            break;
    }
    
    std::cout << "Generated " << blocks.size() << " blocks for vegetation type " << type 
              << " at position (" << basePosition.x << ", " << basePosition.y << ", " << basePosition.z << ")" << std::endl;
}

void VegetationStructure::generateOakTree() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> heightDist(3, 5);
    
    int trunkHeight = heightDist(gen);
    
    for (int y = 0; y < trunkHeight; ++y) {
        cgp::vec3 trunkPos = basePosition + cgp::vec3(0, y, 0);
        blocks.push_back({trunkPos, WOOD});
    }
    
    cgp::vec3 leafBase = basePosition + cgp::vec3(0, trunkHeight, 0);
    
    // 3x3 leaf layer on top
    for (int x = -1; x <= 1; ++x) {
        for (int z = -1; z <= 1; ++z) {
            if (x == 0 && z == 0) continue; // Skip center (trunk position)
            cgp::vec3 leafPos = leafBase + cgp::vec3(x, 0, z);
            blocks.push_back({leafPos, LEAF});
        }
    }
    
    for (int x = -1; x <= 1; ++x) {
        for (int z = -1; z <= 1; ++z) {
            // Skip corners for natural look
            if ((x == -1 || x == 1) && (z == -1 || z == 1)) continue;
            cgp::vec3 leafPos = leafBase + cgp::vec3(x, 1, z);
            blocks.push_back({leafPos, LEAF});
        }
    }
}

void VegetationStructure::generatePineTree() {
    // Pine tree: taller trunk with triangular leaf pattern
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> heightDist(5, 8);
    
    int trunkHeight = heightDist(gen);
    
    // Generate trunk
    for (int y = 0; y < trunkHeight; ++y) {
        cgp::vec3 trunkPos = basePosition + cgp::vec3(0, y, 0);
        blocks.push_back({trunkPos, WOOD}); // Using craft table as wood
    }
    
    cgp::vec3 leafBase = basePosition + cgp::vec3(0, trunkHeight - 2, 0);
    
    for (int x = -1; x <= 1; ++x) {
        for (int z = -1; z <= 1; ++z) {
            if (x == 0 && z == 0) continue;
            cgp::vec3 leafPos = leafBase + cgp::vec3(x, 0, z);
            blocks.push_back({leafPos, LEAF});
        }
    }
    
    for (int x = -1; x <= 1; ++x) {
        for (int z = -1; z <= 1; ++z) {
            if ((x == -1 || x == 1) && (z == -1 || z == 1)) continue;
            cgp::vec3 leafPos = leafBase + cgp::vec3(x, 1, z);
            blocks.push_back({leafPos, LEAF});
        }
    }
    
    // Top leaf (single block)
    cgp::vec3 topLeafPos = leafBase + cgp::vec3(0, 2, 0);
    blocks.push_back({topLeafPos, LEAF});
}

void VegetationStructure::generateCactus() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> heightDist(2, 4);
    
    int cactusHeight = heightDist(gen);
    
    // Generate cactus blocks
    for (int y = 0; y < cactusHeight; ++y) {
        cgp::vec3 cactusPos = basePosition + cgp::vec3(0, y, 0);
        blocks.push_back({cactusPos, CACTUS}); // Using sand blocks for cactus
    }
    
    // Add side arms occasionally
    if (cactusHeight >= 3) {
        std::uniform_int_distribution<int> armChance(0, 100);
        if (armChance(gen) < 40) { 
            int armHeight = cactusHeight - 1;
            std::uniform_int_distribution<int> sideDist(0, 3); 
            int side = sideDist(gen);
            
            cgp::vec3 armOffset;
            switch(side) {
                case 0: armOffset = cgp::vec3(0, 0, 1); break;  // North
                case 1: armOffset = cgp::vec3(1, 0, 0); break;  // East
                case 2: armOffset = cgp::vec3(0, 0, -1); break; // South
                case 3: armOffset = cgp::vec3(-1, 0, 0); break; // West
            }
            
            cgp::vec3 armPos = basePosition + cgp::vec3(0, armHeight, 0) + armOffset;
            blocks.push_back({armPos, CACTUS});
        }
    }
}

void VegetationStructure::generateDeadBush() {
    // Dead bush: single block or small structure
    blocks.push_back({basePosition, DIRT}); // Using dirt as dead bush placeholder
    
    // Sometimes add a second block on top
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> extraBlock(0, 100);
    
    if (extraBlock(gen) < 30) { // 30% chance for two-block bush
        cgp::vec3 topPos = basePosition + cgp::vec3(0, 1, 0);
        blocks.push_back({topPos, DIRT});
    }
}