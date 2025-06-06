#include "world.hpp"

World::World():worldSizeX(0), worldSizeZ(0), renderDistance(0){}

World::~World(){
    for(Chunk* chunk : chunks){
        delete chunk;
    }
}

void World::initialize(int pWorldSizeX, int pWorldSizeZ, int pRenderDistance){
    worldSizeX = pWorldSizeX;
    worldSizeZ = pWorldSizeZ;
    renderDistance = pRenderDistance;
    
    worldGenerator.initialize();
    generateWorld();
    std::cout << "Initialized world with - Size: " << worldSizeX << 
                    "x " << worldSizeZ << std::endl;
}

void World::generateWorld(){
    chunks.clear();

    for(int x = 0; x < worldSizeX; x++){
        for(int z = 0; z < worldSizeZ; z++){
            cgp::vec3 chunkWorldPos = {x * dummyChunk.getSize().width, 0, z * dummyChunk.getSize().depth};
            
            Chunk* chunk = new Chunk(chunkWorldPos);
            
            chunk->initialize();

            worldGenerator.generateChunk(*chunk, chunkWorldPos);

            chunk->populateBlockObjects();
            chunk->markAsGenerated();

            chunks.push_back(chunk);

            // std::cout << "Generated chunk at: " << chunkWorldPos << std::endl;
        }
    }
}

void World::render(const cgp::vec3& position, const cgp::environment_generic_structure& environment){
    for(const auto& chunk : chunks){
        if(chunk && chunk->isGenerated()){
            cgp::vec3 chunkCenter = chunk->getChunkCenter();

            float distance = cgp::norm(position - chunkCenter);

            if(distance < renderDistance){
                chunk->render(environment);
            }
        }
    }
}

Chunk* World::getChunkAt(const cgp::vec3& worldPos) const {
    for(const auto& chunk : chunks) {
        cgp::vec3 chunkPos = chunk->getWorldPosition();
        
        // Check if world position is within this chunk
        if(worldPos.x >= chunkPos.x && worldPos.x < chunkPos.x + chunk->getSize().width &&
           worldPos.y >= chunkPos.y && worldPos.y < chunkPos.y + chunk->getSize().height &&
           worldPos.z >= chunkPos.z && worldPos.z < chunkPos.z + chunk->getSize().depth) {
            return chunk;
        }
    }
    return nullptr;
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
    return nullptr;
}

BlockType World::getBlock(const cgp::vec3& worldPos) const {
    Chunk* chunk = getChunkAt(worldPos);
    if(chunk) {
        cgp::vec3 localPos = chunk->worldToLocal(worldPos);
        return chunk->getBlock(
            static_cast<int>(localPos.x), 
            static_cast<int>(localPos.y), 
            static_cast<int>(localPos.z)
        );
    }
    return AIR;
}


int World::getRenderDistance() const{
    return renderDistance;
}