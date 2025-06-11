#include "world.hpp"

World::World():worldSizeX(0), worldSizeZ(0), renderDistance(0){}


World::~World(){
    // std::cout << "World destructor called - deleting " << chunks.size() << " chunks" << std::endl;
    // for(Chunk* chunk : chunks){
    //     delete chunk;
    // }
    // std::cout << "World destructor completed" << std::endl;
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
    Chunk dummyChunk;

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

            chunk->setWorld(this);

            worldGenerator.generateChunk(*chunk, chunkWorldPos);


            chunk->populateBlockObjects();
            chunk->markAsGenerated();

            chunks.push_back(chunk);
        }

        for(auto& chunk : chunks) {
        if(chunk) {
            chunk->surfaceBlocksCached = false;
        }
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



// Modified getChunkAt with better debugging:
Chunk* World::getChunkAt(const cgp::vec3& worldPos) const {
    static int callCount = 0;
    callCount++;
    
    
    
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

// Replace renderFrustum in world.cpp with this improved version:
void World::renderFrustum(const cgp::vec3& position, const cgp::environment_generic_structure& environment,
                          const cgp::mat4& viewMatrix, const cgp::mat4& projectionMatrix){
    
    // Extract camera forward direction
    cgp::vec3 cameraForward = {-viewMatrix(0,2), -viewMatrix(1,2), -viewMatrix(2,2)};
    cameraForward = cgp::normalize(cameraForward);
    
    int chunksRendered = 0;
    int chunksDistanceCulled = 0;
    int chunksFrustumCulled = 0;
    
    for(const auto& chunk : chunks){
        if(chunk && chunk->isGenerated()){
            cgp::vec3 chunkCenter = chunk->getChunkCenter();

            // Distance culling first
            float distance = cgp::norm(position - chunkCenter);
            if(distance >= renderDistance) {
                chunksDistanceCulled++;
                continue;
            }
            
            // Very simple frustum culling - only cull chunks directly behind
            cgp::vec3 toChunk = cgp::normalize(chunkCenter - position);
            float forwardDot = cgp::dot(toChunk, cameraForward);
            
            // Only cull if chunk is directly behind camera (very conservative)
            if (forwardDot < -0.8f) {  // Only cull chunks more than 145 degrees behind
                chunksFrustumCulled++;
                continue;
            }
            
            // Render everything else
            chunk->renderCached(environment);
            chunksRendered++;
        }
    }
    
    static int frameCount = 0;
    if(++frameCount % 60 == 0) {
        std::cout << "Minimal Camera Culling - Rendered: " << chunksRendered 
                  << ", Distance culled: " << chunksDistanceCulled
                  << ", Frustum culled: " << chunksFrustumCulled << std::endl;
    }
}