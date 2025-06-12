#include "vegetation_generator.hpp"
#include "world/world.hpp"
#include <cmath>
#include <iostream>

VegetationGenerator::VegetationGenerator(WorldGenerator* wg) 
    : worldGenerator(wg), cellSize(0.0f), gridWidth(0), gridHeight(0), rng(std::random_device{}()) {
    initializeBiomeConfigs();
}

void VegetationGenerator::initializeBiomeConfigs() {
    // Plains biome - moderate tree density
    biomeConfigs[PLAINS] = {
        .minDistance = 8.0f,
        .maxAttempts = 30,
        .density = 0.3f,
        .vegetationTypes = {TREE_OAK},
        .typeWeights = {1.0f}
    };
    
    // Snow biome - sparse pine trees
    biomeConfigs[SNOWBIOME] = {
        .minDistance = 12.0f,
        .maxAttempts = 25,
        .density = 0.15f,
        .vegetationTypes = {TREE_PINE},
        .typeWeights = {1.0f}
    };
    
    // Desert biome - very sparse cacti and dead bushes
    biomeConfigs[DESERT] = {
        .minDistance = 15.0f,
        .maxAttempts = 20,
        .density = 0.08f,
        .vegetationTypes = {CACTUS_T, DEAD_BUSH},
        .typeWeights = {0.3f, 0.7f}
    };
}

void VegetationGenerator::generateVegetationForChunk(Chunk& chunk, const cgp::vec3& chunkWorldPos) {
    ChunkSize chunkSize = chunk.getSize();
    
    // Generate vegetation structures
    std::vector<VegetationStructure> vegetation = generateVegetationStructures(chunkWorldPos, chunkSize);
    
    // Place each vegetation structure's blocks into the chunk
    for (const auto& veg : vegetation) {
        placeVegetationInChunk(chunk, veg, chunkWorldPos);
    }
    
    std::cout << "Placed " << vegetation.size() << " vegetation structures in chunk at (" 
              << chunkWorldPos.x << ", " << chunkWorldPos.z << ")" << std::endl;
}

std::vector<VegetationStructure> VegetationGenerator::generateVegetationStructures(
    const cgp::vec3& chunkWorldPos, 
    const ChunkSize& chunkSize) {
    
    std::vector<VegetationStructure> vegetation;
    
    // Get the dominant biome for this chunk
    float centerX = chunkWorldPos.x + chunkSize.width / 2.0f;
    float centerZ = chunkWorldPos.z + chunkSize.depth / 2.0f;
    BiomeType biome = worldGenerator->getBiomeTypeAt(centerX, centerZ);
    
    // Skip vegetation generation for biomes without configuration
    if (biomeConfigs.find(biome) == biomeConfigs.end()) {
        return vegetation;
    }
    
    const BiomeVegetationConfig& config = biomeConfigs[biome];
    
    // Initialize Bridson grid
    initializeBridsonGrid(chunkWorldPos, chunkSize, config.minDistance);
    
    // Generate vegetation using Bridson algorithm
    generateVegetationBridson(chunkWorldPos, chunkSize, config, vegetation);
    
    std::cout << "Generated " << vegetation.size() << " vegetation structures for " 
              << "biome " << biome << " at chunk (" << chunkWorldPos.x << ", " << chunkWorldPos.z << ")" << std::endl;
    
    return vegetation;
}

void VegetationGenerator::initializeBridsonGrid(const cgp::vec3& chunkWorldPos, const ChunkSize& chunkSize, float minDistance) {
    cellSize = minDistance / std::sqrt(2.0f);
    gridWidth = static_cast<int>(std::ceil(chunkSize.width / cellSize));
    gridHeight = static_cast<int>(std::ceil(chunkSize.depth / cellSize));
    
    bridsonGrid.clear();
    bridsonGrid.resize(gridWidth, std::vector<std::vector<cgp::vec2>>(gridHeight));
    activePoints.clear();
}

void VegetationGenerator::generateVegetationBridson(
    const cgp::vec3& chunkWorldPos,
    const ChunkSize& chunkSize,
    const BiomeVegetationConfig& config,
    std::vector<VegetationStructure>& vegetation) {
    
    // Place first point randomly in chunk
    std::uniform_real_distribution<float> xDist(0, static_cast<float>(chunkSize.width));
    std::uniform_real_distribution<float> zDist(0, static_cast<float>(chunkSize.depth));
    
    cgp::vec2 firstPoint(xDist(rng), zDist(rng));
    activePoints.push_back(firstPoint);
    addToBridsonGrid(firstPoint);
    
    // FIX: Calculate proper world position with correct surface height
    float worldX = chunkWorldPos.x + firstPoint.x;
    float worldZ = chunkWorldPos.z + firstPoint.y;
    int surfaceHeight = worldGenerator->generateHeightAt(worldX, worldZ);
    cgp::vec3 firstWorldPos(worldX, static_cast<float>(surfaceHeight), worldZ);
    
    if (shouldSpawnVegetation(firstWorldPos, config)) {
        createVegetationAt(firstWorldPos, config, vegetation);
    }
    
    // Bridson algorithm main loop
    while (!activePoints.empty()) {
        std::uniform_int_distribution<int> indexDist(0, static_cast<int>(activePoints.size()) - 1);
        int randomIndex = indexDist(rng);
        cgp::vec2 currentPoint = activePoints[randomIndex];
        
        bool foundNewPoint = false;
        
        for (int attempt = 0; attempt < config.maxAttempts; ++attempt) {
            cgp::vec2 newPoint = generateRandomPointAround(currentPoint, config.minDistance);
            
            // Check if point is within chunk bounds
            if (newPoint.x < 0 || newPoint.x >= chunkSize.width ||
                newPoint.y < 0 || newPoint.y >= chunkSize.depth) {
                continue;
            }
            
            // Check if point is valid (far enough from existing points)
            if (isValidPoint(newPoint, config.minDistance)) {
                activePoints.push_back(newPoint);
                addToBridsonGrid(newPoint);
                
                // FIX: Calculate proper world position with surface height
                float newWorldX = chunkWorldPos.x + newPoint.x;
                float newWorldZ = chunkWorldPos.z + newPoint.y;
                int newSurfaceHeight = worldGenerator->generateHeightAt(newWorldX, newWorldZ);
                cgp::vec3 worldPos(newWorldX, static_cast<float>(newSurfaceHeight), newWorldZ);
                
                if (shouldSpawnVegetation(worldPos, config)) {
                    createVegetationAt(worldPos, config, vegetation);
                }
                
                foundNewPoint = true;
                break;
            }
        }
        
        if (!foundNewPoint) {
            activePoints.erase(activePoints.begin() + randomIndex);
        }
    }
}


cgp::vec2 VegetationGenerator::generateRandomPointAround(const cgp::vec2& point, float minDistance) {
    std::uniform_real_distribution<float> angleDist(0, 2 * M_PI);
    std::uniform_real_distribution<float> radiusDist(minDistance, 2 * minDistance);
    
    float angle = angleDist(rng);
    float radius = radiusDist(rng);
    
    return cgp::vec2(
        point.x + radius * std::cos(angle),
        point.y + radius * std::sin(angle)
    );
}

bool VegetationGenerator::isValidPoint(const cgp::vec2& point, float minDistance) {
    int gridX = static_cast<int>(point.x / cellSize);
    int gridY = static_cast<int>(point.y / cellSize);
    
    // Check neighboring grid cells
    for (int x = std::max(0, gridX - 2); x <= std::min(gridWidth - 1, gridX + 2); ++x) {
        for (int y = std::max(0, gridY - 2); y <= std::min(gridHeight - 1, gridY + 2); ++y) {
            for (const cgp::vec2& existingPoint : bridsonGrid[x][y]) {
                float distance = cgp::norm(point - existingPoint);
                if (distance < minDistance) {
                    return false;
                }
            }
        }
    }
    return true;
}

void VegetationGenerator::addToBridsonGrid(const cgp::vec2& point) {
    int gridX = static_cast<int>(point.x / cellSize);
    int gridY = static_cast<int>(point.y / cellSize);
    
    if (gridX >= 0 && gridX < gridWidth && gridY >= 0 && gridY < gridHeight) {
        bridsonGrid[gridX][gridY].push_back(point);
    }
}

bool VegetationGenerator::shouldSpawnVegetation(const cgp::vec3& worldPos, const BiomeVegetationConfig& config) {
    // Get surface height for this position
    int surfaceHeight = worldGenerator->generateHeightAt(worldPos.x, worldPos.z);
    
    // FIX: Since we're already passing the surface position, check if we're close to it
    if (std::abs(worldPos.y - surfaceHeight) > 1.0f) {
        return false;
    }
    
    // Check biome-specific surface conditions
    BiomeType biome = worldGenerator->getBiomeTypeAt(worldPos.x, worldPos.z);
    BlockType surfaceBlock = worldGenerator->getBlockTypeAt(worldPos.x, surfaceHeight, worldPos.z, surfaceHeight);
    
    // Biome-specific surface requirements
    switch (biome) {
        case PLAINS:
            if (surfaceBlock != GRASS) return false;
            break;
        case SNOWBIOME:
            if (surfaceBlock != SNOW && surfaceBlock != GRASS) return false;
            break;
        case DESERT:
            if (surfaceBlock != SAND) return false;
            break;
        default:
            return false;
    }
    
    // Random density check
    std::uniform_real_distribution<float> densityDist(0.0f, 1.0f);
    return densityDist(rng) < config.density;
}

void VegetationGenerator::createVegetationAt(
    const cgp::vec3& worldPos,
    const BiomeVegetationConfig& config,
    std::vector<VegetationStructure>& vegetation) {
    
    // Select vegetation type based on weights
    VegetationType selectedType = selectVegetationType(config);
    
    // Get accurate surface height
    int surfaceHeight = worldGenerator->generateHeightAt(worldPos.x, worldPos.z);
    cgp::vec3 spawnPos(worldPos.x, static_cast<float>(surfaceHeight + 1), worldPos.z);
    
    // Create vegetation structure
    vegetation.emplace_back(spawnPos, selectedType);
}

VegetationType VegetationGenerator::selectVegetationType(const BiomeVegetationConfig& config) {
    if (config.vegetationTypes.empty()) {
        return TREE_OAK; // Fallback
    }
    
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    float random = dist(rng);
    
    float totalWeight = 0.0f;
    for (float weight : config.typeWeights) {
        totalWeight += weight;
    }
    
    if (totalWeight == 0.0f) {
        return config.vegetationTypes[0]; // Fallback to first type
    }
    
    float normalizedRandom = random * totalWeight;
    float cumulativeWeight = 0.0f;
    
    for (size_t i = 0; i < config.vegetationTypes.size(); ++i) {
        cumulativeWeight += config.typeWeights[i];
        if (normalizedRandom <= cumulativeWeight) {
            return config.vegetationTypes[i];
        }
    }
    
    // Fallback to last type
    return config.vegetationTypes.back();
}

void VegetationGenerator::placeVegetationInChunk(Chunk& chunk, const VegetationStructure& vegetation, const cgp::vec3& chunkWorldPos) {
    ChunkSize chunkSize = chunk.getSize();
    
    for (const auto& blockPair : vegetation.blocks) {
        cgp::vec3 worldPos = blockPair.first;
        BlockType blockType = blockPair.second;
        
        // Convert world position to local chunk coordinates
        cgp::vec3 localPos = chunk.worldToLocal(worldPos);
        
        // Check if the position is within this chunk
        if (isPositionInChunk(worldPos, chunkWorldPos, chunkSize)) {
            int x = static_cast<int>(localPos.x);
            int y = static_cast<int>(localPos.y);
            int z = static_cast<int>(localPos.z);
            
            // Ensure coordinates are within chunk bounds
            if (x >= 0 && x < chunkSize.width && 
                y >= 0 && y < chunkSize.height && 
                z >= 0 && z < chunkSize.depth) {
                
                // Only place vegetation if the spot is currently air
                if (chunk.getBlock(x, y, z) == AIR) {
                    chunk.setBlock(x, y, z, blockType);
                }
            }
        }
    }
}

bool VegetationGenerator::isPositionInChunk(const cgp::vec3& worldPos, const cgp::vec3& chunkWorldPos, const ChunkSize& chunkSize) {
    return worldPos.x >= chunkWorldPos.x && worldPos.x < chunkWorldPos.x + chunkSize.width &&
           worldPos.z >= chunkWorldPos.z && worldPos.z < chunkWorldPos.z + chunkSize.depth;
}