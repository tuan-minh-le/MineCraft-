#include "world.hpp"

World::World():worldSizeX(0), worldSizeZ(0), renderDistance(0){}


World::~World(){
    std::cout << "World destructor called - deleting " << chunks.size() << " chunks" << std::endl;
    for(Chunk* chunk : chunks){
        delete chunk;
    }
    std::cout << "World destructor completed" << std::endl;
}

void World::initialize(int pWorldSizeX, int pWorldSizeZ, int pRenderDistance){
    worldSizeX = pWorldSizeX;
    worldSizeZ = pWorldSizeZ;
    renderDistance = pRenderDistance;
    
    worldGenerator.initialize();
    generateWorld();
    // std::cout << "Initialized world with - Size: " << worldSizeX << 
    //                 "x " << worldSizeZ << std::endl;
}

void World::generateWorld(){
    chunks.clear();

    // std::cout << "\n=== GENERATING WORLD ===" << std::endl;
    // std::cout << "World size: " << worldSizeX << " x " << worldSizeZ << std::endl;
    // std::cout << "Chunk size: " << dummyChunk.getSize().width << " x " << dummyChunk.getSize().height << " x " << dummyChunk.getSize().depth << std::endl;

    for(int x = 0; x < worldSizeX; x++){
        for(int z = 0; z < worldSizeZ; z++){
            cgp::vec3 chunkWorldPos = {x * dummyChunk.getSize().width, 0, z * dummyChunk.getSize().depth};
            
            // std::cout << "Creating chunk [" << x << "," << z << "] at world position: (" 
                    //   << chunkWorldPos.x << ", " << chunkWorldPos.y << ", " << chunkWorldPos.z << ")" << std::endl;
            
            Chunk* chunk = new Chunk(chunkWorldPos);
            
            chunk->initialize();

            worldGenerator.generateChunk(*chunk, chunkWorldPos);

            chunk->populateBlockObjects();
            chunk->markAsGenerated();

            chunks.push_back(chunk);
        }
    }
    
    // std::cout << "Generated " << chunks.size() << " chunks total" << std::endl;
    
    // Test chunk lookup immediately after generation
    // std::cout << "\n=== TESTING CHUNK LOOKUP ===" << std::endl;
    std::vector<cgp::vec3> testPositions = {
        {0, 0, 0},
        {5, 5, 5},
        {15, 0, 15},
        {8, 8, 8}
    };
    
    // for(const auto& pos : testPositions) {
    //     std::cout << "\nTesting position (" << pos.x << ", " << pos.y << ", " << pos.z << "):" << std::endl;
    //     Chunk* foundChunk = getChunkAt(pos);
    //     if(foundChunk) {
    //         cgp::vec3 chunkPos = foundChunk->getWorldPosition();
    //         std::cout << "Found in chunk at (" << chunkPos.x << ", " << chunkPos.y << ", " << chunkPos.z << ")" << std::endl;
            
    //         // Test getting a block from this chunk
    //         cgp::vec3 localPos = foundChunk->worldToLocal(pos);
    //         int localX = static_cast<int>(localPos.x);
    //         int localY = static_cast<int>(localPos.y);
    //         int localZ = static_cast<int>(localPos.z);
    //         BlockType block = foundChunk->getBlock(localX, localY, localZ);
    //         std::cout << "Block at local (" << localX << "," << localY << "," << localZ << "): " << static_cast<int>(block) << std::endl;
    //     } else {
    //         std::cout << "NO CHUNK FOUND!" << std::endl;
    //     }
    // }
}

void World::renderBasic(const cgp::vec3& position, const cgp::environment_generic_structure& environment){
    for(const auto& chunk : chunks){
        cgp::vec3 chunkCenter = chunk->getChunkCenter();

        float distance = cgp::norm(position - chunkCenter);
        if(distance < renderDistance){
            chunk->renderCached(environment);
        }
    }
}

void World::renderCached(const cgp::vec3& position, const cgp::environment_generic_structure& environment){
    for(const auto& chunk : chunks){
        if(chunk && chunk->isGenerated()){
            cgp::vec3 chunkCenter = chunk->getChunkCenter();

            float distance = cgp::norm(position - chunkCenter);

            if(distance < renderDistance){
                chunk->renderCached(environment);
            }
        }
    }
}


void World::setBlock(const cgp::vec3& worldPos, BlockType blockType) {
    Chunk* chunk = getChunkAt(worldPos);
    if(chunk) {
        cgp::vec3 localPos = chunk->worldToLocal(worldPos);
        chunk->setBlock(
            static_cast<int>(localPos.x), 
            static_cast<int>(localPos.y), 
            static_cast<int>(localPos.z), 
            blockType
        );
    } else {
        // std::cout << "setBlock: No chunk found for position (" << worldPos.x << ", " << worldPos.y << ", " << worldPos.z << ")" << std::endl;
    }
}

Block* World::getBlockObject(const cgp::vec3& worldPos) const {
    Chunk* chunk = getChunkAt(worldPos);
    if(chunk) {
        cgp::vec3 localPos = chunk->worldToLocal(worldPos);
        return chunk->getBlockObject(
            static_cast<int>(localPos.x), 
            static_cast<int>(localPos.y), 
            static_cast<int>(localPos.z)
        );
    }
    // std::cout << "getBlockObject: No chunk found for position (" << worldPos.x << ", " << worldPos.y << ", " << worldPos.z << ")" << std::endl;
    return nullptr;
}

BlockType World::getBlock(const cgp::vec3& worldPos) const {
    // std::cout << "getBlock called for position (" << worldPos.x << ", " << worldPos.y << ", " << worldPos.z << ")" << std::endl;
    
    Chunk* chunk = getChunkAt(worldPos);
    if(chunk) {
        cgp::vec3 localPos = chunk->worldToLocal(worldPos);
        // std::cout << "Found chunk, local position: (" << localPos.x << ", " << localPos.y << ", " << localPos.z << ")" << std::endl;
        
        int localX = static_cast<int>(localPos.x);
        int localY = static_cast<int>(localPos.y);
        int localZ = static_cast<int>(localPos.z);
        
        // std::cout << "Integer coordinates: (" << localX << ", " << localY << ", " << localZ << ")" << std::endl;
        
        BlockType result = chunk->getBlock(localX, localY, localZ);
        // std::cout << "Block type: " << static_cast<int>(result) << std::endl;
        
        return result;
    }
    // std::cout << "getBlock: No chunk found for position (" << worldPos.x << ", " << worldPos.y << ", " << worldPos.z << "), defaulting to AIR" << std::endl;
    return AIR;
}

int World::getRenderDistance() const{
    return renderDistance;
}

Chunk* World::getChunkByIndex(size_t index) const {
    if(index < chunks.size()) {
        return chunks[index];
    }
    return nullptr;
}

size_t World::getChunkCount() const {
    return chunks.size();
}

void World::debugFirstChunk() const {
    // std::cout << "\n=== DEBUGGING FIRST CHUNK ===" << std::endl;
    // if(chunks.empty()) {
    //     std::cout << "No chunks exist!" << std::endl;
    //     return;
    // }
    
    // Chunk* firstChunk = chunks[0];
    // if(!firstChunk) {
    //     std::cout << "First chunk is nullptr!" << std::endl;
    //     return;
    // }
    
    // std::cout << "First chunk exists at world position: (" 
    //           << firstChunk->getWorldPosition().x << ", " 
    //           << firstChunk->getWorldPosition().y << ", " 
    //           << firstChunk->getWorldPosition().z << ")" << std::endl;
    
    // // Test some coordinates in the first chunk
    // std::cout << "Testing coordinates in first chunk:" << std::endl;
    // for(int y = 0; y < 10; y++) {
    //     for(int x = 0; x < 3; x++) {
    //         for(int z = 0; z < 3; z++) {
    //             BlockType block = firstChunk->getBlock(x, y, z);
    //             if(block != AIR) {
    //                 std::cout << "Found block type " << static_cast<int>(block) 
    //                           << " at (" << x << "," << y << "," << z << ")" << std::endl;
    //             }
    //         }
    //     }
    // }
    
    // // Call the chunk's debug method
    // firstChunk->debugChunkContents();
}

void World::debugWorldState() const {
//     std::cout << "\n=== WORLD STATE DEBUG ===" << std::endl;
//     std::cout << "World size: " << worldSizeX << " x " << worldSizeZ << std::endl;
//     std::cout << "Render distance: " << renderDistance << std::endl;
//     std::cout << "Chunks vector size: " << chunks.size() << std::endl;
//     std::cout << "Chunks vector capacity: " << chunks.capacity() << std::endl;
    
//     int validChunks = 0;
//     int nullChunks = 0;
    
//     for(size_t i = 0; i < chunks.size(); i++) {
//         if(chunks[i] == nullptr) {
//             nullChunks++;
//         } else {
//             validChunks++;
//         }
//     }
    
//     std::cout << "Valid chunks: " << validChunks << std::endl;
//     std::cout << "Null chunks: " << nullChunks << std::endl;
//     std::cout << "=========================" << std::endl;
// }

// void World::checkChunkIntegrity() const {
//     std::cout << "\n=== CHUNK INTEGRITY CHECK ===" << std::endl;
    
//     if(chunks.empty()) {
//         std::cout << "ERROR: Chunks vector is empty!" << std::endl;
//         return;
//     }
    
//     std::cout << "Checking first 5 chunks:" << std::endl;
//     for(size_t i = 0; i < std::min(size_t(5), chunks.size()); i++) {
//         if(chunks[i] == nullptr) {
//             std::cout << "Chunk " << i << ": NULL POINTER" << std::endl;
//         } else {
//             cgp::vec3 pos = chunks[i]->getWorldPosition();
//             std::cout << "Chunk " << i << ": Valid at (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
//         }
//     }
//     std::cout << "=============================" << std::endl;
}

// Modified getChunkAt with better debugging:
Chunk* World::getChunkAt(const cgp::vec3& worldPos) const {
    static int callCount = 0;
    callCount++;
    
    // Only show detailed debug for first few calls or when chunks is empty
    bool showDetailed = (callCount <= 3) || chunks.empty();
    
    if(showDetailed) {
        // std::cout << "\n=== getChunkAt DEBUG (call " << callCount << ") ===" << std::endl;
        // std::cout << "Looking for world position: (" << worldPos.x << ", " << worldPos.y << ", " << worldPos.z << ")" << std::endl;
        // std::cout << "Total chunks: " << chunks.size() << std::endl;
        
        // if(chunks.empty()) {
        //     std::cout << "ERROR: Chunks vector is empty!" << std::endl;
        //     std::cout << "World initialization may have failed or chunks were cleared." << std::endl;
        //     debugWorldState();
        // }
    }
    
    for(size_t i = 0; i < chunks.size(); i++) {
        const auto& chunk = chunks[i];
        if(!chunk) {
            // if(showDetailed) std::cout << "Chunk " << i << " is nullptr" << std::endl;
            continue;
        }
        
        cgp::vec3 chunkPos = chunk->getWorldPosition();
        ChunkSize size = chunk->getSize();
        
        bool withinX = worldPos.x >= chunkPos.x && worldPos.x < chunkPos.x + size.width;
        bool withinY = worldPos.y >= chunkPos.y && worldPos.y < chunkPos.y + size.height;
        bool withinZ = worldPos.z >= chunkPos.z && worldPos.z < chunkPos.z + size.depth;
        
        if(withinX && withinY && withinZ) {
            // if(showDetailed) std::cout << "*** FOUND MATCHING CHUNK " << i << " ***" << std::endl;
            return chunk;
        }
    }
    
    // if(showDetailed) std::cout << "*** NO CHUNK FOUND ***" << std::endl;
    return nullptr;
}

void World::renderInstanced(const cgp::vec3& position, const cgp::environment_generic_structure& environment) {
    // Initialize instanced drawables once
    Block::initialize_instanced_drawables();
    
    // Clear all instances from previous frame
    Block::clear_all_instances();
    
    // Collect instances from all chunks within render distance
    for (const auto& chunk : chunks) {
        if (chunk && chunk->isGenerated()) {
            cgp::vec3 chunkCenter = chunk->getChunkCenter();
            float distance = cgp::norm(position - chunkCenter);
            
            if (distance < renderDistance) {
                // Get surface blocks for this chunk
                if (!chunk->surfaceBlocksCached) {
                    chunk->cachedSurfaceBlocks.clear();
                    chunk->findSurfaceBlocksBFS(chunk->cachedSurfaceBlocks);
                    chunk->surfaceBlocksCached = true;
                }
                
                // Add instances for each surface block
                for (const auto& [x, y, z] : chunk->cachedSurfaceBlocks) {
                    BlockType blockType = chunk->getBlock(x, y, z);
                    cgp::vec3 worldPos = chunk->localToWorld({
                        static_cast<float>(x), 
                        static_cast<float>(y), 
                        static_cast<float>(z)
                    });
                    
                    // Add to appropriate instance list
                    switch (blockType) {
                        case GRASS:
                            Block::add_grass_instance(worldPos);
                            break;
                        case STONE:
                            Block::add_stone_instance(worldPos);
                            break;
                        case SAND:
                            Block::add_sand_instance(worldPos);
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }
    
    // Render all collected instances
    Block::render_all_instances(environment);
}